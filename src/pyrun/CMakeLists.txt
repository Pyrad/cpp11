# Set the name of current shared library
set(CurSharedLibName "pyrun")

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR ${CurSharedLibName}] -------------") 
MESSAGE(STATUS "[PYRAD]   CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

# # The following 4 variables are artifacts specification
# # They can be found in https://cmake.org/cmake/help/latest/module/FindPython3.html
# set(Python3_INCLUDE_DIRS "D:/procs/python38/include")
# set(Python3_LIBRARIES "D:/procs/python38/python38.dll")
# set(Python3_LIBRARY "D:/procs/python38")
# set(PYTHON_EXECUTABLE "D:/procs/python38/python.exe")
# 
# if (DEFINED ENV{DISTINGUISHED_NICKNAME})
#     MESSAGE(STATUS "[PYRAD]    A customized env variable DISTINGUISHED_NICKNAME is defined to distinguish host machines!")
#     if ($ENV{DISTINGUISHED_NICKNAME} STREQUAL "Asus_Win7_VBox_Ubuntu_20_04_LTS")
#         # For Ubuntu 20.04 LTS installed on VirtualBox of Windows 7 (Asus desktop)
#         set(Python3_INCLUDE_DIRS "/home/pyrad/procs/Python-3.8.3/include/python3.8")
#         set(Python3_LIBRARIES "/home/pyrad/procs/Python-3.8.3/lib/libpython3.8.so")
#         set(Python3_LIBRARY "/home/pyrad/procs/Python-3.8.3/lib")
#         set(PYTHON_EXECUTABLE "/home/pyrad/procs/Python-3.8.3/bin/python3")
#     endif()
# else()
#     MESSAGE(FATAL_ERROR "[PYRAD]    DISTINGUISHED_NICKNAME must be set to distinguish host machines!")
# endif()

# Following variables are set in src/CMakeLists.txt
MESSAGE(STATUS "[PYRAD]     Python related variables")
MESSAGE(STATUS "[PYRAD]     Python3_INCLUDE_DIRS = ${Python3_INCLUDE_DIRS}")
MESSAGE(STATUS "[PYRAD]     Python3_LIBRARIES = ${Python3_LIBRARIES}")
MESSAGE(STATUS "[PYRAD]     Python3_LIBRARY = ${Python3_LIBRARY}")
MESSAGE(STATUS "[PYRAD]     PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")

#set(Python3_INCLUDE_DIRS "/mingw64/include/python3.10")
#set(Python3_LIBRARIES "/mingw64/bin/libpython3.10.dll")
#set(Python3_LIBRARY "/mingw64/bin")
#set(PYTHON_EXECUTABLE "/mingw64/bin/python")

# This is the CMakeLists.txt file for folder pyrun
add_library(${CurSharedLibName} SHARED pyrun.cpp)

# Remember INTERFACE means things that consumers 
# require but the producer doesn't. 
target_include_directories(${CurSharedLibName}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${Python3_INCLUDE_DIRS}
        PRIVATE ${PROJECT_SOURCE_DIR}
        )

target_link_libraries(${CurSharedLibName} ${Python3_LIBRARIES})
target_link_directories(${CurSharedLibName} PUBLIC ${Python3_LIBRARY})

# After compiled, when to run, remember set PATH (on Windows) or LIBRARA_PATH (on linux)
# to make sure this shared object after compiled can find the python38.dll it needs.
# 
# If it is not set accoordingly, then it can't find it, and an error would be reported.
# 
# See README.md of this project to find more details
#
# Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ ldd build/pyrun/libpyrun.dll
#         ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x76eb0000)
#         kernel32.dll => /c/Windows/system32/kernel32.dll (0x76b50000)
#         KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefc880000)
#         msvcrt.dll => /c/Windows/system32/msvcrt.dll (0x7fefeb20000)
#         libpyrun.dll => /d/Gitee/cpp11/build/pyrun/libpyrun.dll (0x7feee5f0000)
#         libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (0x7fed51c0000)
#         libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (0x7fef4480000)
#         libwinpthread-1.dll => /mingw64/bin/libwinpthread-1.dll (0x7fef2360000)
#         python38.dll => not found

# Another shared library, but it doesn't have to be linked to the main program,
# as it can be used stand alone for Python import modules
set(PyMod_cpp11runtest "cpp11runtest")
add_library(${PyMod_cpp11runtest} SHARED cpp11runtest.cpp)
set_target_properties(${PyMod_cpp11runtest} PROPERTIES PREFIX "")
target_include_directories(${PyMod_cpp11runtest}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${Python3_INCLUDE_DIRS}
        PRIVATE ${PROJECT_SOURCE_DIR}
        )
target_link_libraries(${PyMod_cpp11runtest} ${Python3_LIBRARIES})
target_link_directories(${PyMod_cpp11runtest} PUBLIC ${Python3_LIBRARY})



# A shared library named "bpyrun", which I intended to build it as a library to be imported
# directly in Python.
# Though it can be compiled and linked successfully, it failed to be imported in Python on
# MSYS2, Windows platform, I still haven't figured out why... (2023-10-04)
set(BPY_run "bpyrun")
add_library(${BPY_run} SHARED bpyrun.cpp)
set_target_properties(${BPY_run} PROPERTIES PREFIX "")
target_include_directories(${BPY_run}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${BOOST_INCLUDEDIR}
        PRIVATE ${Python3_INCLUDE_DIRS}
        PRIVATE ${PROJECT_SOURCE_DIR}
        )

# Boost.Python still needs python library, set it accoordingly.
set(boost_pylib "unknown_placeholder")
if ($ENV{DISTINGUISHED_NICKNAME} STREQUAL "Asus_Win7_VBox_Ubuntu_20_04_LTS")
    set(boost_pylib "unknown_placeholder")
elseif ($ENV{DISTINGUISHED_NICKNAME} STREQUAL "Asus_Win7_MSYS2")
    set(boost_pylib "${BOOST_LIBRARYDIR}/libboost_python38-mgw12-mt-x64-1_83.dll" "${Python3_LIBRARIES}")
else()
    set(boost_pylib "unknown_placeholder")
endif()

#set(boost_pylib "${BOOST_LIBRARYDIR}/libboost_python36-mgw12-mt-x64-1_79.dll")
#set(boost_pylib "${BOOST_LIBRARYDIR}/libboost_python38-mgw12-mt-x64-1_83.dll")

target_link_libraries(${BPY_run} ${boost_pylib})
target_link_directories(${BPY_run} PUBLIC ${BOOST_LIBRARYDIR})


MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR ${CurSharedLibName}] [END] -------------") 
