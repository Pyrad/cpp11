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

# PROJECT_VERSION_* comes from project(cpp11 VERSION 0.1) in the root file.
# @ONLY keeps ${...} sequences inside this template untouched.
configure_file(projectconfig.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/projectconfig.h @ONLY)

# ---------------------------------------------------------------------------
# INTERFACE targets carry the requirements shared by every module.
# ---------------------------------------------------------------------------
add_library(cpp11_config INTERFACE)
target_include_directories(cpp11_config INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>            # "pyrun/winmingwrun.hpp" etc.
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
