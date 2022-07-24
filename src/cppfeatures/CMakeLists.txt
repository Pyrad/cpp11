add_library(cppfeatures lvalue_rvalue.cpp)

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(cppfeatures
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )