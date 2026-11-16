add_library(utilities SHARED utilities.cpp)

# cpp11_config provides <repo>/src, the generated projectconfig.h and
# Boost::headers (utilities.hpp includes boost/type_index.hpp).
target_link_libraries(utilities PUBLIC cpp11_config PRIVATE cpp11_warnings)
# PRIVATE: other modules must include this one as "utilities/utilities.hpp"
# through CPP11_SOURCE_ROOT, not by adding this directory to their own search
# path. That keeps one canonical spelling for every cross-module include.
target_include_directories(utilities PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
