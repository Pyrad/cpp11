MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR normal] -------------") 
MESSAGE(STATUS "[PYRAD]   ----- CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR} -----") 

add_library(normal SHARED normal.cpp maintest.cpp)

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(normal
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR normal] [END] -------------") 
