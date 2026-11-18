###########################################################################
# cpp11 -- feature modules and the main executable.
#
# project(), the C++ standard, compiler options and third-party discovery are
# declared in the repository-root CMakeLists.txt; this file only wires up the
# local targets. Dependencies travel through target_link_libraries(), so no
# global include paths or manual link-directory lists are needed.
###########################################################################

# ---------------------------------------------------------------------------
# Feature switches. They are exported to projectconfig.h so that the sources
# can guard their #include and calls with #ifdef.
# ---------------------------------------------------------------------------
option(ENB_NORMAL "Build the normal C++ functionality tests" ON)
option(ENB_BOOSTTEST "Build the Boost functionality tests" ON)
option(ENB_CPP_FEATURES "Build the C++ language feature tests" ON)
option(ENB_EFFECTIVE_MODERN_CPP "Build the EffectiveModernCpp notes" ON)
option(ENB_PYRUN_CPP "Build the embedded-Python modules" ON)

# CGAL is not a plain ON/OFF switch: it can only be enabled when the package was
# actually found by the root CMakeLists.txt. The result still ends up in
# projectconfig.h as ENB_CGAL, so normal.cpp can guard its #include with it.
if(CPP11_ENABLE_CGAL AND CGAL_FOUND)
    set(ENB_CGAL ON)
else()
    set(ENB_CGAL OFF)
endif()

# Same derivation for oneTBB, for the same reason: the module must know whether
# the package was actually found, not just whether the user wanted it.
if(CPP11_ENABLE_TBB AND TBB_FOUND)
    set(ENB_TBB ON)
else()
    set(ENB_TBB OFF)
endif()

# PROJECT_VERSION_* comes from project(cpp11 VERSION 0.1) in the root file.
# @ONLY keeps ${...} sequences inside this template untouched.
configure_file(projectconfig.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/projectconfig.h @ONLY)

# ---------------------------------------------------------------------------
# INTERFACE targets carry the requirements shared by every module.
#
# CPP11_SOURCE_ROOT is the single include root for cross-module headers. With
# it on the search path a module is always reachable by directory name:
#
#     #include "utilities/utilities.hpp"
#     #include "pyrun/pyrun.hpp"
#
# no matter where the including file lives. The relative form
# "../utilities/utilities.hpp" also compiles, but only because the compiler
# searches the including file's own directory first; it silently breaks as
# soon as that file is moved to another directory, so the module-root form is
# the convention used throughout src/. Dropping this root makes every quoted
# module include fail with "No such file or directory", hence the check below
# so a wrong root fails loudly at configure time instead of at compile time.
# ---------------------------------------------------------------------------
set(CPP11_SOURCE_ROOT "${PROJECT_SOURCE_DIR}/src")
if(NOT EXISTS "${CPP11_SOURCE_ROOT}/utilities/utilities.hpp")
    message(FATAL_ERROR
        "[cpp11] ${CPP11_SOURCE_ROOT}/utilities/utilities.hpp is missing, so "
        "CPP11_SOURCE_ROOT is wrong and quoted includes such as "
        "\"utilities/utilities.hpp\" would fail to compile.")
endif()

add_library(cpp11_config INTERFACE)
target_include_directories(cpp11_config INTERFACE
    $<BUILD_INTERFACE:${CPP11_SOURCE_ROOT}>               # "utilities/utilities.hpp" etc.
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)        # generated projectconfig.h
target_link_libraries(cpp11_config INTERFACE Boost::headers)

