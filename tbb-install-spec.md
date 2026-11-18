# oneTBB 2023.2.0 安装与项目集成说明

本文记录 `oneTBB 2023.2.0` 从本地源码包出发、在 MSYS2 MINGW64 环境下编译并安装到 `D:\procs\oneTBB2023p2p0` 的完整过程，以及把它接入本项目 CMake 构建的全部改动与验证方法。

---

## 1. 安装方式结论

**oneTBB 不是 header-only 库，必须编译出动态库；而且不能用官方 Windows 预编译包，因为那是 MSVC 二进制。**

### 1.1 为什么必须编译

`include/tbb/*.h` 只是转发头文件，真正的实现在 `include/oneapi/tbb/`，并且最终要调用 `tbb12.dll` 里的任务调度器：

```
$ head -20 include/tbb/parallel_for.h | tail -1
#include "../oneapi/tbb/parallel_for.h"
```

也就是说，`#include <tbb/parallel_for.h>` 能让代码编译通过，但 `tbb::parallel_for` 的调度、线程池、任务窃取全在动态库里，**不链接 `tbb12` 一定在链接期或运行期失败**。

本项目三个库的"安装"含义差别很大，放在一起看更清楚：

| 库 | 是否需要编译 | 安装阶段产物 | 消费方需要链接 |
| --- | --- | --- | --- |
| pybind11 | 否（纯头文件） | 头文件 + CMake/pkg-config | 只需 `pybind11::module`（header-only） |
| CGAL 6.2.1 | 否（纯头文件，GMP/MPFR 是外部依赖） | 头文件 + `lib/cmake/CGAL` | `CGAL::CGAL`（只有头文件与 GMP/MPFR） |
| **oneTBB 2023.2.0** | **是** | **`.dll` + `.dll.a` + 头文件 + `lib/cmake/TBB`** | **`TBB::tbb`（真动态库）** |

### 1.2 为什么不用官方 Windows 包

`oneapi-tbb-2023.2.0-win.zip` 解压后是 MSVC 布局：

```
oneapi-tbb-2023.2.0/redist/ia32/vc14/tbb12.dll
oneapi-tbb-2023.2.0/redist/intel64/vc14/tbb12.lib      ← MSVC 导入库
oneapi-tbb-2023.2.0/include/...
```

两个不兼容点：

1. **导入库格式**：MinGW 需要 `libtbb12.dll.a`，MSVC 的 `.lib` 里符号是 MSVC 名字修饰（`?parallel_for@...@@YAXXZ`），MinGW 的 `-ltbb12` 找不到 GCC 修饰的符号。
2. **DLL 的 C++ ABI**：`tbb12.dll` 内部导出的是 MSVC ABI 的 C++ 符号（RTTI、异常、`std::` 类型布局），与 libstdc++ 不通用。

所以只能源码编译。这也解释了为什么装完的产物叫 `libtbb12.dll`（MinGW 加 `lib` 前缀），而不是官方的 `tbb12.dll`。

---

## 2. 环境与所用工具

| 工具 | 版本 | 路径 | 作用 |
| --- | --- | --- | --- |
| MSYS2（MINGW64 环境） | — | `D:\procs\msys64` | 提供编译工具链 |
| MinGW-w64 g++ | 13.2.0 | `D:\procs\msys64\mingw64\bin\g++.exe` | 编译 oneTBB |
| CMake | 3.27.7 | `D:\procs\msys64\mingw64\bin\cmake.exe` | 配置、构建、安装 |
| Ninja | 1.11.1 | `D:\procs\msys64\mingw64\bin\ninja.exe` | 生成器 |
| tar | — | Git Bash 自带 | 解压 `.tar.gz`（保留符号链接与权限） |
| 逻辑处理器 | 8 | — | 影响 `default concurrency` 与 `-j` 的取值 |

oneTBB 是纯 C++ 项目，**不需要 Boost、不需要 Python、不需要 GMP**，与 CGAL 的依赖树完全独立，两者可以共存于同一个目标里。

---

## 3. 源码包的选择

`D:\Programs\oneTBB` 下有 8 个文件，只有一个是本次要用的：

| 文件 | 内容 | 本次是否使用 |
| --- | --- | --- |
| `oneTBB-2023.2.0-src.tar.gz` | **源码包（Unix 换行/符号链接友好）**，解压出 `oneTBB-2023.2.0/`，1975 个条目 | **使用** |
| `oneTBB-2023.2.0-src.zip` | 同一份源码的 zip 版 | 备用（等价，未用） |
| `oneapi-tbb-2023.2.0-win.zip` | **MSVC 预编译包**（`redist/vc14/tbb12.lib` + `tbb12.dll`） | **未用**，见 1.2 |
| `oneapi-tbb-2023.2.0-lin.tgz` / `-mac.tgz` | Linux / macOS 预编译包 | 未用（平台不符） |
| `oss-docs.zip` | 离线 HTML 文档 | 未用 |
| `tcm-1.6.0-win.zip` / `-lin.tgz` | Thread Composability Manager 独立包 | 未用（依赖 hwloc，见 6.2） |

> 说明：zip 与 tar.gz 内容一致；选 `.tar.gz` 只是因为 Git Bash 下 `tar xzf` 比 `unzip` 快，且不会丢失文件权限。

---

## 4. 实际安装步骤

以下命令全部在 **MSYS2 MINGW64 环境**下执行。从 Git Bash 调用时用登录 shell 包装一层，保证 `PATH` 里是 MSYS2 的 g++：

```bash
MSYSTEM=MINGW64 /d/procs/msys64/usr/bin/bash.exe -lic '<命令>'
```

### 4.1 解压源码包

```bash
cd /d/Programs/oneTBB
tar xzf oneTBB-2023.2.0-src.tar.gz
```

得到 `D:\Programs\oneTBB\oneTBB-2023.2.0`，确认版本：

```bash
grep -E "define TBB_VERSION_(MAJOR|MINOR|PATCH)" \
     /d/Programs/oneTBB/oneTBB-2023.2.0/include/oneapi/tbb/version.h
# #define TBB_VERSION_MAJOR 2023
# #define TBB_VERSION_MINOR 2
# #define TBB_VERSION_PATCH 0
```

### 4.2 配置（生成构建树）

```bash
cmake -S /d/Programs/oneTBB/oneTBB-2023.2.0 \
      -B /d/Programs/oneTBB/build-2023.2.0 \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=D:/procs/oneTBB2023p2p0 \
      -DTBB_TEST=OFF \
      -DTBB_EXAMPLES=OFF \
      -DTBB_VERIFY_DEPENDENCY_SIGNATURE=OFF \
      -DTCM_BUILD=OFF \
      -DTCM_TEST=OFF \
      -DCMAKE_CXX_FLAGS="-DLOAD_LIBRARY_SAFE_CURRENT_DIRS=0x00002000"
```

各参数的作用，以及**哪些是必需的、哪些只是开关**：

| 参数 | 作用 | 是否 MinGW 必需 |
| --- | --- | --- |
| `-S` / `-B` | 源码树 / 构建树分离 | 常规做法 |
| `-G Ninja` | 用 Ninja，构建快 | 否 |
| `-DCMAKE_BUILD_TYPE=Release` | 单配置生成器下决定 `-O3 -DNDEBUG` | 否（但强烈建议） |
| `-DCMAKE_INSTALL_PREFIX=...` | 安装前缀；默认会是 `c:/Program Files/TBB` | 是（必须改） |
| `-DTBB_TEST=OFF` | 不编译 2000+ 个测试目标（默认 ON） | 是（否则构建时间翻十倍） |
| `-DTBB_EXAMPLES=OFF` | 不编译示例 | 否（默认已 OFF） |
| `-DTBB_VERIFY_DEPENDENCY_SIGNATURE=OFF` | 关闭运行期依赖签名校验 | 建议（见 6.3） |
| `-DTCM_BUILD=OFF` / `-DTCM_TEST=OFF` | 关掉 Thread Composability Manager | **是**（本机无 hwloc，见 6.2） |
| `-DCMAKE_CXX_FLAGS="-DLOAD_LIBRARY_SAFE_CURRENT_DIRS=0x00002000"` | 补一个 MinGW 头文件里缺失的常量 | **是**（见 6.1） |

配置成功的实际输出（节选）：

```
-- The CXX compiler identification is GNU 13.2.0
-- Dependency signature verification during dynamic linking in run-time is DISABLED
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Checking for one of the modules 'hwloc'
-- IPO enabled
-- HWLOC target HWLOC::hwloc_static doesn't exist. The tbbbind target cannot be created
-- HWLOC target HWLOC::hwloc_2 doesn't exist. The tbbbind_2_0 target cannot be created
-- HWLOC target HWLOC::hwloc_2_5 doesn't exist. The tbbbind_2_5 target cannot be created
-- Configuring done (7.7s)
-- Generating done (0.1s)
-- Build files have been written to: D:/Programs/oneTBB/build-2023.2.0
```

三行 `HWLOC target ... doesn't exist` 是**预期结果**，不是错误：本机 MSYS2 没装 hwloc，`tbbbind` 系列库不会生成（影响见 6.2）。

复核配置结果：

```bash
grep -iE "^(CMAKE_BUILD_TYPE|CMAKE_INSTALL_PREFIX|TBB_STRICT|TBB_TEST|TBB_EXAMPLES|TCM_BUILD|TBB_VERIFY_DEPENDENCY_SIGNATURE|CMAKE_CXX_FLAGS):" \
     /d/Programs/oneTBB/build-2023.2.0/CMakeCache.txt
```

```
CMAKE_BUILD_TYPE:STRING=Release
CMAKE_CXX_FLAGS:STRING=-DLOAD_LIBRARY_SAFE_CURRENT_DIRS=0x00002000
CMAKE_INSTALL_PREFIX:PATH=D:/procs/oneTBB2023p2p0
TBB_ENABLE_IPO:BOOL=ON
TBB_EXAMPLES:BOOL=OFF
TBB_STRICT:BOOL=ON
TBB_TEST:BOOL=OFF
TBB_VERIFY_DEPENDENCY_SIGNATURE:BOOL=OFF
TCM_BUILD:BOOL=OFF
```

注意 `TBB_STRICT:BOOL=ON`（默认值，等价 `-Werror`）：本次**没有**关掉它，实测 47/47 目标、0 警告通过，详见 6.4。

### 4.3 编译

```bash
cmake --build /d/Programs/oneTBB/build-2023.2.0 -j 4
```

结果：**47/47 步、0 错误、0 警告**。最后三步是链接：

```
[...] Linking CXX shared library gnu_13.2_cxx11_64_release\libtbb12.dll
[...] Linking C shared library     gnu_13.2_cxx11_64_release\libtbbmalloc.dll
[...] Linking CXX shared library   gnu_13.2_cxx11_64_release\libtbbmalloc_proxy.dll
```

