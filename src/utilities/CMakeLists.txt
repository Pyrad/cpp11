MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR utilities] -------------") 
MESSAGE(STATUS "[PYRAD]   CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

add_library(utilities SHARED utilities.cpp)

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(utilities
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR utilities] [END] -------------") 
