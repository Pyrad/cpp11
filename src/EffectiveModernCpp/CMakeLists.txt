MESSAGE(STATUS "[PYRAD] [DIR EffectiveModernCpp] CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

# This is the CMakeLists.txt file for folder EffectiveModernCpp
add_library(EffectiveModernCpp 
    effectiveModernCpp.cpp
    chapter1_item01.cpp
    chapter1_item02.cpp
    chapter1_item03.cpp
    chapter1_item04.cpp
    chapter2_item05.cpp
    chapter2_item06.cpp
    chapter3_item07.cpp
    chapter3_item08.cpp
    chapter3_item09.cpp
    chapter3_item10.cpp
    chapter3_item11.cpp
    chapter3_item12.cpp
    chapter3_item13.cpp
    chapter3_item14.cpp
    chapter3_item15.cpp
    chapter3_item16.cpp
    chapter3_item17.cpp
    chapter4_item18.cpp
    chapter4_item19.cpp
    chapter4_item20.cpp
    chapter4_item21.cpp
    chapter4_item22.cpp
    chapter5_item23.cpp
    )

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(EffectiveModernCpp
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )
