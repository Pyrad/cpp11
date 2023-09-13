MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR boosttest] -------------") 
MESSAGE(STATUS "[PYRAD]   CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

add_library(boosttest SHARED boosttest.cpp)

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(boosttest
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR boosttest] [END] -------------") 
