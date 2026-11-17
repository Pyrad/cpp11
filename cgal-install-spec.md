# CGAL 6.2.1 安装与项目集成说明

本文记录 `CGAL 6.2.1` 从本地 release 包安装到本机 `D:\proc\cgal6p2p1` 的完整过程、所用工具与命令、依赖关系，以及把它接入本项目 CMake 构建的全部改动与验证方法。

> **本文已按本机实测重写（2026-11-17）。** 早期版本记录的是另一台机器（前缀 `D:\procs\...`、Boost 1.79、g++ 13.2、CMake 3.27）的过程，与当前环境不符，已全部替换为本机数值；两边的差异集中列在第 2.1 节。

---

## 0. 本次工作的时间线（避免误读）

项目侧的 CGAL 集成不是一次做完的，分两步：

| 步骤 | 提交 | 内容 | 状态 |
| --- | --- | --- | --- |
| ① 配置侧集成 | `b4020aa feat(cgal): install CGAL 6.2.1 and wire it into the build` | `CPP11_ENABLE_CGAL` 选项、`find_package(CGAL 6.0 QUIET)`、`ENB_CGAL` 派生宏、`normal` 的 `PRIVATE` 链接、GMP/MPFR 运行时 DLL 拷贝、`cgal_demo()` 示例 | 已完成 |
| ② 本机安装 + 接线 | 本次 | 把 CGAL 实际装到 `D:/proc/cgal6p2p1`、`CPP11_CGAL_ROOT` 环境变量支持、`nocgal` 预设、本机 `CMakeUserPresets.json` 指向 CGAL | 已完成 |

也就是说，`b4020aa` 提交时本机**磁盘上并没有 CGAL**（`D:\proc\cgal6p2p1` 为空目录），那一次只完成了「有 CGAL 时怎么用、没有时怎么降级」的代码路径；本次才真正把它装上并跑通。

---

## 1. 安装方式结论

**CGAL 6.x 是 header-only（纯头文件）库，不需要编译出 CGAL 自己的 `.a` / `.dll` / `.so`。**

这一点可由源码包本身的目录结构确认，本机解压后的 `CGAL-6.2.1` 里：

```
$ ls -d */
LICENSES/  auxiliary/  cmake/  data/  demo/  doc_html/  examples/  include/  lib/  scripts/  share/

$ ls src                   # 不存在
ls: cannot access 'src': No such file or directory

$ ls lib                   # 只有 cmake 配置目录，没有任何库文件
cmake
```

包内文档对此有明确记载：

- `INSTALL.md` 第 4 行：*Since Version 5.0, CGAL is now header-only by default, meaning that you do not need to build and install CGAL.*
- `CHANGES.md` 的 **5.3（2021-07）** 条目：*The support for the compiled version of CGAL is dropped. Only the header-only version is supported.*

因此发布包里既没有 `src/`，`lib/` 下也没有任何库文件；5.x 早期还存在的 `CGAL_Core`、`CGAL_ImageIO` 等"需要编译的支持库"也随之消失。`cmake --build --target install` 实际只做一件事：**把 `include/`、`cmake/`、`scripts/`、`share/` 复制到安装前缀**。

真正需要"库文件"的是 CGAL 的**外部依赖 GMP / MPFR**：精确谓词核（`Exact_predicates_exact_constructions_kernel`）的数类型底层是 GMP 有理数，必须链接 `libgmp` / `libgmpxx` / `libmpfr` 三个二进制库。本机 MSYS2 已经自带这组库，因此本次安装**没有重复编译它们**，直接复用。

> 说明：没有采用 `pacman -S mingw-w64-x86_64-cgal` 的方式。原因是 pacman 安装会把 CGAL 绑死在 MSYS2 前缀下、版本随系统升级漂移，且与项目已有的 Boost 1.92 形成两套依赖树。改为独立前缀安装后，版本固定为 6.2.1，整个目录可整体复制/删除。

---

## 2. 环境与所用工具

| 工具 | 版本 | 路径 | 作用 |
| --- | --- | --- | --- |
| MSYS2（MINGW64 环境） | — | `D:\proc\msys64` | 提供编译工具链与 GMP/MPFR |
| MinGW-w64 g++ | **15.1.0** | `D:\proc\msys64\mingw64\bin\g++.exe` | CGAL 的 CMake 配置阶段会试编译检测编译器特性 |
| CMake | **4.0.2** | `D:\proc\msys64\mingw64\bin\cmake.exe` | 配置与安装 CGAL |
| Ninja | **1.12.1** | `D:\proc\msys64\mingw64\bin\ninja.exe` | 生成器 |
| Boost | **1.92.0** | `D:\proc\boost_1_92_0_msys` | CGAL 必需依赖（要求 ≥ 1.74） |
| GMP | **6.3.0** | `D:\proc\msys64\mingw64` | 精确数类型后端（随 MSYS2 安装） |
| MPFR | **4.2.2** | `D:\proc\msys64\mingw64` | GMP 的浮点扩展（随 MSYS2 安装） |
| Python | 3.13.7（MSVC ABI） | `D:\proc\Python313` | 项目 `pyrun` 模块，与 CGAL 无直接关系 |
| unzip | 6.00（Git Bash 自带） | `/usr/bin/unzip` | 解压 release 包 |

GMP / MPFR 的版本确认方式：

```bash
grep -E "define __GNU_MP_VERSION" /d/proc/msys64/mingw64/include/gmp.h
grep -E "define MPFR_VERSION_STRING" /d/proc/msys64/mingw64/include/mpfr.h
```

```
#define __GNU_MP_VERSION            6
#define __GNU_MP_VERSION_MINOR      3
#define __GNU_MP_VERSION_PATCHLEVEL 0
#define MPFR_VERSION_STRING "4.2.2"
```

MSYS2 里对应的三个 DLL（运行期需要，见第 7.4 节）：

```
D:/proc/msys64/mingw64/bin/libgmp-10.dll
D:/proc/msys64/mingw64/bin/libgmpxx-4.dll
D:/proc/msys64/mingw64/bin/libmpfr-6.dll
```

以及对应的导入库（链接期需要）：

```
D:/proc/msys64/mingw64/lib/libgmp.dll.a
D:/proc/msys64/mingw64/lib/libgmpxx.dll.a
D:/proc/msys64/mingw64/lib/libmpfr.dll.a
```

### 2.1 与旧版本文档的环境差异

| 项 | 旧文档（另一台机） | 本机实测 | 影响 |
| --- | --- | --- | --- |
| 安装/工具前缀 | `D:\procs\...` | `D:\proc\...`（**少一个 s**） | 所有命令的路径都要改，写错就会装到不存在的目录 |
| g++ | 13.2.0 | 15.1.0 | ABI 号可能变化；GMP 库名不变（Boost 的 `-mgw15` 属于 Boost 自己的命名） |
| CMake | 3.27.7 | 4.0.2 | CGAL 声明 `3.15...3.31`，4.0.2 兼容（CMake 4 只拒绝 <3.5） |
| Ninja | 1.11.1 | 1.12.1 | 无影响 |
| Boost | 1.79.0 `D:\procs\boost_1_79_0` | 1.92.0 `D:\proc\boost_1_92_0_msys`（头文件在 `include/boost-1_92`） | 满足 CGAL 的 ≥1.74 要求 |
| MPFR | 4.2.1 | 4.2.2 | 无影响 |
| 源码包目录 | `D:\Programs\CGAL` | `D:\download\CGAL` | 解压/构建命令路径不同 |

---

## 3. 源码包的选择

`D:\download\CGAL` 下有多个压缩包，用途完全不同，必须先分清：

| 压缩包 | 内容 | 本次是否使用 |
| --- | --- | --- |
| `CGAL-6.2.1.zip` | **官方 release 包**（41 MB，7815 个 zip 条目／7173 个文件）：含根 `CMakeLists.txt`、`VERSION`、`include/`、`cmake/`、`lib/cmake/`、`examples/`、`demo/` | **使用** |
| `CGAL-6.2.1-library.zip` | 与 release 包同构但更精简的"仅库"包（只有 `include/` + `cmake/` + `lib/`，无示例与文档） | 备用，未用 |
| `CGAL-6.2.1-win64-auxiliary-libraries-gmp-mpfr.zip` | **MSVC 版** GMP/MPFR：`auxiliary/gmp/bin/gmp-10.dll` + `auxiliary/gmp/lib/{gmp,gmpxx,mpfr}.lib` | **未用**（见下方说明） |
| `CGAL-6.2.1-doc_html.zip` / `CGAL-6.2.1-examples.zip` | 离线文档 / 示例集 | 未用，与安装无关 |

> **为什么不用官方附带的 GMP/MPFR 包**：它的文件名是 `gmp-10.dll` / `gmp.lib`，是 MSVC 的命名与 ABI（`.lib` 导入库）。MinGW 需要的是 `libgmp-10.dll` + `libgmp.dll.a`。两者混用会在链接期报 `undefined reference`，或在运行期因两套 CRT 崩溃。本机 MSYS2 的 GMP/MPFR 与 g++ 15.1 同源同 ABI，故直接复用。

---

## 4. 实际安装步骤

以下命令全部在 **MSYS2 MINGW64 环境**下执行。若从 Git Bash / cmd 调用，用登录 shell 包装一层，以保证 `PATH` 里是 MSYS2 工具链而不是其他 gcc：

```bash
MSYSTEM=MINGW64 /d/proc/msys64/usr/bin/bash.exe -lic '<命令>'
```

### 4.1 解压 release 包

```bash
cd /d/download/CGAL
unzip -q -o CGAL-6.2.1.zip -d /d/download/CGAL/
```

实测耗时约 **27 秒**，解开后得到 `D:\download\CGAL\CGAL-6.2.1`，确认版本号与文件数：

```bash
cat /d/download/CGAL/CGAL-6.2.1/VERSION                          # 6.2.1
find /d/download/CGAL/CGAL-6.2.1 -type f | wc -l                 # 7173
ls -d /d/download/CGAL/CGAL-6.2.1/src 2>/dev/null || echo "no src/ -- header-only"
ls /d/download/CGAL/CGAL-6.2.1/lib                               # cmake（仅此一项）
```

### 4.2 配置（生成构建树）

本次实际执行的命令：

```bash
cmake -S /d/download/CGAL/CGAL-6.2.1 \
      -B /d/download/CGAL/build-6.2.1 \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=D:/proc/cgal6p2p1 \
      -DBUILD_TESTING=OFF \
      -DCGAL_ENABLE_TESTING=OFF
```

各参数含义：

| 参数 | 作用 |
| --- | --- |
| `-S` / `-B` | 源码树 / 构建树分离（out-of-source），源码目录保持干净 |
| `-G Ninja` | 用 Ninja 而非默认生成器；配置与安装都更快 |
| `-DCMAKE_BUILD_TYPE=Release` | 决定 `CXXFLAGS`（实际为 `-O3 -DNDEBUG`） |
| `-DCMAKE_INSTALL_PREFIX=D:/proc/cgal6p2p1` | **安装前缀**，即最终交付目录 |
| `-DBUILD_TESTING=OFF` / `-DCGAL_ENABLE_TESTING=OFF` | 不生成 CGAL 自带的测试目标，安装过程只做文件复制 |

> **关于 Boost / GMP 参数**：CGAL 是 header-only，**install 阶段不需要 Boost**，所以本次命令里没有传 `BOOST_ROOT` / `CMAKE_PREFIX_PATH`。即使传了也读不到，CMake 只会给出：
>
> ```
> CMake Warning:
>   Manually-specified variables were not used by the project:
>     BOOST_ROOT
> ```
>
> 这条警告无害，但说明参数是多余的。Boost 是在**消费方** `find_package(CGAL)` 时才被 CGAL 的 `CGAL_SetupBoost.cmake` 要求的。省掉这几个参数后，本次配置日志里**没有出现任何 `CMake Warning`**。

配置成功的实际输出：

```
-- The CXX compiler identification is GNU 15.1.0
-- The C compiler identification is GNU 15.1.0
-- Build CGAL from release in directory CGAL-6.2.1
-- Using D:/proc/msys64/mingw64/bin/c++.exe compiler.
-- USING CMake version: 4.0.2
-- System: Windows
-- USING GCC_VERSION = '15.1.0'
-- Build type: Release
-- USING CXXFLAGS = ' -O3 -DNDEBUG'
-- Configuring done (4.7s)
-- Generating done (0.0s)
```

注意输出里**没有** `Found GMP` / `Found Boost` 之类的行——这正是 header-only 的表现：CGAL 自己什么都不需要链接，GMP/MPFR 的查找被推迟到消费方。

配置结果可在 `CMakeCache.txt` 中复核：

```bash
grep -iE "^(CMAKE_INSTALL_PREFIX|CMAKE_BUILD_TYPE|CMAKE_GENERATOR|CMAKE_CXX_COMPILER|BUILD_TESTING|CGAL_ENABLE_TESTING):" \
     /d/download/CGAL/build-6.2.1/CMakeCache.txt
```

```
BUILD_TESTING:UNINITIALIZED=OFF
CGAL_ENABLE_TESTING:BOOL=OFF
CMAKE_BUILD_TYPE:STRING=Release
CMAKE_CXX_COMPILER:FILEPATH=D:/proc/msys64/mingw64/bin/c++.exe
CMAKE_INSTALL_PREFIX:PATH=D:/proc/cgal6p2p1
CMAKE_GENERATOR:INTERNAL=Ninja
```

### 4.3 执行安装

```bash
cmake --build /d/download/CGAL/build-6.2.1 --target install
```

因为 CGAL 没有需要编译的源文件，这个命令不会真正"编译"任何东西，只是把文件复制到前缀。安装清单写入：

```bash
wc -l /d/download/CGAL/build-6.2.1/install_manifest.txt      # 3918
```

即共安装 **3918** 个文件。

### 4.4 校验安装结果

```bash
ls /d/proc/cgal6p2p1
# bin  include  lib  share

ls -1 /d/proc/cgal6p2p1/include/CGAL | wc -l          # 1391
ls -1 /d/proc/cgal6p2p1/lib/cmake/CGAL | head         # CGALConfig.cmake 等
grep -E "define CGAL_VERSION " /d/proc/cgal6p2p1/include/CGAL/version.h
# #define CGAL_VERSION 6.2.1
du -sh /d/proc/cgal6p2p1                              # 51M
find /d/proc/cgal6p2p1/lib -name "*.a" -o -name "*.dll"   # 无输出
```

---

## 5. 安装结果目录结构

```
D:\proc\cgal6p2p1\
├─ bin\
│  ├─ cgal_create_CMakeLists            # 脚本：为示例生成 CMakeLists.txt
│  └─ cgal_create_cmake_script
├─ include\
│  ├─ CGAL\                             # 1391 个条目，全部头文件
│  │  ├─ Exact_predicates_exact_constructions_kernel.h
│  │  ├─ Gmpq.h
│  │  ├─ version.h                      # CGAL_VERSION 6.2.1
│  │  └─ ...
│  └─ (CGAL 内部有些目录与 CGAL/ 平级)
├─ lib\
│  └─ cmake\CGAL\                       # ★ 消费方 find_package 的入口
│     ├─ CGALConfig.cmake
│     ├─ CGALConfigVersion.cmake
│     ├─ CGAL_SetupCGALDependencies.cmake
│     ├─ CGAL_SetupBoost.cmake
│     ├─ CGAL_SetupGMP.cmake
│     └─ ...
└─ share\
   ├─ doc\
   └─ man\
```

关键信息：

- 头文件根目录：`D:\proc\cgal6p2p1\include`（代码里写 `#include <CGAL/...>`）
- CMake 配置目录：`D:\proc\cgal6p2p1\lib\cmake\CGAL`（`CGAL_DIR` 指向这里）
- **`lib\` 下没有任何 `.a` / `.dll`**——这正是 header-only 的证据

---

## 6. 依赖传递关系（接入前必须知道的三条硬约束）

CGAL 的 `CGAL::CGAL` 是一个 INTERFACE（纯头文件）imported target，它的使用要求全部由 `lib/cmake/CGAL/CGAL_SetupCGALDependencies.cmake` 施加：

| 约束 | 出处（安装后文件） | 对项目的影响 |
| --- | --- | --- |
| **强制 C++17** | `CGAL_SetupCGALDependencies.cmake:124`：`target_compile_features(${target} INTERFACE cxx_std_17)` | 任何链接 `CGAL::CGAL` 的目标都会被抬到 `-std=c++17`，且该要求**会沿链接关系传播** |
| Boost ≥ 1.74 | `CGAL_SetupBoost.cmake:17`：`find_package(Boost 1.74 REQUIRED)` | 本机 1.92.0 满足 |
| GMP / GMPXX / MPFR | `CGAL_SetupGMP.cmake`：`find_package(GMP QUIET)` + `GMPXX` + `MPFR` | 精确核必须链接这三者；MSYS2 前缀下已就绪 |

第一条决定了本项目的接入方式：**必须用 `PRIVATE` 链接**。原因见 7.3。

---

## 7. 项目的 CMake 集成

### 7.1 `CMakeLists.txt`（顶层）：可关闭的可选依赖

```cmake
option(CPP11_ENABLE_CGAL
       "Build the CGAL example in the normal module (needs CGAL 6.x + GMP/MPFR)" ON)
if(CPP11_ENABLE_CGAL)
    # Same convention as CPP11_BOOST_ROOT / CPP11_PYTHON_ROOT above: a machine
    # whose CGAL lives outside CMAKE_PREFIX_PATH can export CPP11_CGAL_ROOT
    # (the install prefix, e.g. D:/proc/cgal6p2p1) instead of hard-coding the
    # path here or passing -DCGAL_DIR on every configure.
    if(DEFINED ENV{CPP11_CGAL_ROOT} AND NOT DEFINED CGAL_DIR)
        set(CGAL_DIR "$ENV{CPP11_CGAL_ROOT}/lib/cmake/CGAL")
    endif()
    find_package(CGAL 6.0 QUIET)
    if(NOT CGAL_FOUND)
        message(WARNING
            "CGAL not found: the CGAL example will be compiled out. "
            "Pass -DCGAL_DIR=<prefix>/lib/cmake/CGAL, or add the install "
            "prefix to CMAKE_PREFIX_PATH (or set CPP11_CGAL_ROOT to it), or "
            "configure with -DCPP11_ENABLE_CGAL=OFF to silence this warning.")
    endif()
endif()
```

状态汇总行：

```cmake
if(CGAL_FOUND)
    message(STATUS "[cpp11] CGAL           : ${CGAL_VERSION}  (${CGAL_DIR})")
else()
    message(STATUS "[cpp11] CGAL           : not used")
endif()
```

要点：

- 用 `QUIET` + `WARNING` 而不是 `REQUIRED`，**找不到 CGAL 不中断构建**。这样没有 CGAL 的机器（或 CI）仍能编译整个项目，`CPP11_CXX_STANDARD=11/14` 的实验也保持可用。
- `CPP11_CGAL_ROOT` 与已有的 `CPP11_BOOST_ROOT` / `CPP11_PYTHON_ROOT` 保持同一套约定：**机器相关的路径不写进仓库**，由环境变量或 preset 提供。它只在 `CGAL_DIR` 未被显式指定时生效，`-DCGAL_DIR=...` 优先级更高。
- 注意 `CPP11_CGAL_ROOT` 期望的是**安装前缀**（`D:/proc/cgal6p2p1`），不是 `lib/cmake/CGAL`；后半段由 CMake 自己拼上去。

### 7.2 `src/CMakeLists.txt`：把"选项"变成"宏"

CGAL 有两个条件（用户是否开启 + 是否真的找到），用一个派生变量收敛，再写进 `projectconfig.h`：

```cmake
if(CPP11_ENABLE_CGAL AND CGAL_FOUND)
    set(ENB_CGAL ON)
else()
    set(ENB_CGAL OFF)
endif()
```

### 7.3 `src/normal/CMakeLists.txt`：`PRIVATE` 链接 + C++17 隔离

```cmake
if(ENB_CGAL)
    target_link_libraries(normal PRIVATE CGAL::CGAL)
