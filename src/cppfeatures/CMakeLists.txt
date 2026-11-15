add_library(cppfeatures SHARED lvalue_rvalue.cpp)

target_link_libraries(cppfeatures PUBLIC cpp11_config PRIVATE cpp11_warnings)
target_include_directories(cppfeatures PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
