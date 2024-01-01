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
    set(boost_pylib "${BOOST_LIBRARYDIR}/libboost_python38.so")
elseif ($ENV{DISTINGUISHED_NICKNAME} STREQUAL "Asus_Win7_MSYS2")
    set(boost_pylib "${BOOST_LIBRARYDIR}/libboost_python38-mgw12-mt-x64-1_83.dll" "${Python3_LIBRARIES}")
else()
    set(boost_pylib "unknown_placeholder")
endif()

#set(boost_pylib "${BOOST_LIBRARYDIR}/libboost_python36-mgw12-mt-x64-1_79.dll")
#set(boost_pylib "${BOOST_LIBRARYDIR}/libboost_python38-mgw12-mt-x64-1_83.dll")

target_link_libraries(${BPY_run} ${boost_pylib})
target_link_directories(${BPY_run} PUBLIC ${BOOST_LIBRARYDIR})


#-------------------------------------------------------------------------------------
# A shared library named "winmingwrun"
# 
# Usage of this lib "winmingwrun"

# (1) cd directory: ${REPO_ROOT}/build
# (2) Invoke python
# (3) >>> import ctypes
#     >>> dl = ctypes.windll.LoadLibrary
#     >>> lib = dl('./pyrun/winmingwrun.dll')
#     >>> lib.run_normal()
#
#     Use windll for Windows API and functions
#     compiled with __stdcall, i.e., ctypes.windll.LoadLibrary
#
# (4) Now (2024-01-01) the following error occurs, and I haven't found the root cause,
# 
#     Pyrad@SSEA MINGW64 /d/Gitee/cpp11/build
#     $ python
#     Python 3.10.5 (main, Jun 18 2022, 01:16:30)  [GCC 12.1.0 64 bit (AMD64)] on win32
#     Type "help", "copyright", "credits" or "license" for more information.
#     >>> import ctypes
#     >>> dl = ctypes.windll.LoadLibrary
#     >>> lib = dl('./pyrun/winmingwrun.dll')
#     Traceback (most recent call last):
#       File "<stdin>", line 1, in <module>
#       File "D:/procs/msys64/mingw64/lib/python3.10/ctypes/__init__.py", line 452, in LoadLibrary
#         return self._dlltype(name)
#       File "D:/procs/msys64/mingw64/lib/python3.10/ctypes/__init__.py", line 374, in __init__
#         self._handle = _dlopen(self._name, mode)
#     FileNotFoundError: Could not find module 'D:\Gitee\cpp11\build\pyrun\winmingwrun.dll' (or one of its dependencies). Try using the full path with constructor syntax.
#     >>>
#
# (5) !!! Need to fix the issue above !!!
#-------------------------------------------------------------------------------------
set(winmingwrun "winmingwrun")
add_library(${winmingwrun} SHARED winmingwrun.cpp)
set_target_properties(${winmingwrun} PROPERTIES PREFIX "")

target_include_directories(${winmingwrun}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${PROJECT_SOURCE_DIR}
        )

# Create a list to append libraries for winmingwrun to link
set(LIB_LIST "")

set(LIB_UTILITIES "utilities")
# add_subdirectory(${LIB_UTILITIES})
list(APPEND LIB_LIST ${LIB_UTILITIES})

set(LIB_NORMAL "normal")
# add_subdirectory(${LIB_NORMAL})
list(APPEND LIB_LIST ${LIB_NORMAL})

set(LIB_BOOSTTEST "boosttest")
# add_subdirectory(${LIB_BOOSTTEST})
list(APPEND LIB_LIST ${LIB_BOOSTTEST})

set(LIB_CPPFEATURES "cppfeatures")
# add_subdirectory(${LIB_CPPFEATURES})
list(APPEND LIB_LIST ${LIB_CPPFEATURES})

set(LIB_EFFECTIVE_MODERN_CPP "EffectiveModernCpp")
# add_subdirectory(${LIB_EFFECTIVE_MODERN_CPP})
list(APPEND LIB_LIST ${LIB_EFFECTIVE_MODERN_CPP})

# Set link library
target_link_libraries(${winmingwrun} PUBLIC ${LIB_LIST})

# # Create a directory list to append path for linking
set(LINK_DIRS "")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/normal/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/boosttest/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/EffectiveModernCpp/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/cppfeatures/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/pyrun/")
target_link_directories(${winmingwrun} PUBLIC ${LINK_DIRS} )

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR ${CurSharedLibName}] [END] -------------") 
