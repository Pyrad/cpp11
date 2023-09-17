# Set the name of current shared library
set(CurSharedLibName "pyrun")

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR ${CurSharedLibName}] -------------") 
MESSAGE(STATUS "[PYRAD]   CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

# The following 4 variables are artifacts specification
# They can be found in https://cmake.org/cmake/help/latest/module/FindPython3.html
set(Python3_INCLUDE_DIRS "D:/procs/python38/include")
set(Python3_LIBRARIES "D:/procs/python38/python38.dll")
set(Python3_LIBRARY "D:/procs/python38")
set(PYTHON_EXECUTABLE "D:/procs/python38/python.exe")

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

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR ${CurSharedLibName}] [END] -------------") 
