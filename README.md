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



## How to build?

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
        ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x77000000)
        kernel32.dll => /c/Windows/system32/kernel32.dll (0x76ca0000)
        KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefcba0000)
        libcppfeatures.dll => not found
        libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (?)
        msvcrt.dll => /c/Windows/system32/msvcrt.dll (?)
        libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (?)
        libEffectiveModernCpp.dll => not found
```

Here it says `libcppfeatures.dll` and `libEffectiveModernCpp.dll` are not found, thus we add the paths to let it find it, as below,

Add library paths for `libcppfeatures.dll` and `libEffectiveModernCpp.dll`,

```shell
export PATH="$PATH:/d/Gitee/cpp11/build/cppfeatures"
export PATH="$PATH:/d/Gitee/cpp11/build/EffectiveModernCpp"
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
Pyrad@SSEA MINGW64 /d/Gitee/cpp11
$ ldd ./build/mymainrun.exe
        ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x77000000)
        kernel32.dll => /c/Windows/system32/kernel32.dll (0x76ca0000)
        KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefcba0000)
        libcppfeatures.dll => /d/Gitee/cpp11/build/cppfeatures/libcppfeatures.dll (0x7fef3c50000)
        libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (0x7fef3c00000)
        msvcrt.dll => /c/Windows/system32/msvcrt.dll (0x7fefe4e0000)
        libwinpthread-1.dll => /mingw64/bin/libwinpthread-1.dll (0x7fef3be0000)
        libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (0x7fed0a30000)
        libEffectiveModernCpp.dll => /d/Gitee/cpp11/build/EffectiveModernCpp/libEffectiveModernCpp.dll (0x7fecdbb0000)
```



- **Linux**

Variable `LD_LIBRARY_PATH` should set to include the paths for those shared libraries,

```shell
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/d/Gitee/cpp11/build/cppfeatures"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/d/Gitee/cpp11/build/EffectiveModernCpp"
```

Then it's OK to run it as usual.
