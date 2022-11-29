MESSAGE(STATUS "[PYRAD] [DIR boosttest] CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

add_library(boosttest boosttest.cpp)

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(boosttest
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )
