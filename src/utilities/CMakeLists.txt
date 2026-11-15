add_library(utilities SHARED utilities.cpp)

# cpp11_config provides <repo>/src, the generated projectconfig.h and
# Boost::headers (utilities.hpp includes boost/type_index.hpp).
target_link_libraries(utilities PUBLIC cpp11_config PRIVATE cpp11_warnings)
target_include_directories(utilities PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
