###########################################################################
# cpp11 -- a playground for verifying C++ language features.
#
# The project entry point now lives at the repository root (review document
# phase 4), so the standard commands work:
#
#   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#   ./build/bin/mymainrun.exe
#
# Machine-specific dependency roots are NOT hard-coded here. Supply them via
# CMakeUserPresets.json (gitignored), -DCMAKE_PREFIX_PATH / -DPython3_ROOT_DIR,
# or the CPP11_BOOST_ROOT / CPP11_PYTHON_ROOT environment variables.
###########################################################################

cmake_minimum_required(VERSION 3.21)

project(cpp11
        VERSION 0.1
        DESCRIPTION "Experiments for C++ language features"
        LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Build knobs (phase 5)
# ---------------------------------------------------------------------------
set(CPP11_CXX_STANDARD "14" CACHE STRING "C++ standard used for the feature tests")
set_property(CACHE CPP11_CXX_STANDARD PROPERTY STRINGS 11 14 17 20 23)
set(CMAKE_CXX_STANDARD ${CPP11_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # -std=c++14 instead of -std=gnu++14
if(CPP11_CXX_STANDARD LESS 14)
    # Kept as an explicit experiment result rather than a hard error: the
    # sources use generic lambdas (normal/maintest.cpp) and Boost 1.92 needs
    # std::is_final / std::remove_cv_t, so C++11 cannot compile today.
    message(WARNING
        "CPP11_CXX_STANDARD=${CPP11_CXX_STANDARD}: C++11 builds are known to "
        "fail (generic lambdas + Boost 1.92 requires C++14). Use 14 or newer.")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
                 Debug Release RelWithDebInfo MinSizeRel)
endif()

option(CPP11_ENABLE_WARNINGS "Compile project targets with -Wall/-Wextra/-Wpedantic" ON)
option(CPP11_ENABLE_ASAN "Build with AddressSanitizer + UBSan (GCC/Clang only)" OFF)
option(CPP11_COPY_RUNTIME_DLLS
       "On Windows, copy third-party runtime DLLs next to the executable" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # clangd / IDE support

# ---------------------------------------------------------------------------
# One output directory for every runtime artifact (phase 3)
#
# Windows loads dependent DLLs from the executable's directory, and Linux is
# handled with $ORIGIN below. No PATH / LD_LIBRARY_PATH tweaking needed.
# ---------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
foreach(cfg IN ITEMS DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfg} "${CMAKE_BINARY_DIR}/lib")
endforeach()

if(UNIX AND NOT APPLE)
    set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
    set(CMAKE_BUILD_RPATH "$ORIGIN")
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()

# ---------------------------------------------------------------------------
# Dependency discovery (phase 2)
#
# DEV_NICKNAME used to hard-code absolute Boost/Python paths per machine.
# find_package + imported targets now validate versions and paths; machine
# differences move to CMakeUserPresets.json / -D flags.
# ---------------------------------------------------------------------------
if(DEFINED ENV{CPP11_BOOST_ROOT} AND NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "$ENV{CPP11_BOOST_ROOT}")
endif()
if(DEFINED ENV{CPP11_PYTHON_ROOT} AND NOT DEFINED Python3_ROOT_DIR)
    set(Python3_ROOT_DIR "$ENV{CPP11_PYTHON_ROOT}")
endif()

find_package(Boost 1.70 CONFIG REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# ---------------------------------------------------------------------------
# Optional dependency: CGAL
#
# CGAL 6.x is header-only, but it requires C++17 and, for exact constructions,
# the non-header-only GMP/MPFR libraries. Its imported target CGAL::CGAL
# carries the include dirs, cxx_std_17 and the GMP/MPFR link interface, so
# nothing has to be spelled out by hand here.
#
# A missing CGAL is not fatal: the example is compiled out and the build keeps
# working, which matters for the C++11/C++14 experiments and for machines that
# do not have CGAL installed.
# ---------------------------------------------------------------------------
option(CPP11_ENABLE_CGAL
       "Build the CGAL example in the normal module (needs CGAL 6.x + GMP/MPFR)" ON)
if(CPP11_ENABLE_CGAL)
    # Same convention as CPP11_BOOST_ROOT / CPP11_PYTHON_ROOT above: a machine
    # whose CGAL lives outside CMAKE_PREFIX_PATH can export CPP11_CGAL_ROOT
    # (the install prefix, e.g. D:/proc/cgal6p2p1) instead of hard-coding the
    # path here or passing -DCGAL_DIR on every configure.
    if(DEFINED ENV{CPP11_CGAL_ROOT} AND NOT DEFINED CGAL_DIR)
        set(CGAL_DIR "$ENV{CPP11_CGAL_ROOT}/lib/cmake/CGAL")
    endif()
    find_package(CGAL 6.0 QUIET)
    if(NOT CGAL_FOUND)
        message(WARNING
            "CGAL not found: the CGAL example will be compiled out. "
            "Pass -DCGAL_DIR=<prefix>/lib/cmake/CGAL, or add the install "
            "prefix to CMAKE_PREFIX_PATH (or set CPP11_CGAL_ROOT to it), or "
            "configure with -DCPP11_ENABLE_CGAL=OFF to silence this warning.")
    endif()
endif()

# ---------------------------------------------------------------------------
# Optional dependency: oneTBB
#
# oneTBB is the opposite case of CGAL: its headers are thin wrappers over the
# task scheduler that lives in tbb12.dll, so TBB::tbb has to be linked. It does
# not declare any compile feature of its own -- its headers were verified to
# build under C++11/14/17 -- so it does not raise CPP11_CXX_STANDARD either.
#
# Its TBBConfig.cmake is minimal -- it only includes TBBTargets.cmake -- so the
# import target is the reliable indicator that the package is usable here, not
# the TBB_FOUND variable alone.
#
# Same policy as CGAL: a missing oneTBB warns and compiles the example out.
# ---------------------------------------------------------------------------
option(CPP11_ENABLE_TBB
       "Build the oneTBB example in the normal module (needs oneTBB 2021+)" ON)
if(CPP11_ENABLE_TBB)
    find_package(TBB 2021 QUIET)
    if(TBB_FOUND AND NOT TARGET TBB::tbb)
        set(TBB_FOUND FALSE)
    endif()
    if(NOT TBB_FOUND)
        message(WARNING
            "oneTBB not found: the TBB example will be compiled out. "
            "Pass -DTBB_DIR=<prefix>/lib/cmake/TBB, or add the install prefix "
            "to CMAKE_PREFIX_PATH, or configure with -DCPP11_ENABLE_TBB=OFF "
            "to silence this warning.")
    endif()
endif()

message(STATUS "[cpp11] ==========================================================")
message(STATUS "[cpp11] C++ standard   : ${CPP11_CXX_STANDARD} (extensions OFF)")
message(STATUS "[cpp11] Boost          : ${Boost_VERSION}  (${Boost_INCLUDE_DIRS})")
message(STATUS "[cpp11] Python         : ${Python3_VERSION}  (${Python3_EXECUTABLE})")
if(CGAL_FOUND)
    message(STATUS "[cpp11] CGAL           : ${CGAL_VERSION}  (${CGAL_DIR})")
else()
    message(STATUS "[cpp11] CGAL           : not used")
endif()
if(TBB_FOUND)
    message(STATUS "[cpp11] oneTBB         : ${TBB_VERSION}  (${TBB_DIR})")
else()
    message(STATUS "[cpp11] oneTBB         : not used")
endif()
message(STATUS "[cpp11] Build type     : ${CMAKE_BUILD_TYPE}")
message(STATUS "[cpp11] Runtime output : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "[cpp11] ==========================================================")

# When python3xx.dll is copied next to the executable, CPython would otherwise
# derive sys.prefix from that DLL location and lose its standard library.
# CPP11_PYTHON_HOME is passed to pyrun.cpp, which pins PyConfig.home instead.
if(WIN32 AND CPP11_COPY_RUNTIME_DLLS)
    get_filename_component(_cpp11_py_prefix "${Python3_EXECUTABLE}" DIRECTORY)
    set(CPP11_PYTHON_HOME "${_cpp11_py_prefix}" CACHE PATH
        "Python installation prefix used by the embedded interpreter")
endif()

enable_testing()
add_subdirectory(src)