endif()
```

**为什么必须是 `PRIVATE`**：`CGAL::CGAL` 的 `cxx_std_17` 是 INTERFACE 编译特性，写成 `PUBLIC` 会传播到 `mymainrun`、`cpp11runtest`、`winmingwrun`，整个项目被强行拉到 C++17，`CPP11_CXX_STANDARD=11/14` 的意义就消失了。`normal.hpp` 里没有 CGAL 头文件，所以对外不需要暴露。

**为什么没有显式写 `CXX_STANDARD 17`**：`CGAL::CGAL` 的 INTERFACE 编译特性会自动把 `normal` 这个 target 抬到 C++17，CMake 会为它单独加 `-std=c++17`，无需手工覆盖。实测验证（见 9.2）两者等价而前者更少一处冗余。

### 7.4 `src/CMakeLists.txt`：运行时 DLL 自包含（第二段）

```cmake
if(MINGW AND CPP11_COPY_RUNTIME_DLLS)
    get_filename_component(_cpp11_mingw_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)

    set(_cpp11_toolchain_dlls "")
    foreach(_dll IN ITEMS
            libgcc_s_seh-1.dll libgcc_s_dw2-1.dll
            libstdc++-6.dll libwinpthread-1.dll)
        if(EXISTS "${_cpp11_mingw_bin}/${_dll}")
            list(APPEND _cpp11_toolchain_dlls "${_cpp11_mingw_bin}/${_dll}")
        endif()
    endforeach()

    if(CGAL_FOUND)
        file(GLOB _cpp11_bignum_dlls
                "${_cpp11_mingw_bin}/libgmp-*.dll"
                "${_cpp11_mingw_bin}/libmpfr-*.dll"
                "${_cpp11_mingw_bin}/libgmpxx-*.dll")
        list(APPEND _cpp11_toolchain_dlls ${_cpp11_bignum_dlls})
    endif()

    if(_cpp11_toolchain_dlls)
        add_custom_command(TARGET mymainrun POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${_cpp11_toolchain_dlls} $<TARGET_FILE_DIR:mymainrun>
            COMMENT "[cpp11] Copying MinGW/CGAL runtime DLLs next to mymainrun")
    endif()
endif()
```

这一段解决的是一个**运行时才暴露**的问题，细节见第 10 节。要点：

- `$<TARGET_RUNTIME_DLLS>` 只识别 imported **target**，看不到编译器自带的 `libgcc/libstdc++/libwinpthread`，也看不到 CGAL 以"裸库路径"传进来的 GMP/MPFR。所以必须手工补齐。
- GMP 系用 `glob` 匹配（`libgmp-*.dll`），MSYS2 升 ABI 号（`-10` → `-12`）时不用改 CMake。

### 7.5 `src/projectconfig.h.in`

```c
#cmakedefine ENB_CGAL
```

### 7.6 `src/normal/normal.hpp` / `normal.cpp`

头文件只加一行无条件声明，使调用方无需关心 CGAL 是否存在：

```cpp
void cgal_demo();
```

实现放在 `normal.cpp`，用 `#ifdef ENB_CGAL` 包住 CGAL 相关代码：

```cpp
#include "projectconfig.h"

#ifdef ENB_CGAL
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Gmpq.h>
#include <CGAL/number_utils.h>
#include <CGAL/squared_distance_2.h>

// CGAL_VERSION 是预处理数字（6.2.1），不是合法 C++ 表达式，只能字符串化
#define CPP11_STR_IMPL(x) #x
#define CPP11_STR(x) CPP11_STR_IMPL(x)
#endif
```

并在 `normal_test_all()` 开头调用：

```cpp
cgal_demo();
```

### 7.7 预设：`CMakePresets.json`（入库）+ `CMakeUserPresets.json`（本机）

入库的 `CMakePresets.json` 新增一个**降级预设**，用于随时验证「没有 CGAL 也能编译」这条路径：

```json
{ "name": "nocgal", "inherits": "base", "displayName": "CGAL disabled (fallback path)",
  "cacheVariables": { "CPP11_ENABLE_CGAL": "OFF" } }
```

以及对应的 `buildPresets` / `testPresets` 条目，于是 `cmake --list-presets` 能看到：

```
"local", "msys2", "linux", "debug", "cxx11", "cxx17", "cxx20", "nocgal", "asan"
```

本机路径只写在 **gitignored** 的 `CMakeUserPresets.json` 里：

```json
{
  "name": "local",
  "inherits": "msys2",
  "binaryDir": "${sourceDir}/build",
  "cacheVariables": {
    "CMAKE_PREFIX_PATH": "D:/proc/boost_1_92_0_msys;D:/proc/cgal6p2p1",
    "Python3_ROOT_DIR": "D:/proc/Python313",
    "CGAL_DIR": "D:/proc/cgal6p2p1/lib/cmake/CGAL"
  }
}
```

两处都写：`CMAKE_PREFIX_PATH` 里加前缀对 `find_package` 已经足够，`CGAL_DIR` 属冗余但显式，便于排查时一眼看出命中的是哪份 CGAL。

---

## 8. 示例内容与运行结果