**注意产物位置**：oneTBB 把库放在构建树里的 `gnu_13.2_cxx11_64_release/` 子目录（由编译器 ID + 版本 + 标准 + 位数拼出来），不是常见的 `bin/` 或 `lib/`。这是它支持"同一构建树放多种 ABI"的设计，不要手工去那里找库文件——安装后路径就正常了。

编译期用的是 `-std=c++11`（oneTBB 刻意用低标准构建运行时，保证 ABI 稳定），并启用了 IPO：

```
-flto=auto -fno-fat-lto-objects -fvisibility=hidden -fno-keep-inline-dllexport
```

### 4.4 安装

```bash
cmake --install /d/Programs/oneTBB/build-2023.2.0
```

开头几行（注意 MinGW 的命名）：

```
-- Install configuration: "Release"
-- Installing: D:/procs/oneTBB2023p2p0/lib/libtbb12.dll.a
-- Installing: D:/procs/oneTBB2023p2p0/bin/libtbb12.dll
-- Installing: D:/procs/oneTBB2023p2p0/lib/pkgconfig/tbb.pc
-- Installing: D:/procs/oneTBB2023p2p0/lib/libtbbmalloc.dll.a
-- Installing: D:/procs/oneTBB2023p2p0/bin/libtbbmalloc.dll
-- Installing: D:/procs/oneTBB2023p2p0/lib/libtbbmalloc_proxy.dll.a
-- Installing: D:/procs/oneTBB2023p2p0/bin/libtbbmalloc_proxy.dll
```

安装清单共 **161 项**：

```bash
wc -l < /d/Programs/oneTBB/build-2023.2.0/install_manifest.txt      # 161
```

### 4.5 校验安装结果

```bash
ls /d/procs/oneTBB2023p2p0
# bin  include  lib  share

ls /d/procs/oneTBB2023p2p0/bin
# libtbb12.dll  libtbbmalloc.dll  libtbbmalloc_proxy.dll

ls /d/procs/oneTBB2023p2p0/lib
# cmake  libtbb12.dll.a  libtbbmalloc.dll.a  libtbbmalloc_proxy.dll.a  pkgconfig

find /d/procs/oneTBB2023p2p0/include -name "*.h" | wc -l    # 149
du -sh /d/procs/oneTBB2023p2p0                              # 3.3M
```

---

## 5. 安装结果目录结构

```
D:\procs\oneTBB2023p2p0\
├─ bin\
│  ├─ libtbb12.dll                (650 KB)  任务调度器运行时，核心
│  ├─ libtbbmalloc.dll            (411 KB)  可伸缩内存分配器
│  └─ libtbbmalloc_proxy.dll      (266 KB)  malloc/free 替换代理
├─ include\
│  ├─ tbb\                        51 个转发头文件
│  │  └─ parallel_for.h   →  #include "../oneapi/tbb/parallel_for.h"
│  └─ oneapi\
│     ├─ tbb\                     51 个真正的实现头文件
│     ├─ tbb.h
│     └─ tbb.cppm                 C++20 模块接口
├─ lib\
│  ├─ libtbb12.dll.a              MinGW 导入库（注意不是 .lib）
│  ├─ libtbbmalloc.dll.a
│  ├─ libtbbmalloc_proxy.dll.a
│  ├─ cmake\TBB\                  ★ 消费方 find_package 的入口
│  │  ├─ TBBConfig.cmake          只有一行：include TBBTargets.cmake
│  │  ├─ TBBConfigVersion.cmake
│  │  ├─ TBBTargets.cmake         定义 TBB::tbb / TBB::tbbmalloc / TBB::tbbmalloc_proxy
│  │  └─ TBBTargets-release.cmake IMPORTED_LOCATION 指向 bin/*.dll
│  └─ pkgconfig\tbb.pc            非 CMake 工程的备选入口
└─ share\doc\TBB\README.md
```

关键信息：

- 头文件根目录：`D:\procs\oneTBB2023p2p0\include`（`#include <tbb/...>` 与 `#include <oneapi/tbb/...>` 两种写法都行）
- CMake 配置目录：`D:\procs\oneTBB2023p2p0\lib\cmake\TBB`（`TBB_DIR` 指向这里）
- 导入库：`lib/*.dll.a`；运行库：`bin/*.dll`
- `TBB::tbb` 是 **SHARED IMPORTED** 且带 `IMPORTED_LOCATION`，所以 `$<TARGET_FILE:TBB::tbb>` 能直接拿到 DLL 路径（本项目用它做运行时拷贝）

---

## 6. MinGW 与 Debug 下的六个关键点

### 6.1 `LOAD_LIBRARY_SAFE_CURRENT_DIRS` 未定义（真错误，必须处理）

第一遍编译在 `src/tbb/dynamic_link.cpp` 直接失败：

```
src/tbb/dynamic_link.cpp:562:16: error: 'LOAD_LIBRARY_SAFE_CURRENT_DIRS' was not declared in this scope;
did you mean 'LOAD_LIBRARY_SEARCH_USER_DIRS'?
  562 |         return LOAD_LIBRARY_SAFE_CURRENT_DIRS;
compilation terminated due to -Wfatal-errors.
```

原因：这个常量在 MSYS2 的 `mingw64/include/libloaderapi.h` 里是**有条件定义**的：

