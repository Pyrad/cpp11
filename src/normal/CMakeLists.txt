add_library(normal SHARED normal.cpp maintest.cpp)

target_link_libraries(normal PUBLIC cpp11_config PRIVATE cpp11_warnings)
target_include_directories(normal PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