`cgal_demo()` 刻意选了 `Exact_predicates_exact_constructions_kernel`（EPECK）：它的数类型是 `CGAL::Lazy_exact_nt<CGAL::Exact_rational>`，而 `Exact_rational` 在本机解析为 GMP 的 `Gmpq`（`CGAL/Exact_rational.h` 的注释写明其定义取决于 GMP/LEDA 是否可用；本项目编译参数里就有 `-DCGAL_USE_GMPXX=1`）。这个组合**并非纯头文件**，会真正调用 `libgmp` / `libmpfr`。因此这一个函数同时验证了三件事：头文件找得到（编译）、库链接得上（链接）、DLL 加载得了（运行）。

```cpp
void cgal_demo() {
    std::cout << "===== CGAL demo =====" << std::endl;
#ifndef ENB_CGAL
    std::cout << "CGAL is not configured (ENB_CGAL undefined); skipping the demo" << std::endl;
#else
    using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
    using Point = Kernel::Point_2;
    using Segment = Kernel::Segment_2;
    using Triangle = Kernel::Triangle_2;
    using FT = Kernel::FT;

    std::cout << "CGAL version: " << CPP11_STR(CGAL_VERSION) << std::endl;

    const Point a(0, 0), b(4, 0), c(4, 3);
    const Triangle tri(a, b, c);

    const FT area = tri.area();                                   // 精确构造
    std::cout << "triangle area      = " << CGAL::to_double(area) << std::endl;

    const Point in(3, 1), out(1, 1);                              // 精确谓词
    std::cout << "has_on_bounded_side(3,1) = "
              << (tri.has_on_bounded_side(in) ? "true" : "false") << std::endl;
    std::cout << "has_on_bounded_side(1,1) = "
              << (tri.has_on_bounded_side(out) ? "true" : "false") << std::endl;

    const Segment s(Point(0, 1), Point(6, 1));                    // 精确距离（无 sqrt）
    const FT d2 = CGAL::squared_distance(Point(2, 5), s);
    std::cout << "squared distance   = " << CGAL::to_double(d2) << std::endl;

    const CGAL::Gmpq q = CGAL::Gmpq(1, 3) + CGAL::Gmpq(1, 6);      // GMP 有理数运算
    std::cout << "1/3 + 1/6          = " << q.numerator() << "/" << q.denominator()
              << " = " << q.to_double() << std::endl;

    std::cout << "area == 6 exactly  = " << ((area == FT(6)) ? "true" : "false")
              << std::endl;                                        // 无容差精确比较
#endif
    std::cout << "===== CGAL demo end =====" << std::endl;
}
```

运行 `build/bin/mymainrun.exe` 的实际输出：

```
===== CGAL demo =====
CGAL version: 6.2.1
triangle area      = 6
has_on_bounded_side(3,1) = true
has_on_bounded_side(1,1) = false
squared distance   = 16
1/3 + 1/6          = 1/2 = 0.5
area == 6 exactly  = true
===== CGAL demo end =====
```

注意 `1/3 + 1/6 = 1/2`：如果用 `double` 计算会是 `0.49999999999999994`，这里的 `1/2` 是 GMP 分数的分子分母直接打印出来的，是"确实链接到了 GMP"的直观证据。

---

## 9. 验证矩阵（本次实际执行结果）

### 9.1 构建与测试

| 场景 | 命令 | 结果 |
| --- | --- | --- |
| 配置 | `cmake --preset local --fresh` | 退出码 0；`[cpp11] CGAL : 6.2.1 (D:/proc/cgal6p2p1/lib/cmake/CGAL)` |
| 编译 | `cmake --build build --clean-first -j 4` | **54/54，0 error，143 warning**（与未接 CGAL 时完全一致） |
| 冒烟测试 | `ctest --preset local` | `1/1 Test #1: smoke ... Passed 0.03 sec`，100% passed |
| 无 MSYS2 PATH 直接运行 | `PATH="/usr/bin:/c/Windows/System32:/c/Windows" ./build/bin/mymainrun.exe` | 退出码 0，**813 行**输出，`retcode=0` |

143 条警告全部是既有代码与 Boost 模板的格式串类警告，**没有一条来自 CGAL 头文件**。

### 9.2 C++ 标准隔离验证

查 `build/compile_commands.json`：

| 源文件 | `-std` | CGAL 相关参数 |
| --- | --- | --- |
| `src/normal/normal.cpp` | `-std=c++17` | `-DCGAL_USE_GMPXX=1`，`-isystem D:/proc/cgal6p2p1/include` |
| `src/main.cpp` | `-std=c++14` | 无 |
| `src/pyrun/cpp11runtest.cpp` | `-std=c++14` | 无 |

即：CGAL 的 C++17 要求只作用于 `normal` 这一个 target，项目其余部分仍是 `CPP11_CXX_STANDARD=14`。若把 7.3 的 `PRIVATE` 改成 `PUBLIC`，上表后两行会变成 `-std=c++17`。

### 9.3 链接验证

链接行里的 GMP 系（来自 `build/build.ninja`）：