```c
#if (NTDDI_VERSION >= NTDDI_WIN10_RS1)
#define LOAD_LIBRARY_SAFE_CURRENT_DIRS 0x00002000
```

而 MinGW 默认 `NTDDI_VERSION` 停在 Windows 7（`0x06010000`），所以这个宏根本没被展开。MSVC 的 Windows SDK 默认目标版本更高，所以官方 CI 不会遇到。

两种修法：

| 方案 | 命令 | 评价 |
| --- | --- | --- |
| **A. 补常量值** | `-DCMAKE_CXX_FLAGS="-DLOAD_LIBRARY_SAFE_CURRENT_DIRS=0x00002000"` | **本次采用**。值照抄头文件里的定义；只在宏缺失时生效，不影响其他声明 |
| B. 抬高目标版本 | `-DCMAKE_CXX_FLAGS="-DNTDDI_VERSION=0x0A000001 -D_WIN32_WINNT=0x0A00"` | 让头文件自己暴露该宏，更"正统"，但会把整个库的目标平台抬到 Win10，可能连带改变其他 API 声明 |

选 A 的理由是改动面最小：只影响一个宏，不改变任何其他声明。

### 6.2 TCM 的 HWLOC 依赖与"二次配置"陷阱

`TCM`（Thread Composability Manager）默认 `TCM_BUILD=ON`，它的 `CMakeLists.txt` 里这样写：

```cmake
if (TCM_BUILD OR TCM_TEST)
  if (IS_TCM_STANDALONE OR TCM_BUILD_OPTION_EXPLICITLY_SET)
    find_package(HWLOC REQUIRED 2.5 VERSION)     # ← 这里会 FATAL
  else()
    find_package(HWLOC 2.5 QUIET)
```

而根 `CMakeLists.txt` 用一个**反直觉**的写法判断"用户是否显式指定"：

```cmake
if (TCM_BUILD)                     # 缓存里是 ON 就算"显式指定"
  set(TCM_BUILD_OPTION_EXPLICITLY_SET TRUE)
```

于是出现这个陷阱：

1. **首次配置**：`TCM_BUILD` 还未进缓存 → 走 `QUIET` 分支 → 打印"Disabling build of TCM since HWLOC cannot be found"，配置成功。
2. **再次配置同一构建树**：`TCM_BUILD` 已在缓存里且为 ON → 被当成"显式指定" → 走 `REQUIRED` 分支 → **配置直接失败**：

```
CMake Error at .../FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find HWLOC (missing: HWLOC_VERSION _hwloc_include_dirs
  _hwloc_lib) (found version "")
Call Stack (most recent call first):
  thread_composability_manager/cmake/modules/FindHWLOC.cmake:105 (find_package_handle_standard_args)
  thread_composability_manager/CMakeLists.txt:42 (find_package)
```

处理：**显式 `-DTCM_BUILD=OFF -DTCM_TEST=OFF`**（本次做法），或者把 `hwloc` 装上。注意 `TCM_TEST` 默认 ON，只关 `TCM_BUILD` 不够——第一个 `if` 仍为真。

影响：没有 hwloc 就没有：

- `tbbbind` / `tbbbind_2_0` / `tbbbind_2_5` 库（NUMA 感知与混合核 P/E 核绑定）
- TCM 库

对普通并行算法（`parallel_for` / `parallel_reduce` / 容器）**没有影响**，只是调度器无法感知 NUMA 拓扑与混合架构核类型。台式机单 NUMA 节点场景可忽略。

### 6.3 依赖签名校验（建议关闭）

`oneTBB` 在 Windows 上有一个运行期检查：通过 `LoadLibraryEx` 加载动态依赖时，可以只允许**已签名**的 DLL。相关选项是 `TBB_VERIFY_DEPENDENCY_SIGNATURE`：

- 不指定：CMake 打印警告（"value is unspecified"）；
- `ON`：只加载已签名依赖 —— 本地编译出来的 DLL 没有签名，会被拒绝；
- `OFF`：不检查 —— 本次选择。

配置输出里的 `-- Dependency signature verification during dynamic linking in run-time is DISABLED` 就是这一项的确认。

### 6.4 `TBB_STRICT`（`-Werror`）居然不需要关

`TBB_STRICT` 默认 `ON`，会给所有目标加 `-Wall -Wextra -Werror`。按常见经验，第三方代码 + MinGW 往往会因警告失败，所以最初排查时曾把它关掉。

事后专门验证了一遍：默认 `TBB_STRICT=ON` 重新配置 + 全新构建，结果 **47/47 目标、0 错误、0 警告**，DLL 正常生成。结论是这套代码在 g++ 13.2 + MinGW 下是 `-Werror` 干净的，**不需要**为它做适配。最终安装就是用默认值构建的（`CMakeCache.txt` 里 `TBB_STRICT:BOOL=ON`），所以文档里的命令可以照抄。

> 教训：MinGW 适配项应当先验证再关，否则会把"能过"的选项误记成"必须关"。

### 6.5 IPO / LTO 可以正常工作

`TBB_ENABLE_IPO` 默认 `ON`，MinGW 下展开成 `-flto=auto -fno-fat-lto-objects`。三个 DLL 都正常链接，没有出现常见的 "`-flto` + DLL 导出" 报错。唯一代价是构建变慢（47 个目标约 1.5–2 分钟）。

### 6.6 Debug 配置下才暴露的 `profiling.h` 缺陷

