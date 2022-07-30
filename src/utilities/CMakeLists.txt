add_library(utilities utilities.cpp)

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(utilities
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )