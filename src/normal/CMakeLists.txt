add_library(normal SHARED normal.cpp maintest.cpp)

target_link_libraries(normal PUBLIC cpp11_config PRIVATE cpp11_warnings)
target_include_directories(normal PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# CGAL 6.x is header-only but requires C++17, and its imported target carries
# the GMP/MPFR libraries used by the exact-constructions kernel.
#
# Both third-party links are PRIVATE on purpose: normal.hpp stays free of CGAL
# and TBB headers, so no third-party requirement may leak to mymainrun /
# cpp11runtest / winmingwrun, which would otherwise lose their ability to test
# the older CPP11_CXX_STANDARD values.
if(ENB_CGAL)
    target_link_libraries(normal PRIVATE CGAL::CGAL)
    # CGAL::CGAL already carries "INTERFACE cxx_std_17", which is enough to
    # compile this target as C++17; no explicit CXX_STANDARD is needed and none
    # is set, so the standard keeps following the package's own contract.
endif()

# oneTBB is not header-only: the headers call into the task scheduler that
# lives in tbb12.dll, so the shared library has to be linked. Unlike CGAL it
# exports no compile feature: its headers were verified to build under
# -std=c++11, c++14 and c++17 alike, so linking TBB::tbb deliberately does not
# raise the standard either.
if(ENB_TBB)
    target_link_libraries(normal PRIVATE TBB::tbb)
    # oneTBB 2023.2.0 has a MinGW-only defect in oneapi/tbb/profiling.h: when
    # profiling is enabled, the overload taking "const char*" forwards to
    # r1::itt_set_sync_name(void*, const tchar*), and tchar is wchar_t on
    # Windows, so the call does not compile. Profiling is enabled whenever
    # TBB_USE_DEBUG is set, which TBBTargets.cmake injects for every Debug
    # configuration through INTERFACE_COMPILE_DEFINITIONS -- hence a Release
    # build compiles and a Debug build does not.
    #
    # These are ITT/VTune notifications only, so pinning the feature off is
    # cheaper and safer than patching the installed third-party header.
    target_compile_definitions(normal PRIVATE TBB_USE_PROFILING_TOOLS=0)
endif()