add_library(cpp11_warnings INTERFACE)
if(CPP11_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(cpp11_warnings INTERFACE /W4)
    else()
        target_compile_options(cpp11_warnings INTERFACE -Wall -Wextra -Wpedantic)
    endif()
endif()
if(CPP11_ENABLE_ASAN AND NOT MSVC)
    target_compile_options(cpp11_warnings INTERFACE
            -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(cpp11_warnings INTERFACE -fsanitize=address,undefined)
endif()

# ---------------------------------------------------------------------------
# Modules. CPP11_MODULES records the targets that actually exist, which lets
# pyrun's helper libraries link only what was enabled (fixes P0-4).
# ---------------------------------------------------------------------------
set(CPP11_MODULES "")

add_subdirectory(utilities)
list(APPEND CPP11_MODULES utilities)

if(ENB_NORMAL)
    add_subdirectory(normal)
    list(APPEND CPP11_MODULES normal)
endif()
if(ENB_BOOSTTEST)
    add_subdirectory(boosttest)
    list(APPEND CPP11_MODULES boosttest)
endif()
if(ENB_CPP_FEATURES)
    add_subdirectory(cppfeatures)
    list(APPEND CPP11_MODULES cppfeatures)
endif()
if(ENB_EFFECTIVE_MODERN_CPP)
    add_subdirectory(EffectiveModernCpp)
    list(APPEND CPP11_MODULES EffectiveModernCpp)
endif()
if(ENB_PYRUN_CPP)
    add_subdirectory(pyrun)
    list(APPEND CPP11_MODULES pyrun)
endif()

# Only copies top.py; fixed to be a valid, self-contained sub-configuration.
add_subdirectory(pymods)

message(STATUS "[cpp11] Enabled modules: ${CPP11_MODULES}")

# ---------------------------------------------------------------------------
# Main executable
# ---------------------------------------------------------------------------
add_executable(mymainrun main.cpp)
target_link_libraries(mymainrun PRIVATE ${CPP11_MODULES} cpp11_config cpp11_warnings)

add_test(NAME smoke COMMAND mymainrun)
set_tests_properties(smoke PROPERTIES
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    TIMEOUT 180)

# ---------------------------------------------------------------------------
# Self-contained Windows runtime: $<TARGET_RUNTIME_DLLS> collects the DLLs of
# all transitive imported targets (notably python3xx.dll) and copies them next
# to the executable, so no PATH adjustment is required to launch or to ctest.
# ---------------------------------------------------------------------------
if(WIN32 AND CPP11_COPY_RUNTIME_DLLS)
    add_custom_command(TARGET mymainrun POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:mymainrun> $<TARGET_FILE_DIR:mymainrun>
        COMMAND_EXPAND_LISTS
        COMMENT "[cpp11] Copying third-party runtime DLLs next to mymainrun")
endif()

# ---------------------------------------------------------------------------
# Self-contained Windows runtime, part 2: the toolchain's own DLLs.
#
# $<TARGET_RUNTIME_DLLS> only knows about imported *targets*, so it never sees
# libgcc/libstdc++/libwinpthread (they come from the compiler) nor the GMP and
# MPFR DLLs used by CGAL's exact kernels (CGAL passes them as plain library
# paths). Python 3.8+ stops searching PATH when it loads an extension module,
# so `import cpp11runtest` fails with error 126 ("the specified module could not
# be found") unless every one of those DLLs sits next to the .pyd. Copying them
# also makes build/bin launchable without any MSYS2 PATH.
#
# oneTBB belongs in the same list for a different reason: TBB::tbb is linked
# PRIVATE by the normal module, so mymainrun's own $<TARGET_RUNTIME_DLLS> never
# sees libtbb12.dll -- it is a dependency of libnormal.dll, not of the exe.
# ---------------------------------------------------------------------------
if(MINGW AND CPP11_COPY_RUNTIME_DLLS)
    get_filename_component(_cpp11_mingw_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)

    set(_cpp11_toolchain_dlls "")
    foreach(_dll IN ITEMS
            libgcc_s_seh-1.dll libgcc_s_dw2-1.dll
            libstdc++-6.dll libwinpthread-1.dll)
        if(EXISTS "${_cpp11_mingw_bin}/${_dll}")
            list(APPEND _cpp11_toolchain_dlls "${_cpp11_mingw_bin}/${_dll}")
        endif()
    endforeach()

    if(CGAL_FOUND)
        # Globs keep this working when MSYS2 bumps the ABI numbers
        # (libgmp-10.dll, libmpfr-6.dll, libgmpxx-4.dll, ...).
        file(GLOB _cpp11_bignum_dlls
                "${_cpp11_mingw_bin}/libgmp-*.dll"
                "${_cpp11_mingw_bin}/libmpfr-*.dll"
                "${_cpp11_mingw_bin}/libgmpxx-*.dll")
        list(APPEND _cpp11_toolchain_dlls ${_cpp11_bignum_dlls})
    endif()

    if(ENB_TBB AND TARGET TBB::tbb)
        # Resolved through the imported target, so the file name (libtbb12.dll
        # for MinGW, tbb12.dll for MSVC) and its directory follow the install.
        list(APPEND _cpp11_toolchain_dlls $<TARGET_FILE:TBB::tbb>)
    endif()

    if(_cpp11_toolchain_dlls)
        add_custom_command(TARGET mymainrun POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${_cpp11_toolchain_dlls} $<TARGET_FILE_DIR:mymainrun>
            COMMENT "[cpp11] Copying MinGW/CGAL/TBB runtime DLLs next to mymainrun")
    endif()
endif()
