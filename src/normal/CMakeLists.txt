add_library(normal SHARED normal.cpp maintest.cpp)

target_link_libraries(normal PUBLIC cpp11_config PRIVATE cpp11_warnings)
target_include_directories(normal PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# CGAL 6.x is header-only but requires C++17, and its imported target carries
# the GMP/MPFR libraries used by the exact-constructions kernel.
#
# The link is PRIVATE on purpose: normal.hpp stays CGAL-free, so the C++17
# requirement must not leak to mymainrun / cpp11runtest / winmingwrun, which
# would otherwise lose their ability to test the older CPP11_CXX_STANDARD
# values. For this target itself CGAL::CGAL raises the standard to c++17
# through its INTERFACE compile feature, so no CXX_STANDARD override is needed.
if(ENB_CGAL)
    target_link_libraries(normal PRIVATE CGAL::CGAL)
endif()
