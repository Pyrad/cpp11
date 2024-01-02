# cpp11
A repository for testing various features in CPP (c98, c11, and so on)



## Note: !!! Always Keeping 2 Repositories Same !!!

There are 2 repositories working as backups for each other, which are

- From **Gitee**: [git@gitee.com:pyrad/cpp11.git]( git@gitee.com:pyrad/cpp11.git)
- From **GitHub**: [git@github.com:Pyrad/cpp11.git](git@github.com:Pyrad/cpp11.git)

So the 2 repositories must be ***exactly the same*** as each other.

For daily use, clone it from Gitee, and add another URL from Github as `push` URL to keep them synced.

```bash
### Clone to local folder
$ git clone git@gitee.com:pyrad/cpp11.git
### Check current fetch & push origin
$ git remote -v 
origin  git@gitee.com:pyrad/cpp11.git (fetch)
origin  git@gitee.com:pyrad/cpp11.git (push)
### Add another repo as *PUSH* origin
$ git remote set-url --add origin git@github.com:Pyrad/cpp11.git
### Check current fetch & push origin again
$ git remote -v
origin  git@gitee.com:pyrad/cpp11.git (fetch)
origin  git@gitee.com:pyrad/cpp11.git (push)
origin  git@github.com:Pyrad/cpp11.git (push)
```



## Dependencies

Currently only it only depends on`boost` library.

- `boost` library



## Set include paths for header files

If include paths are not correctly set, then it needs to specify the right path in a source file for `cmake` to find the header file, for example as below, in file `effectiveModernCpp.cpp`, we need the the following directive to specify the header we want to include,

```cpp
#include "../utilities/utilities.hpp"
```

Since `effectiveModernCpp.cpp` wants to include `utilities.hpp`, and that file is in a different directory, and `cmake` doesn't know where to find it, so we need to clearly declare that it is a subdirectory of the upper folder (i.e. `../utilities/`).

But if we specify the include path as below in `CMakeLists.txt`, then `cmake` will know where to find these header files included in a source file,

```cmake
target_include_directories(EffectiveModernCpp
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE "${PROJECT_SOURCE_DIR}"
        )
```

Here `${PROJECT_SOURCE_DIR}` is the source folder of current project, and there are a few sub folders in it, such as `utilities`, `normal`, `boosttest` and `EffectiveModernCpp`.

After setting it, now we could just use the directive below to include a header file,

```cpp
#include "utilities/utilities.hpp"
```



## How to build?

### Build as usual

- Create a directory named `build` in current folder, then change directory to `build` just created

- Run CMake to generate Makefile

  ```bash
  $ cmake -G Ninja ../src -DCMAKE_BUILD_TYPE=Release
  ```

  

  If you want to generate `compile_commands.json` for editor's completion plugin, add option `-DCMAKE_EXPORT_COMPILE_COMMANDS=1`

  ```bash
  $ cmake -G Ninja ../src -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
  ```

  More about `compile_commands.json`, refer to [**clangd**](https://clangd.llvm.org/installation)

  

- Make

  ```bash
  $ cmake --build .
  ```

- Since currently I set the target name as `mymainrun`, so after compilation, a binary named `mymainrun` is created.


### Build with option settings in command line

If some cmake macro variables are defined by `option` command in `CMakeList.txt`, and then they are defined in a configure file as `#cmakedefine ENB_BOOSTTEST`, then they can be set in command line with different values.

For example, if an option named `ENB_NORMAL` is defined in `CMakeList.txt`as following,

```cmake
option(ENB_NORMAL "Testing c++ functionalities" ON)
```

it's value is set to `ON` by default, while we can set it to `OFF` when generating a Makefile, as below,

```shell
cmake -G Ninja ../src -DENB_NORMAL=OFF
```

Then this macro will be disabled after compilation, even though it is turned ON when defined.

After setting it in `CMakeList.txt`, it should be defined in a configure file (here in this project is `projectconfig.h.in`) as follows,

```cpp
#cmakedefine ENB_PYRUN_CPP
```

Thus it can be defined when compiling.




## Dynamic Linkage

In order to run the executables as expect without the error as below, some settings must be set.

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ ./build/mymainrun.exe
D:/Gitee/cpp11/build/mymainrun.exe: error while loading shared libraries: libEffectiveModernCpp.dll: cannot open shared object file: No such file or directory
```



### 1 Linking setting in `CMakeList.txt` for each module

In order to build with dynamic linkage, add `SHARED` after the library name in `CMakeLists.txt` in ***each module***, below is an example in folder (module) **EffectiveModernCpp**,

```shell
add_library(EffectiveModernCpp SHARED
    effectiveModernCpp.cpp
    chapter1_item01.cpp
    ...
    )
```



### 2 Linking setting in `CMakeList.txt` for top

Set link directory as below by command `target_link_directories`

```shell
# Create a directory list to append path for linking
set(LINK_DIRS "")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/normal/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/boosttest/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/EffectiveModernCpp/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/cppfeatures/")
MESSAGE(STATUS "[PYRAD] LINK_DIRS = ${LINK_DIRS}")

# Set executable target depending on link directoires
target_link_directories(${MY_EXEC_NAME} PUBLIC ${LINK_DIRS} )
```

Set include directory as below by command `target_include_directories`

```shell
# The name '${MY_EXEC_NAME}' MUST have been declared by add_executable() or add_library()
set(INC_DIRS "")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/normal/")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/boosttest/")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/EffectiveModernCpp/")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/cppfeatures/")
MESSAGE(STATUS "[PYRAD] INC_DIRS = ${INC_DIRS}")

# Set executable target depending on include directoires (headers)
target_include_directories(${MY_EXEC_NAME} PUBLIC ${INC_DIRS})
```



### 3 Build

Build as usual, which is mentioned in the 2nd part.



### 4 Running settings

In order to let the executable find the path for shared libraries, paths or variables should be set accordingly before running, depending on different operating system, such as Linux, Windows, MSYS2 and etc.

- **Windows/MSYS2**

Variable `PATH` should be set for those shared libraries (`.dll`) in Windows.

For example, below are the shared libraries after compilation,

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ find . -name "*.dll*"
./build/boosttest/libboosttest.dll
./build/boosttest/libboosttest.dll.a
./build/cppfeatures/libcppfeatures.dll
./build/cppfeatures/libcppfeatures.dll.a
./build/EffectiveModernCpp/libEffectiveModernCpp.dll
./build/EffectiveModernCpp/libEffectiveModernCpp.dll.a
./build/normal/libnormal.dll
./build/normal/libnormal.dll.a
./build/utilities/libutilities.dll
./build/utilities/libutilities.dll.a
```

In order to let the executable find them, checks which directories are need by the following command,

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ ldd ./build/mymainrun.exe
        ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x76fd0000)
        kernel32.dll => /c/Windows/system32/kernel32.dll (0x76d70000)
        KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefc9c0000)
        libcppfeatures.dll => not found
        libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (0x7feef5a0000)
        msvcrt.dll => /c/Windows/system32/msvcrt.dll (0x7fefdf40000)
        libwinpthread-1.dll => /mingw64/bin/libwinpthread-1.dll (0x7feeeee0000)
        libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (0x7fed5de0000)
        libEffectiveModernCpp.dll => not found
        libnormal.dll => not found
```

Here it says `libcppfeatures.dll` and `libEffectiveModernCpp.dll` are not found, thus we add the paths to let it find it, as below,

Add library paths for `libcppfeatures.dll` and `libEffectiveModernCpp.dll`,

```shell
export PATH="$PATH:/d/Gitee/cpp11/build/cppfeatures"
export PATH="$PATH:/d/Gitee/cpp11/build/EffectiveModernCpp"
export PATH="$PATH:/d/Gitee/cpp11/build/normal"
```

After adding 2 library paths, now `PATH` is,

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11
$ echo $PATH | awk -F: '{for (i = 1; i <= NF; ++i) print i, $i}'
1 /mingw64/bin
2 /usr/local/bin
3 /usr/bin
4 /bin
5 /c/Windows/System32
6 /c/Windows
7 /c/Windows/System32/Wbem
8 /c/Windows/System32/WindowsPowerShell/v1.0/
9 /usr/bin/site_perl
10 /usr/bin/vendor_perl
11 /usr/bin/core_perl
12 /d/procs/Meld
```

Now it's OK to run the executable, as they can be found now as below,

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ ldd ./build/mymainrun.exe
        ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x76fd0000)
        kernel32.dll => /c/Windows/system32/kernel32.dll (0x76d70000)
        KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefc9c0000)
        libcppfeatures.dll => /d/Gitee/cpp11/build/cppfeatures/libcppfeatures.dll (0x7fed3bf0000)
        libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (0x7feef5a0000)
        msvcrt.dll => /c/Windows/system32/msvcrt.dll (0x7fefdf40000)
        libwinpthread-1.dll => /mingw64/bin/libwinpthread-1.dll (0x7feeeee0000)
        libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (0x7fed5de0000)
        libEffectiveModernCpp.dll => /d/Gitee/cpp11/build/EffectiveModernCpp/libEffectiveModernCpp.dll (0x7fed60f0000)
        libnormal.dll => /d/Gitee/cpp11/build/normal/libnormal.dll (0x7fed3a30000)
```



- **Linux**

Variable `LD_LIBRARY_PATH` should set to include the paths for those shared libraries,

```shell
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/d/Gitee/cpp11/build/cppfeatures"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/d/Gitee/cpp11/build/EffectiveModernCpp"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/d/Gitee/cpp11/build/normal"
```

Then it's OK to run it as usual.



## How to build with Python library?

In MSYS2 environment, take folder `pyrun` as an example

### 1 Create a folder

This means it will be compiled to a shared object to link to a Python library.

In this project, we create `pyrun` folder, and link all the objects compiled to a Python library.



### 2 Setup `CMakeLists.txt` file

```cmake
# Set the name of current shared library
set(CurSharedLibName "pyrun")

set(Python3_INCLUDE_DIRS "D:/procs/python38/include")
set(Python3_LIBRARIES "D:/procs/python38/python38.dll")
set(PYTHON_EXECUTABLE "D:/procs/python38/python.exe")

# This is the CMakeLists.txt file for folder pyrun
add_library(${CurSharedLibName} SHARED pyrun.cpp)

# Remember INTERFACE means things that consumers
# require but the producer doesn't.
target_include_directories(${CurSharedLibName}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${Python3_INCLUDE_DIRS}
        )

target_link_libraries(${CurSharedLibName} ${Python3_LIBRARIES})
target_link_directories(${CurSharedLibName} PUBLIC "D:/procs/python38")
```

Just like other folder's (i.e. sub modules), set a target name, here it is `pyrun`.

Then 3 variables should be set accordingly, which are `Python3_INCLUDE_DIRS`, `Python3_LIBRARIES` and `PYTHON_EXECUTABLE`. These depend on the Python installed on current OS. In this project, we have Python3.8 installed on Windows 7.

Next we notify `cmake` that we want to create a target as shared object, whose source file is `pyrun.cpp`. (Of course we could add more source files).

Next we should notify compiler where to find the Python headers, for example, `Python.h`. Here we add `Python3_INCLUDE_DIRS` as include path by command `target_include_directories`.

Then we let `cmake` aware that which Python library to link, and where to find it, we achieve this by using `target_link_libraries` and `target_link_directories` separately.



### 3 Set `PATH` or `LD_LIBRARY_PATH` variables

After it successfully compiled, we add the paths of these shared libraries after compiled, to `PATH` (for Windows) or `LD_LIBRARAY_PATH` (for Linux), then we check it with `ldd` and see all compiled libraries **can be found**, as below,

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ ldd ./build/mymainrun.exe
        ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x76eb0000)
        kernel32.dll => /c/Windows/system32/kernel32.dll (0x76b50000)
        KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefc880000)
        libcppfeatures.dll => /d/Gitee/cpp11/build/cppfeatures/libcppfeatures.dll (0x7fed5490000)
        libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (0x7fef4480000)
        msvcrt.dll => /c/Windows/system32/msvcrt.dll (0x7fefeb20000)
        libwinpthread-1.dll => /mingw64/bin/libwinpthread-1.dll (0x7fef2360000)
        libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (0x7fed51c0000)
        libEffectiveModernCpp.dll => /d/Gitee/cpp11/build/EffectiveModernCpp/libEffectiveModernCpp.dll (0x7fed30f0000)
        libnormal.dll => /d/Gitee/cpp11/build/normal/libnormal.dll (0x7fed3040000)
        libpyrun.dll => /d/Gitee/cpp11/build/pyrun/libpyrun.dll (0x7feee630000)
```

**BUT**, if we launch it to execute, it reports `libpyrun.dll` can not be found!!

That's because we link `libpyrun.dll` to `python38.dll` in that sub folder, thus we must tell it where to find it when running as dynamic library. If we don't set it, we can see it can't find `python38.dll` as follows,

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ ./build/mymainrun.exe
D:/Gitee/cpp11/build/mymainrun.exe: error while loading shared libraries: libpyrun.dll: cannot open shared object file: No such file or directory
```

Though it reported `libpyrun.dll` can not be found (actually it does exist), it can not find `Python38.dll` indeed, as below,

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ ldd ./build/pyrun/libpyrun.dll
        ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x76eb0000)
        kernel32.dll => /c/Windows/system32/kernel32.dll (0x76b50000)
        KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefc880000)
        msvcrt.dll => /c/Windows/system32/msvcrt.dll (0x7fefeb20000)
        libpyrun.dll => /d/Gitee/cpp11/build/pyrun/libpyrun.dll (0x7feee630000)
        libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (0x7fed51c0000)
        libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (0x7fef4480000)
        libwinpthread-1.dll => /mingw64/bin/libwinpthread-1.dll (0x7fef2360000)
        python38.dll => not found
```

Thus we add `python38.dll`'s path to `PATH` (for Windows) or `LD_LIBRARAY_PATH` (for Linux) accordingly.

```shell
export PATH="$PATH:/d/procs/python38"
```

Then we check `libpyrun.dll` again, now it finds all the shared libraries it needs to link against.

```shell
Pyrad@SSEA MINGW64 /d/Gitee/cpp11 $ ldd ./build/pyrun/libpyrun.dll
ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x76eb0000)
kernel32.dll => /c/Windows/system32/kernel32.dll (0x76b50000)
KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefc880000)
msvcrt.dll => /c/Windows/system32/msvcrt.dll (0x7fefeb20000)
libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (0x7fed51c0000)
libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (0x7fef4480000)
libwinpthread-1.dll => /mingw64/bin/libwinpthread-1.dll (0x7fef2360000)
python38.dll => /d/procs/python38/python38.dll (0x7fed4c70000)
VERSION.dll => /c/Windows/system32/VERSION.dll (0x7fefc530000)
SHLWAPI.dll => /c/Windows/system32/SHLWAPI.dll (0x7fefebc0000)
GDI32.dll => /c/Windows/system32/GDI32.dll (0x7fefd560000)
USER32.dll => /c/Windows/system32/USER32.dll (0x76c70000)
LPK.dll => /c/Windows/system32/LPK.dll (0x7fefcc80000)
USP10.dll => /c/Windows/system32/USP10.dll (0x7fefd600000)
WS2_32.dll => /c/Windows/system32/WS2_32.dll (0x7fefcc30000)
RPCRT4.dll => /c/Windows/system32/RPCRT4.dll (0x7fefd8d0000)
NSI.dll => /c/Windows/system32/NSI.dll (0x7fefd5f0000)
ADVAPI32.dll => /c/Windows/system32/ADVAPI32.dll (0x7fefd310000)
sechost.dll => /c/Windows/SYSTEM32/sechost.dll (0x7fefd5d0000)
VCRUNTIME140.dll => /c/Windows/system32/VCRUNTIME140.dll (0x7fef5da0000)
api-ms-win-crt-runtime-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-runtime-l1-1-0.dll (0x7fef5d90000)
ucrtbase.DLL => /c/Windows/system32/ucrtbase.DLL (0x7fef5b00000)
api-ms-win-core-timezone-l1-1-0.dll => /c/Windows/system32/api-ms-win-core-timezone-l1-1-0.dll (0x7fef5d80000)
api-ms-win-core-file-l2-1-0.dll => /c/Windows/system32/api-ms-win-core-file-l2-1-0.dll (0x7fef5d70000)
api-ms-win-core-localization-l1-2-0.dll => /c/Windows/system32/api-ms-win-core-localization-l1-2-0.dll (0x7fef5d60000)
api-ms-win-core-synch-l1-2-0.dll => /c/Windows/system32/api-ms-win-core-synch-l1-2-0.dll (0x7fef8d80000)
api-ms-win-core-processthreads-l1-1-1.dll => /c/Windows/system32/api-ms-win-core-processthreads-l1-1-1.dll (0x7fef5d50000)
api-ms-win-core-file-l1-2-0.dll => /c/Windows/system32/api-ms-win-core-file-l1-2-0.dll (0x7fef5d40000)
api-ms-win-crt-heap-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-heap-l1-1-0.dll (0x7fef5d30000)
api-ms-win-crt-string-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-string-l1-1-0.dll (0x7fef5af0000)
api-ms-win-crt-stdio-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-stdio-l1-1-0.dll (0x7fef5ae0000)
api-ms-win-crt-convert-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-convert-l1-1-0.dll (0x7fef5ad0000)
api-ms-win-crt-math-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-math-l1-1-0.dll (0x7fef5a80000)
api-ms-win-crt-locale-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-locale-l1-1-0.dll (0x7fef5ac0000)
api-ms-win-crt-time-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-time-l1-1-0.dll (0x7fef5aa0000)
api-ms-win-crt-environment-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-environment-l1-1-0.dll (0x7fef5a90000)
api-ms-win-crt-process-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-process-l1-1-0.dll (0x7fef8420000)
api-ms-win-crt-conio-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-conio-l1-1-0.dll (0x7fef5a50000)
api-ms-win-crt-filesystem-l1-1-0.dll => /c/Windows/system32/api-ms-win-crt-filesystem-l1-1-0.dll (0x7fef5ab0000)
IMM32.DLL => /c/Windows/system32/IMM32.DLL (0x7fefd1e0000)
MSCTF.dll => /c/Windows/system32/MSCTF.dll (0x7fefd3f0000)
```



## How to call functions in dynamic library `winmingwrun.dll` after compiled?

A shared library named `winmingwrun`, defined in directory `src/pyrun/CMakeLists.txt`

Usage of this lib `winmingwrun`

- cd build directory

  ```shell
  cd ${REPO_ROOT}/build
  ```

- Check which dynamic libs `winmingwrun.dll` depends on

  ```shell
  $ ldd ./pyrun/winmingwrun.dll
             ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x777d0000)
             kernel32.dll => /c/Windows/system32/kernel32.dll (0x77570000)
             KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefd1c0000)
             msvcrt.dll => /c/Windows/system32/msvcrt.dll (0x7fefe960000)
             winmingwrun.dll => /d/Gitee/cpp11/build/pyrun/winmingwrun.dll (0x7fee0ec0000)
             libboosttest.dll => not found
             libcppfeatures.dll => not found
             libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (?)
             libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (?)
             libEffectiveModernCpp.dll => not found
             libnormal.dll => not found
  ```

- Invoke python, and use `os.add_dll_directory` to add search paths for these `*.dll` files

  Note that the path format should be in Windows style

  ```python
  import os
  os.add_dll_directory("D:\\Gitee\\cpp11\\build\\boosttest")
  os.add_dll_directory("D:\\Gitee\\cpp11\\build\\cppfeatures")
  os.add_dll_directory("D:\\Gitee\\cpp11\\build\\EffectiveModernCpp")
  os.add_dll_directory("D:\\Gitee\\cpp11\\build\\normal")
  ```

  Note, in Windows, use path format as above.

- Use ctypes to load "winmingwrun", then run the functions it loaded

  ```python
  import ctypes
  dl = ctypes.windll.LoadLibrary
  lib = dl('./pyrun/winmingwrun.dll')
  lib.run_normal()
  ```

  Here `run_normal()` is one of the functions defined in (and linked to) `winmingwrun.dll`.











