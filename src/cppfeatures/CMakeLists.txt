MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR cppfeatures] -------------") 
MESSAGE(STATUS "[PYRAD]   CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cppfeatures SHARED lvalue_rvalue.cpp)

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(cppfeatures
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${PROJECT_SOURCE_DIR}
        )

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR cppfeatures] [END] -------------") 
