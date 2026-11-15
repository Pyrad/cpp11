# CONFIGURE_DEPENDS makes CMake re-glob on every build, so a new chapter/item
# .cpp produced by gen_emcxx_new_chapter.sh is compiled without editing this
# file. (Registering it in effectiveModernCpp.cpp is still a manual step.)
file(GLOB CPP11_EMCXX_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(SORT CPP11_EMCXX_SOURCES)

add_library(EffectiveModernCpp SHARED ${CPP11_EMCXX_SOURCES})

target_link_libraries(EffectiveModernCpp PUBLIC cpp11_config PRIVATE cpp11_warnings)
target_include_directories(EffectiveModernCpp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
