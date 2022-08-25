MESSAGE(STATUS "[PYRAD] [DIR EffectiveModernCpp] CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

# This is the CMakeLists.txt file for folder EffectiveModernCpp
add_library(EffectiveModernCpp 
    effectiveModernCpp.cpp
    chapter1_item01.cpp
    )

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(EffectiveModernCpp
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )
