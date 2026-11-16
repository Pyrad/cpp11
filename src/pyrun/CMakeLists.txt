###########################################################################
# Embedded Python / ctypes helper libraries.
#
# Artifact naming (review document P0-6): only cpp11runtest is a genuine
# Python extension module and therefore gets the .pyd/.so + no-lib-prefix
# treatment. pyrun and winmingwrun are ordinary shared libraries linked by
# CMake targets, so they keep the platform default (libpyrun.dll,
# libwinmingwrun.dll / libpyrun.so, libwinmingwrun.so). The old .pyd on all
# three disagreed with README, run.py and set_shared_lib_path.sh.
###########################################################################

if(WIN32)
    set(CPP11_PYMOD_SUFFIX ".pyd")
else()
    set(CPP11_PYMOD_SUFFIX ".so")
endif()

file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/run.py
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run.py
        COPYONLY)

# ---------------------------------------------------------------------------
# pyrun: the embedded interpreter helper linked by mymainrun.
# ---------------------------------------------------------------------------
add_library(pyrun SHARED pyrun.cpp)
target_link_libraries(pyrun PRIVATE Python3::Python cpp11_config cpp11_warnings)
target_include_directories(pyrun PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_definitions(pyrun PRIVATE
        CPP11_PYRUN_SCRIPT="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run.py")
if(CPP11_PYTHON_HOME)
    target_compile_definitions(pyrun PRIVATE
            CPP11_PYTHON_HOME="${CPP11_PYTHON_HOME}")
endif()

# ---------------------------------------------------------------------------
# cpp11runtest: the module imported by Python ("import cpp11runtest").
# CPP11_MODULES only contains targets that were actually created, so turning
# off any ENB_* switch no longer leaves a dangling -l<module> on the link line.
# ---------------------------------------------------------------------------
add_library(cpp11runtest SHARED cpp11runtest.cpp)
set_target_properties(cpp11runtest PROPERTIES
        PREFIX "" SUFFIX "${CPP11_PYMOD_SUFFIX}")
target_link_libraries(cpp11runtest PRIVATE
        ${CPP11_MODULES} Python3::Module cpp11_config cpp11_warnings)

# ---------------------------------------------------------------------------
# winmingwrun: a plain ctypes DLL, not a Python extension module.
# ---------------------------------------------------------------------------
add_library(winmingwrun SHARED winmingwrun.cpp)
target_link_libraries(winmingwrun PRIVATE
        ${CPP11_MODULES} pyrun Python3::Python cpp11_config cpp11_warnings)
