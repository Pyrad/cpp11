add_library(boosttest SHARED boosttest.cpp)

# boost/geometry, boost/bimap, boost/circular_buffer etc. are header-only, so
# Boost::headers (via cpp11_config) is the only Boost requirement today.
target_link_libraries(boosttest PUBLIC cpp11_config PRIVATE cpp11_warnings)
target_include_directories(boosttest PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
