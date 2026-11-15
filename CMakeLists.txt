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

message(STATUS "[cpp11] ==========================================================")
message(STATUS "[cpp11] C++ standard   : ${CPP11_CXX_STANDARD} (extensions OFF)")
message(STATUS "[cpp11] Boost          : ${Boost_VERSION}  (${Boost_INCLUDE_DIRS})")
message(STATUS "[cpp11] Python         : ${Python3_VERSION}  (${Python3_EXECUTABLE})")
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