```
D:/proc/msys64/mingw64/lib/libgmp.dll.a
D:/proc/msys64/mingw64/lib/libgmpxx.dll.a
D:/proc/msys64/mingw64/lib/libmpfr.dll.a
```

产物依赖（`objdump -p build/bin/libnormal.dll`；`ldd` 对 `.pyd` 与部分 DLL 不可靠，用 `objdump` 更稳）：

```
libgcc_s_seh-1.dll
KERNEL32.dll
msvcrt.dll
libwinpthread-1.dll
libstdc++-6.dll
libgmp-10.dll        ← CGAL 精确核
libmpfr-6.dll        ← CGAL 精确核
```

`build/bin/` 下的运行时 DLL 清单（POST_BUILD 拷贝的结果）：

```
libEffectiveModernCpp.dll  libboosttest.dll   libcppfeatures.dll
libgcc_s_seh-1.dll         libgmp-10.dll      libgmpxx-4.dll
libmpfr-6.dll              libnormal.dll      libpyrun.dll
libstdc++-6.dll            libutilities.dll   libwinmingwrun.dll
libwinpthread-1.dll        python313.dll
```

### 9.4 降级路径验证

| 场景 | 命令 | 结果 |
| --- | --- | --- |
| 显式关闭 | `-DCPP11_ENABLE_CGAL=OFF` | 配置退出码 0，状态行 `CGAL : not used`，`projectconfig.h` 为 `/* #undef ENB_CGAL */` |
| CGAL 路径不存在 | `-DCGAL_DIR=D:/nonexistent/lib/cmake/CGAL` | 配置仍成功（rc=0），仅打印 CMake Warning（含三种修复建议），`ENB_CGAL` 未定义 |
| 仅用环境变量 | `export CPP11_CGAL_ROOT=D:/proc/cgal6p2p1`，不传 `CGAL_DIR` | 配置 rc=0，同样命中 `CGAL : 6.2.1` |
| 预设降级 | `cmake --preset nocgal` | 预设可用，`CPP11_ENABLE_CGAL=OFF` 已生效 |

---

## 10. 注意事项（踩过的坑）

1. **`ImportError: DLL load failed while importing cpp11runtest`（错误码 126）**
   典型症状是程序编译通过但一运行就报这个。根因：`libnormal.dll` 新增了 `libgmp-10.dll` / `libmpfr-6.dll` 依赖，而 **Python 3.8+ 在加载扩展模块时不再搜索 `PATH`**，`$<TARGET_RUNTIME_DLLS>` 又只能收集 imported target 的 DLL。解决办法就是 7.4 那段 POST_BUILD 拷贝。本机这次因为 7.4 已经就位，**没有复现**该错误。

2. **`CGAL_VERSION` 不能直接输出**
   `CGAL_VERSION` 是 `6.2.1` 这种预处理数字，`std::cout << CGAL_VERSION` 会因为 `6.2.1` 不是合法字面量而编译失败。必须字符串化（`CPP11_STR(CGAL_VERSION)`）。

3. **不要用 `PUBLIC` 链接 `CGAL::CGAL`**
   会把 C++17 泄漏到整个项目，破坏 `CPP11_CXX_STANDARD` 的可配置性。见 7.3 / 9.2。

4. **GMP/MPFR 必须与编译器同 ABI**
   官方 `CGAL-6.2.1-win64-auxiliary-libraries-gmp-mpfr.zip` 是 MSVC 版（`gmp.lib` + `gmp-10.dll`），MinGW 用会链接失败。用 MSYS2 的 `libgmp.dll.a` + `libgmp-10.dll`。

5. **CGAL 6.x 要求 C++17，项目默认 C++14**
   这不是配置错误，而是硬性约束。集成方式是"局部抬标准"，不是"全项目抬标准"。

6. **`cmake --build --target install` 不产生库文件**
   只复制文件。如果 `lib/` 下出现 `.a`，说明用的不是 6.x 或误用了旧包的 `src/`。

7. **CGAL 的 CMake 配置阶段需要能运行编译器**
   若在 Git Bash 里直接调 `cmake.exe` 而 `PATH` 中没有 MSYS2 的 `mingw64/bin`，试编译会因缺少 `libwinpthread-1.dll` 之类而失败，报 `CMAKE_CXX_COMPILER not set` 类似的假错误。务必先进入 MINGW64 环境（或把 `D:/proc/msys64/mingw64/bin` 放到 `PATH` 最前）。

8. **安装前缀不要放在源码目录内**
   本次前缀 `D:/proc/cgal6p2p1` 与源码 `D:/download/CGAL/CGAL-6.2.1` 分离。如果前缀落在源码树里，下次配置会扫到自己的安装产物，引发难以排查的重复包含。

9. **验证"零 PATH 运行"时别用 `timeout` 包装**
   把 `PATH` 清成只剩 `C:\Windows\...` 之后，`timeout` 不再是 coreutils 的那个，而是落到 `C:\Windows\System32\timeout.exe`，于是输出

   ```
   系统找不到指定的程序。
   键入 TIMEOUT /? 以获取使用方式。
   ```

   并返回 1——**看起来像程序崩溃，其实只是包装命令错了**。正确做法是把 `/usr/bin` 留在 `PATH` 里（它不含 `mingw64/bin`、CGAL 与 Python 目录，仍然能验证 DLL 自包含）：

   ```bash
   PATH="/usr/bin:/c/Windows/System32:/c/Windows" timeout 180 ./mymainrun.exe
   ```