**这个问题不在 oneTBB 的构建阶段，而在使用阶段**：Release 构建一切正常，切到 Debug 预设编译本项目时，`normal.cpp` 直接失败：

```
D:/procs/oneTBB2023p2p0/include/oneapi/tbb/profiling.h:148:36:
error: cannot convert 'const char*' to 'const tbb::detail::d0::tchar*' {aka 'const wchar_t*'}
  148 |         r1::itt_set_sync_name(obj, name);
      |                                    ^~~~
```

完整因果链有三环：

1. `TBBTargets.cmake` 给所有目标注入了一个按配置生效的定义：

   ```cmake
   INTERFACE_COMPILE_DEFINITIONS "$<$<CONFIG:DEBUG>:TBB_USE_DEBUG>"
   ```

   所以 Debug 配置下编译行里会出现 `-DTBB_USE_DEBUG`（可在 `compile_commands.json` 里看到）。

2. `oneapi/tbb/detail/_config.h` 里 `TBB_USE_PROFILING_TOOLS` 跟随 `TBB_USE_DEBUG`：

   ```c
   #if TBB_USE_DEBUG
       #define TBB_USE_PROFILING_TOOLS 2
   #else
       #define TBB_USE_PROFILING_TOOLS 0
   ```

3. `profiling.h` 同时提供"宽字符重载"和"窄字符转换重载"，但**把 MinGW 排除在外**，随后又直接调用要求 `const wchar_t*` 的导出函数：

   ```cpp
   #if (_WIN32||_WIN64) && !__MINGW32__
       inline void itt_set_sync_name(void* obj, const wchar_t* name) { ... }
       inline void itt_set_sync_name(void* obj, const char* name) { /* 转宽字符 */ }
   #else
       inline void itt_set_sync_name(void* obj, const char* name) {
           r1::itt_set_sync_name(obj, name);   // ← 参数类型不匹配
       }
   #endif
   ```

这是 oneTBB 针对 MinGW 的上游缺陷，MSVC 与 Linux 都不会走到这条分支，所以官方 CI 发现不了。

处理：把该特性关掉即可。ITT 通知只服务于 Intel VTune 之类的性能工具，本项目不用：

```cmake
target_compile_definitions(normal PRIVATE TBB_USE_PROFILING_TOOLS=0)
```

三个备选方案对比：

| 方案 | 做法 | 评价 |
| --- | --- | --- |
| **关特性（本次）** | `-DTBB_USE_PROFILING_TOOLS=0` | 一行，局部，不动第三方文件；保留 `TBB_USE_DEBUG` 的断言语义 |
| 补丁头文件 | 改安装前缀里的 `profiling.h` | 前缀是可复用的公共安装，改动会被下次重装覆盖，且污染"官方 2023.2.0" |
| 不构建 Debug | 只在 Release 下用 TBB | 等于放弃调试能力，不可接受 |

> 注意：关掉的是"ITT 通知"，不是 TBB 的调试断言。`TBB_USE_DEBUG` 仍然生效，Debug 下的 `__TBB_ASSERT` 依旧检查。

---

## 7. 项目的 CMake 集成

共改动 6 个文件（+176 / −6）。

### 7.1 `CMakeLists.txt`（顶层）：可选依赖 + 状态行

```cmake
option(CPP11_ENABLE_TBB
       "Build the oneTBB example in the normal module (needs oneTBB 2021+)" ON)
if(CPP11_ENABLE_TBB)
    find_package(TBB 2021 QUIET)
    if(TBB_FOUND AND NOT TARGET TBB::tbb)
        set(TBB_FOUND FALSE)
    endif()
    if(NOT TBB_FOUND)
        message(WARNING
            "oneTBB not found: the TBB example will be compiled out. "
            "Pass -DTBB_DIR=<prefix>/lib/cmake/TBB, or add the install prefix "
            "to CMAKE_PREFIX_PATH, or configure with -DCPP11_ENABLE_TBB=OFF "
            "to silence this warning.")
    endif()
endif()
```

状态行：

```cmake
if(TBB_FOUND)
    message(STATUS "[cpp11] oneTBB         : ${TBB_VERSION}  (${TBB_DIR})")
else()
    message(STATUS "[cpp11] oneTBB         : not used")
endif()
```

两个细节：

- `TBBConfig.cmake` 只有一行 `include(TBBTargets.cmake)`，**不定义任何版本变量**。`TBB_VERSION` 是 CMake 从 `TBBConfigVersion.cmake` 的 `PACKAGE_VERSION` 自动填的（实测得到 `2023.2.0`）。
- 不能写 `find_package(TBB COMPONENTS tbb)`：这个 config 不处理组件，指定组件反而会让 `find_package` 失败。因此改成"`TBB_FOUND` 且 `TARGET TBB::tbb` 存在"两个条件，后者才是真正能用的判据。

### 7.2 `src/CMakeLists.txt`：选项 → 宏

```cmake
if(CPP11_ENABLE_TBB AND TBB_FOUND)
    set(ENB_TBB ON)
else()
    set(ENB_TBB OFF)
endif()
```

### 7.3 `src/normal/CMakeLists.txt`：`PRIVATE` 链接，不抬标准

```cmake
if(ENB_CGAL)
    target_link_libraries(normal PRIVATE CGAL::CGAL)
endif()

if(ENB_TBB)
    target_link_libraries(normal PRIVATE TBB::tbb)
    target_compile_definitions(normal PRIVATE TBB_USE_PROFILING_TOOLS=0)
endif()
```

