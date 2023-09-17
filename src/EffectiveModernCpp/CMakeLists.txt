MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR EffectiveModernCpp] -------------") 
MESSAGE(STATUS "[PYRAD]   CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

# This is the CMakeLists.txt file for folder EffectiveModernCpp
add_library(EffectiveModernCpp SHARED
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
    chapter5_item24.cpp
    chapter5_item25.cpp
    chapter5_item26.cpp
    chapter5_item27.cpp
    chapter5_item28.cpp
    chapter5_item29.cpp
    chapter5_item30.cpp
    chapter6_item31.cpp
    chapter6_item32.cpp
    chapter6_item33.cpp
    chapter6_item34.cpp
    )

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
# --------
# In order to include headers from other sub-folders, include the source
# directory of current project
# Here variable PROJECT_SOURCE_DIR is a built-in project name, it is
# actually is "D:/Gitee/cpp11/src" in current project, and there are sub
# folders (utilities EffectiveModernCpp normal boosttest) which contains
# other headers, these headers then can be included in a source file as
# below,
# #include "utilities/utilities.hpp"
# 2023-09-17
target_include_directories(EffectiveModernCpp
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE "${PROJECT_SOURCE_DIR}"
        )

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR EffectiveModernCpp] [END] -------------") 