10. **CMake 4.x 与 CGAL 的版本声明**
    CGAL 6.2.1 声明 `cmake_minimum_required(VERSION 3.15...3.31)`，CMake 4.0.2 可以正常配置。CMake 4 只拒绝声明低于 3.5 的项目，所以这条不是坑，但换成更老的第三方包时要留意。

11. **改完 preset 必须 `--fresh`**
    `CMAKE_PREFIX_PATH` 里的顺序决定命中哪份 Boost/CGAL。本机是 `D:/proc/boost_1_92_0_msys;D:/proc/cgal6p2p1`，靠前的前缀优先。改完 preset 后若只做增量配置，`Boost_DIR` / `CGAL_DIR` 会沿用缓存里的旧值，表现为"改了没生效"。

---

## 11. 项目侧使用方式

`CMakeUserPresets.json`（`.gitignore` 已忽略，不入库）中的本机预设已包含 CGAL：

```json
"CMAKE_PREFIX_PATH": "D:/proc/boost_1_92_0_msys;D:/proc/cgal6p2p1",
"Python3_ROOT_DIR": "D:/proc/Python313",
"CGAL_DIR": "D:/proc/cgal6p2p1/lib/cmake/CGAL"
```

日常：

```bash
cmake --preset local && cmake --build --preset local && ctest --preset local
./build/bin/mymainrun.exe
```

若不用预设，则必须显式告知 CGAL 位置，三选一：

```bash
# 方式 A：指向 config 目录（优先级最高）
cmake -S . -B build -G Ninja -DCGAL_DIR=D:/proc/cgal6p2p1/lib/cmake/CGAL

# 方式 B：把前缀加入搜索路径（find_package 会自己找 lib/cmake/CGAL）
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=D:/proc/cgal6p2p1

# 方式 C：环境变量（给的是安装前缀，不是 lib/cmake/CGAL）
export CPP11_CGAL_ROOT=D:/proc/cgal6p2p1
cmake -S . -B build -G Ninja
```

新机器上接入的最小可用片段：

```cmake
find_package(CGAL 6.0 REQUIRED)
target_link_libraries(<your_target> PRIVATE CGAL::CGAL)
```

---

## 12. 故障排查速查

| 现象 | 原因 | 处理 |
| --- | --- | --- |
| 配置输出 `CGAL : not used`，但确实装了 CGAL | 没告诉 CMake 去哪找 | 加 `-DCGAL_DIR=<prefix>/lib/cmake/CGAL`、把前缀放进 `CMAKE_PREFIX_PATH`，或设 `CPP11_CGAL_ROOT` |
| `Could not find a package configuration file provided by "CGAL"` | 同上；或指向了 `.../lib/cmake` 而不是 `.../lib/cmake/CGAL` | `CGAL_DIR` 必须精确到 `CGAL` 子目录；`CPP11_CGAL_ROOT` 则只到前缀 |
| `CMake Error: ... requires the C++ standard 17` / 大量 C++17 语法报错 | 用了 C++14 编译含 CGAL 的源文件 | 确认 `target_link_libraries(<t> PRIVATE CGAL::CGAL)`，让其 INTERFACE 特性生效 |
| `undefined reference to __gmpq_...` / `mpfr_...` | GMP/MPFR 未上链接行（常见于手工 `-l` 或用了 MSVC 版 `.lib`） | 交给 `CGAL::CGAL` 传递；确认 `find_package(GMP)` 命中 MSYS2 前缀 |
| 运行期 `ImportError ... 126`，或双击 exe 无反应 | `libgmp-10.dll` / `libstdc++-6.dll` 等不在 exe/pyd 同级目录，且 Python 3.8+ 不搜 PATH | 开启 `CPP11_COPY_RUNTIME_DLLS`，走 7.4 的 POST_BUILD 拷贝；或把 `D:/proc/msys64/mingw64/bin` 加入 PATH |
| `std::cout << CGAL_VERSION` 编译报错 | `6.2.1` 不是合法 C++ 字面量 | 用 `#define CPP11_STR(x)` 两级宏字符串化 |
| 链接期报 `file format not recognized` 指向 `gmp.lib` | 用了官方 MSVC 版 GMP/MPFR | 换 MSYS2 的 `libgmp.dll.a` |
| 配置阶段 `CMAKE_CXX_COMPILER not set` / 试编译失败 | 未在 MSYS2 环境，g++ 无法运行 | `MSYSTEM=MINGW64 /d/proc/msys64/usr/bin/bash.exe -lic '...'`，或修正 PATH |
| 改了 preset 但仍指向旧路径 | CMakeCache 缓存了 `CGAL_DIR` / `Boost_DIR` | `cmake --preset local --fresh` |
| 清空 PATH 后运行报「键入 TIMEOUT /?」 | `timeout` 落到了 Windows 自带版本 | 保留 `/usr/bin` 在 PATH 里，见 10.9 |