`TBB_USE_PROFILING_TOOLS=0` 是 Debug 配置下的必需项，原因见 6.6（oneTBB 的 `profiling.h` 在 MinGW 下把 `const char*` 传给要求 `const wchar_t*` 的函数）。它只关掉 ITT 性能通知，不影响 `TBB_USE_DEBUG` 带来的断言检查。

**为什么是 `PRIVATE`**：`normal.hpp` 不包含 CGAL 也不包含 TBB 头文件，链接依赖不该传播到 `mymainrun` / `cpp11runtest` / `winmingwrun`，否则这些目标也会被拖上额外依赖。

**为什么不像最初设想那样设置 `CXX_STANDARD 17`**：一开始以为 oneTBB 要求消费方 C++17，于是加了显式设置。实测发现这个前提是错的——用 `-std=c++11/14/17` 分别编译同一段使用 `parallel_for` / `parallel_reduce` / `concurrent_vector` 的代码，**三种标准都通过**：

```
c++11: OK
c++14: OK
c++17: OK
```

所以 oneTBB 的导入目标不提供 `INTERFACE_COMPILE_FEATURES` 是"确实不需要"，而不是"漏了"。最终**没有**设置 `CXX_STANDARD`：`normal` 的标准仍由 `CPP11_CXX_STANDARD`（默认 14）与 CGAL 的 `cxx_std_17` 共同决定。实测结果见 9.2。

### 7.4 `src/CMakeLists.txt`：把 `libtbb12.dll` 拷到 exe 旁边

```cmake
if(ENB_TBB AND TARGET TBB::tbb)
    list(APPEND _cpp11_toolchain_dlls $<TARGET_FILE:TBB::tbb>)
endif()
```

这一段接在既有的运行时 DLL 拷贝逻辑里（CGAL/GMP 那一套）。为什么必须单独加：

- `$<TARGET_RUNTIME_DLLS:mymainrun>` 只遍历 `mymainrun` 自身的链接依赖；
- `TBB::tbb` 是 `normal` 的 **PRIVATE** 依赖，对 `mymainrun` 不可见，所以生成器表达式抓不到它；
- 结果是 `libtbb12.dll` 缺失 → 运行期 `126` 错误（与之前 GMP/MPFR 那次同一类故障）。

用 `$<TARGET_FILE:TBB::tbb>` 而不是硬编码文件名，是因为 MSVC 下叫 `tbb12.dll`、MinGW 下叫 `libtbb12.dll`，路径也随安装前缀变化。

### 7.5 `src/projectconfig.h.in`

```c
#cmakedefine ENB_TBB
```

### 7.6 `src/normal/normal.hpp` / `normal.cpp`

头文件加无条件声明：

```cpp
void tbb_demo();
```

实现用 `#ifdef ENB_TBB` 包住，包含的头文件：

```cpp
#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <tbb/info.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/version.h>
```

并在 `normal_test_all()` 里调用 `tbb_demo();`（紧跟 `cgal_demo()`）。

---

## 8. 示例内容与运行结果

`tbb_demo()` 的设计目标是"**结果可核对**"：并行程序最容易写错的地方是"跑通但算错"，所以三个用例都选有确定答案的：

| 用例 | API | 校验方式 |
| --- | --- | --- |
| 平方表 | `tbb::parallel_for` + lambda | `squares.back()` 应等于 `(2^20−1)^2 = 1099509530625` |
| 平方和 | `tbb::parallel_deterministic_reduce` | 与闭式解 `n(n+1)(2n+1)/6` 比对 |
| 筛倍数 | `tbb::concurrent_vector::push_back` | 元素个数与 `⌊(2^20−1)/7⌋+1` 比对 |

```cpp
void tbb_demo() {
    std::cout << "===== oneTBB demo =====" << std::endl;
#ifndef ENB_TBB
    std::cout << "oneTBB is not configured (ENB_TBB undefined); skipping the demo" << std::endl;
#else
    std::cout << "oneTBB version: " << TBB_VERSION_STRING
              << "  (interface " << TBB_INTERFACE_VERSION << ")" << std::endl;
    std::cout << "default concurrency = " << tbb::info::default_concurrency()
              << std::endl;

    // 1. parallel_for：每个下标只被一个任务写，内容与调度无关
    const std::size_t kCount = 1u << 20;  // 1 048 576
    std::vector<long long> squares(kCount);
    tbb::parallel_for(std::size_t(0), kCount, [&squares](std::size_t i) {
        squares[i] = static_cast<long long>(i) * static_cast<long long>(i);
    });
    std::cout << "parallel_for    : " << squares.size() << " squares, last = "
              << squares.back() << std::endl;

    // 2. 确定性归约：归约树固定，结果可复现，能与闭式解比对
    const long long kMax = 1000000;
    const long long sum = tbb::parallel_deterministic_reduce(
        tbb::blocked_range<long long>(0, kMax + 1), 0LL,
        [](const tbb::blocked_range<long long> &r, long long partial) {
            for (long long i = r.begin(); i != r.end(); ++i) { partial += i * i; }
            return partial;
        },
        [](long long a, long long b) { return a + b; });
    const long long closed_form = kMax * (kMax + 1) * (2 * kMax + 1) / 6;
    std::cout << "sum of i^2      : " << sum << " (closed form " << closed_form
              << ", match: " << ((sum == closed_form) ? "true" : "false") << ")"
              << std::endl;

    // 3. concurrent_vector：多任务并发 push_back，无锁；顺序不定，只校验个数
    tbb::concurrent_vector<long long> multiples;
    tbb::parallel_for(std::size_t(0), kCount,
                      [&multiples, &squares](std::size_t i) {
                          if (squares[i] % 7 == 0) {
                              multiples.push_back(static_cast<long long>(i));
                          }
                      });
    const std::size_t expected = (kCount - 1) / 7 + 1;
    std::cout << "concurrent_vector: " << multiples.size()
              << " multiples of 7 in [0, 2^20) (expected " << expected
              << ", match: " << ((multiples.size() == expected) ? "true" : "false") << ")"
              << std::endl;
#endif // ENB_TBB
    std::cout << "===== oneTBB demo end =====" << std::endl;
}
```

运行 `build/bin/mymainrun.exe` 的实际输出：

```
===== oneTBB demo =====
oneTBB version: 2023.2.0  (interface 12200)
default concurrency = 8
parallel_for    : 1048576 squares, last = 1099509530625
sum of i^2      : 333333833333500000 (closed form 333333833333500000, match: true)
concurrent_vector: 149797 multiples of 7 in [0, 2^20) (expected 149797, match: true)
===== oneTBB demo end =====
```

三处数字都可以独立验算：

- `1048575² = 1099509530625`；
- `333333833333500000` 正是 `⌊n(n+1)(2n+1)/6⌋` 在 `n = 10⁶` 时的值（未溢出 `long long`，1e6·1e6·2e6 ≈ 2×10¹⁸ < 9.22×10¹⁸）；
- `[0, 2²⁰)` 内 7 的倍数共 `⌊1048575/7⌋+1 = 149797` 个（含 0）。

`default concurrency = 8` 与机器逻辑处理器数一致——这也说明运行期确实连上了 `tbb12.dll` 的调度器，而不是某个被内联的空实现。

---

## 9. 验证矩阵（本次实际执行结果）

### 9.1 构建与运行

| 场景 | 命令 | 结果 |
| --- | --- | --- |
| 配置 | `cmake --preset local` | 退出码 0；`[cpp11] oneTBB : 2023.2.0 (D:/procs/oneTBB2023p2p0/lib/cmake/TBB)` |
| 编译 | `cmake --build --preset local` | 0 error |
| 运行 | `PATH="C:\Windows\System32;C:\Windows" ./build/bin/mymainrun.exe` | 退出码 0，816 行输出，`retcode=0` |
| 冒烟测试 | `ctest --preset local` | `1/1 Test #1: smoke ... Passed 0.64 sec`，100% |
| Debug 预设 | `--preset local-debug` | 配置/编译/运行均通过（`TBB_USE_PROFILING_TOOLS=0` 已在 7.3 加上，否则会编译失败，见 6.6） |

### 9.2 C++ 标准与依赖隔离

| 场景 | `normal.cpp` 的 `-std` | 说明 |
| --- | --- | --- |
| CGAL + TBB 都开（默认） | `-std=c++17` | C++17 来自 `CGAL::CGAL` 的 `cxx_std_17`，不是 TBB |
| `main.cpp` / `cpp11runtest.cpp` | `-std=c++14` | TBB 的 PRIVATE 链接没有外泄 |
| `-DCPP11_ENABLE_CGAL=OFF`（TBB 仍开） | `-std=c++14` | **TBB 示例在 C++14 下正常编译并输出相同结果**，证明 TBB 不要求抬标准 |

### 9.3 链接与运行期依赖

`objdump -p build/bin/libnormal.dll` 的依赖：

```
libgcc_s_seh-1.dll
KERNEL32.dll
msvcrt.dll
libwinpthread-1.dll
libstdc++-6.dll
libgmp-10.dll        ← CGAL
libmpfr-6.dll        ← CGAL
libtbb12.dll         ← oneTBB（本次新增）
```

`build/bin` 下的运行库（自包含）：

```
libgcc_s_seh-1.dll  libgmp-10.dll  libgmpxx-4.dll  libmpfr-6.dll
libstdc++-6.dll     libtbb12.dll   libwinpthread-1.dll
```

### 9.4 降级路径

| 场景 | 命令 | 结果 |
| --- | --- | --- |
| 显式关闭 | `-DCPP11_ENABLE_TBB=OFF` | 配置 0，状态行 `oneTBB : not used`，`/* #undef ENB_TBB */`；构建产物运行打印 `oneTBB is not configured ... skipping the demo` |
| TBB 路径不存在 | `-DTBB_DIR=D:/nonexistent/lib/cmake/TBB` | 配置仍成功，打印 CMake Warning（含三种修复建议），示例被编译掉 |

---

## 10. 注意事项

1. **不要用 `oneapi-tbb-*-win.zip` 的预编译库**：MSVC ABI + `.lib` 导入库，MinGW 链接必失败或运行期崩溃。只能用 `-src` 包自己编。
2. **安装前缀必须改**：默认是 `c:/Program Files/TBB`，会在 Program Files 里制造一个非安装器安装的目录，卸载/清理都麻烦。
3. **`TBB_TEST=OFF` 是必选项**：默认 ON 会生成上千个测试目标，构建时间与磁盘占用都不可接受。
4. **`TCM_BUILD=OFF` + `TCM_TEST=OFF` 一起给**：只关一个会在二次配置时踩 `find_package(HWLOC REQUIRED)`（见 6.2）。
5. **构建树里的库在 `gnu_13.2_cxx11_64_release/`**，不在 `bin/`；要引用产物请用 `$<TARGET_FILE:TBB::tbb>` 或安装后的路径。
6. **导入库后缀是 `.dll.a`**，不是 `.lib`；DLL 名带 `lib` 前缀（`libtbb12.dll`）。
7. **`libtbbmalloc_proxy.dll` 不会自动生效**：它靠链接时替换 `malloc` 符号来工作，必须显式链接 `TBB::tbbmalloc_proxy`；只是把 DLL 放在旁边不起作用。本项目没有用它。
8. **`tbbbind*` 缺失是正常的**：没装 hwloc 就没有 NUMA/混合核绑定支持，普通并行算法不受影响。
9. **头文件有两套路径**：`tbb/*.h`（转发）与 `oneapi/tbb/*.h`（实现）。写 `<tbb/tbb.h>` 或 `<oneapi/tbb.h>` 都可以，但不要混用同一头文件的两种拼法。
10. **运行期 DLL 必须与 exe/pyd 同目录**：Python 3.8+ 加载扩展模块时不搜 PATH，所以 `libtbb12.dll` 必须被拷到 `build/bin`（本项目已自动化）。
11. **`TBB_VERSION` 不是 config 文件给的**：它由 CMake 从 `TBBConfigVersion.cmake` 填；自己写 `find_package` 逻辑时不要假设 config 里能读到版本变量。
12. **Release 能编通不等于 Debug 能编通**：`TBBTargets.cmake` 只在 Debug 配置注入 `TBB_USE_DEBUG`，由此打开 ITT 通知，才会踩到 6.6 的 `profiling.h` 缺陷。接入 TBB 后 Release 与 Debug 两个配置都必须验证一遍。

---

## 11. 项目侧使用方式

`CMakeUserPresets.json`（`.gitignore` 已忽略，不入库）里的本机预设已包含 oneTBB：

```json
"CMAKE_PREFIX_PATH": "D:/procs/boost_1_79_0;D:/procs/cgal6p2p1;D:/procs/oneTBB2023p2p0",
"TBB_DIR": "D:/procs/oneTBB2023p2p0/lib/cmake/TBB"
```

日常：

```bash
cmake --preset local && cmake --build --preset local && ctest --preset local
```

不用预设时，二选一告诉 CMake 位置：

```bash
# 方式 A：直接指向 config 目录
cmake -S . -B build -G Ninja -DTBB_DIR=D:/procs/oneTBB2023p2p0/lib/cmake/TBB

# 方式 B：把前缀加入搜索路径（find_package 自己找 lib/cmake/TBB）
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=D:/procs/oneTBB2023p2p0
```

新工程接入的最小片段：

```cmake
find_package(TBB 2021 REQUIRED)
target_link_libraries(<your_target> PRIVATE TBB::tbb)
```

---

## 12. 故障排查速查

| 现象 | 原因 | 处理 |
| --- | --- | --- |
| `'LOAD_LIBRARY_SAFE_CURRENT_DIRS' was not declared` | MinGW 头文件按 `NTDDI_VERSION` 条件定义 | 加 `-DCMAKE_CXX_FLAGS="-DLOAD_LIBRARY_SAFE_CURRENT_DIRS=0x00002000"`（或抬 `NTDDI_VERSION`） |
| 二次配置报 `Could NOT find HWLOC` | TCM 把缓存里的 `TCM_BUILD=ON` 当"显式指定"，走 `REQUIRED` 分支 | `-DTCM_BUILD=OFF -DTCM_TEST=OFF`，或安装 hwloc |
| 链接报 `undefined reference to tbb::detail::r1::...` | 没链 `TBB::tbb`，或链了 MSVC 的 `.lib` | 用 `find_package(TBB)` + `TBB::tbb`；确认 `lib/*.dll.a` 存在 |
| 运行报 `ImportError ... 126` / 双击无反应 | `libtbb12.dll` 不在 exe/pyd 同目录 | 让 POST_BUILD 拷 `$<TARGET_FILE:TBB::tbb>`（本项目已做），或把 `D:/procs/oneTBB2023p2p0/bin` 加进 PATH |
| `find_package(TBB COMPONENTS tbb)` 失败 | 该 `TBBConfig.cmake` 不处理组件 | 去掉 `COMPONENTS`，改用 `if(TARGET TBB::tbb)` 判断 |
| 构建树里找不到 `.dll` | 产物在 `gnu_13.2_cxx11_64_release/` 子目录 | 用 `$<TARGET_FILE:TBB::tbb>`，或在安装后取 `bin/` |
| `default concurrency` 与核数不符（偏小） | 进程被 CPU 亲和性/任务管理器限制 | 与本库无关；检查 `tbb::info::default_concurrency()` 与系统亲和性设置 |
| Debug 构建报 `cannot convert 'const char*' to 'const wchar_t*'`（`profiling.h`） | Debug 下 `TBB_USE_DEBUG` 打开了 ITT 通知，MinGW 分支存在类型缺陷（6.6） | `target_compile_definitions(<target> PRIVATE TBB_USE_PROFILING_TOOLS=0)` |
| 想用 NUMA/混合核绑定但没有 `tbbbind` 库 | 构建时无 hwloc | 安装 hwloc 后重新配置（`-DTCM_BUILD=ON` 也不需要，`tbbbind` 独立于 TCM） |
