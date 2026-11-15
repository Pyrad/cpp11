# cpp11 编译配置优化日志报告

| 项目 | 内容 |
| --- | --- |
| 报告日期 | 2026-11-15 |
| 工作目录 | `D:/Pyrad/Gitee/cpp11`（MSYS2 路径 `/d/Pyrad/Gitee/cpp11`） |
| 分支 | `master` |
| 依据文档 | `BUILD_CONFIG_REVIEW.md`（2026-11-14 的评审与分阶段改进建议） |
| 构建目录 | `D:/Pyrad/Gitee/cpp11/build` |
| 报告定位 | 记录本次「编译成功 → 修复运行期崩溃 → 按评审文档优化 → 回归验证」的完整过程、结论与遗留项 |
| 文件名说明 | 采用英文文件名，避免 Gitee / GitHub 双仓库同步时的中文编码差异 |

---

## 0. 一页速览

本次工作分三次提交完成，最终状态：**构建通过、运行正常、无需任何手工环境变量**。

| 阶段 | 提交 | 内容 | 结果 |
| --- | --- | --- | --- |
| 1 | `36d6c06` | 提交 MSYS2/Windows 平台分流（此前为让项目能编译而做的最小改动） | 已提交 |
| 2 | `c738bf5` | 修复运行期 SIGSEGV 及其关联问题 | 已提交，`rc=0` |
| 3 | 本次提交 | 按评审文档阶段 1–6 正式优化 + 本报告 | 见第 6、7 节 |

关键结果：

- `cmake --build build --clean-first -j 4` → **54/54 通过，0 错误**。
- `PATH` 中**不含** Python、Boost 或任何模块目录时，`./build/bin/mymainrun.exe` → **退出码 0**，嵌入的 Python 脚本 `retcode=0`，输出 804 行，无 Traceback。
- `ctest --test-dir build` → **1/1 通过**（0.03 s）。
- 6 种 `ENB_*` 开关组合全部构建通过（评审文档 P0-4 的失败场景已消除）。
- C++14 / 17 / 20 构建通过；C++11 失败且已给出明确诊断（见 7.3）。
- 构建产物统一落在 `build/bin/`，无需再 `source set_shared_lib_path.sh`。

一句话结论：项目从「只在一台已知机器、全开关打开、手工 source 脚本这一条窄路径可用」，改造成「依赖由 `find_package` 校验、依赖由 CMake target 传递、产物自包含、开关与标准可切换、构建过程由 Presets 版本化」。

---

## 1. 环境实测

| 项 | 值 |
| --- | --- |
| 调用方式 | `MSYSTEM=MINGW64 /d/proc/msys64/usr/bin/bash.exe -lic '...'` |
| CMake | 4.0.2（`/d/proc/msys64/mingw64/bin/cmake.exe`） |
| Ninja | 1.12.1 |
| g++ | 15.1.0（MSYS2 project, MinGW-w64, x86_64-win32-seh） |
| Boost | 目录名 `D:/proc/boost_1_97_0_msys`，**实际内容 Boost 1.92.0**（`include/boost-1_92`、`lib/cmake/Boost-1.92.0/BoostConfig.cmake`） |
| Python | `D:/proc/Python313` = CPython **3.13.7**（MSVC ABI，只有 `libs/python313.lib`，无 `.dll.a`） |
| `DEV_NICKNAME` | 由 `.bashrc` 提供，**优化后 CMake 已不再依赖它** |

> 注意：Boost 目录名里的 `1_97_0` 与真实版本 `1.92.0` 不一致，是历史命名。现在由 `find_package(Boost CONFIG)` 读取 `BoostConfig.cmake` 自动得到真实版本，不再依赖人工写对目录名。

配置时显式给出本机依赖前缀（这些**没有**写进版本库）：

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH="D:/proc/boost_1_97_0_msys" \
      -DPython3_ROOT_DIR="D:/proc/Python313"
```

等价的免参数方式是使用本机 `CMakeUserPresets.json`（已 gitignore）中的 `local` 预设：`cmake --preset local`。

---

## 2. 执行时间线

### 2.1 第一次提交 `36d6c06`

`build(cmake): select dependency paths by platform for WSL machine nickname`

起因：零修改基线构建在 Windows/MSYS2 下失败，报错为

```
ninja: error: '/home/pyrad/proc/Python-3.13.7/lib/libpython3.13.so',
needed by 'pyrun/libpyrun.dll.a', missing and no known rule to make it
```

原因是 `DEV_NICKNAME=Asus_tuf_gaming_b650m_plus_Win11_WSL_Ubuntu_24_04_LTS` 那条分支写死 WSL Linux 路径。改动只在该分支内部加 `if(WIN32)` 分流：Windows 走 `D:/proc/...`，WSL Linux 分支逐字保留。此次提交仅改 `src/CMakeLists.txt`（+31 / −10）。

**附带说明**：提交时 git 报 `Author identity unknown`，因此按历史提交把作者身份设为**仓库级**（非全局）：`pyrad <cailong89@163.com>`。该配置位于 `.git/config`，不随代码入库。

### 2.2 第二次提交 `c738bf5`

`fix(pyrun): avoid passing a FILE* across C runtimes (runtime SIGSEGV)` —— 详见第 3 节。

### 2.3 第三次提交（本次）

按评审文档阶段 1–6 的优化，以及本日志报告。详见第 4–7 节。

---

## 3. 运行期段错误专题：原因与解决方法

### 3.1 现象

全开关打开的 `mymainrun.exe` 退出码 **139 (SIGSEGV)**，且输出到文件时为空（stdout 全缓冲，进程崩溃前未 flush，所以「无输出」不等于「崩在开头」）。

关闭 `-DENB_PYRUN_CPP=OFF` 的对照变体构建成功、运行 `rc=0`，把问题限定在 Python 嵌入路径。

### 3.2 根因（gdb 实测，非推测）

```
Thread 1 received signal SIGSEGV
#3  0x... in ucrtbase!fgetc () from C:\WINDOWS\System32\ucrtbase.dll
#4  0x... in python313!_Py_UniversalNewlineFgetsWithSize ()
#19 0x... in python313!PyRun_SimpleFileExFlags ()
#0..#2  ntdll!RtlInitializeResource / RtlRaiseStatus / RtlEnterCriticalSection
```

头文件 `D:/proc/Python313/include/cpython/pythonrun.h:83` 显示：

```c
#define PyRun_SimpleFile(f, p) PyRun_SimpleFileExFlags((f), (p), 0, NULL)
```

即 `PyRun_SimpleFile()` 会把 `FILE*` 直接交给 CPython。

- 宿主程序由 **MinGW** 编译，链接 `msvcrt.dll`，`fopen()` 返回的是 **msvcrt 的 `FILE` 结构**；
- 官方 CPython 3.13 Windows 版由 **MSVC** 编译，使用 **UCRT**（`ucrtbase.dll`）；
- `FILE` 是不透明结构，由创建它的 C 运行时拥有，两个运行时的布局/内部锁字段完全不同；
- `ucrtbase!fgetc()` 把 msvcrt 的 `FILE*` 当成 UCRT 的结构来读，解引用了一个垃圾临界区指针，在 `RtlEnterCriticalSection` 崩溃。

这也解释了 `pyrun.cpp` 里 2024-01-06 的历史注释「这段会段错误，原因不明」，以及「WSL Ubuntu 上却能跑」——Linux 上宿主与 Python 用的是同一个 glibc CRT，不存在跨运行时边界。

### 3.3 解决方法

**不再把 `FILE*` 跨 C 运行时传递**：在宿主自己的 CRT 内用 C++ 流把 `run.py` 读成 `std::string`，再通过 `PyRun_SimpleStringFlags(source.c_str(), nullptr)` 交给 CPython。该 API 只接收普通 `char*`，彻底消除 CRT 边界，且在 Linux/WSL 上行为一致。

`PyRun_SimpleString*` 不会像 `PyRun_SimpleFile()` 那样设置 `__file__` 和 `sys.path[0]`，因此在执行前用 C API 补回：

- `PyDict_SetItemString(globals, "__file__", ...)`；
- `PyList_Insert(sys.path, 0, <脚本所在目录>)`。

### 3.4 同一执行链上的其他修复

| 问题 | 修法 |
| --- | --- |
| `Py_GetPythonHome()` / `Py_GetProgramName()` 返回 `wchar_t*` 却用 `%s` 打印（且 3.13 起 deprecated，会产生 2 条警告） | 改读 `sys.prefix` / `sys.executable`，用 `PyUnicode_AsUTF8` 转窄字符串；构建警告从 2 条降为 0 |
| `std::string(getenv("DEV_NICKNAME"))` 在变量缺失时构造空指针 | 先判 NULL |
| `run.py` 里 5 处按机器硬编码的绝对路径（`D:/Gitee/...`、`/home/pyrad/...`） | 全部改为由 `__file__` 推导 |
| Python 3.8+ 在 Windows 上不再用 `PATH` 搜索扩展模块的依赖 DLL，导致 `ImportError: DLL load failed while importing cpp11runtest` | 用 `os.add_dll_directory()` 注册由脚本位置推导出的目录 |
| `run.py` 用 POST_BUILD 拷贝，只有 `pyrun` 目标重链时才刷新 | 改为 `configure_file(... COPYONLY)`，脚本一改就触发重配置 |
| 旧的死 `pyhome` 字符串（打印了 WSL 路径但从未调用 `Py_SetPythonHome`） | 删除，Python home 交给解释器自行探测（后续优化中由 `PyConfig.home` 接管） |

验证：构建 0 警告、`rc=0`、`Succeeded running file (retcode=0)`、`Finished running Python function: run_main`。

---

## 4. 优化内容（对照评审文档阶段）

### 阶段 1 ｜P0 止血

| 编号 | 落地情况 |
| --- | --- |
| P0-1 | `cmake_minimum_required` 从 3.10 提到 **3.21**（支持 Presets v3 与 `$<TARGET_RUNTIME_DLLS>`），命令风格统一小写 |
| P0-2 | `projectconfig.h.in` 改用 `@PROJECT_VERSION_MAJOR@` / `@PROJECT_VERSION_MINOR@`，新增 `CPP11_PYTHON_VERSION`，`configure_file(... @ONLY)`；生成的宏实测为 `0` / `1`，不再是空宏 |
| P0-3 | `src/pymods/CMakeLists.txt` 去掉对 `CurSharedLibName`（在 pyrun 目录作用域，不可见）的引用，改为自包含的 `configure_file(COPYONLY)`，并由顶层 `add_subdirectory(pymods)` 真正加载 |
| P0-4 | 删除 `pyrun/CMakeLists.txt` 里无条件写死的 5 个库名，改由 `src/CMakeLists.txt` 的 `CPP11_MODULES` 只收集**真实存在**的 target；6 种开关组合全部构建通过 |
| P0-5 | 整个 `CUSTOM_LINK_DIRS` / `LINK_DIRS` 变量拼写错误与「把库文件当链接目录」的问题随 `target_link_directories` 一并删除 |
| P0-6 | 统一产物命名：`pyrun` → `libpyrun.dll`、`winmingwrun` → `libwinmingwrun.dll`（平台默认），只有真正的 Python 扩展模块 `cpp11runtest` 保留 `cpp11runtest.pyd`；`run.py`、README、脚本注释同步 |
| P0-7 | `set_shared_lib_path.sh` 与 `no_use/run.sh` 的 shebang 改为 `#!/usr/bin/env bash`，头部注明「只能 source」及「现已降级为兼容兜底」；`bash -n` 均通过 |
| P0-8 | 删除 `src/CMakeLists.txt` 里那段含 `${UNIX} STREQUAL "Unix"` 死分支、会误拒 MSVC/macOS 的平台判断；平台差异改用 `WIN32` / `UNIX` |

### 阶段 2 ｜依赖发现现代化

- 删除 `src/CMakeLists.txt` 中约 110 行按 `DEV_NICKNAME` 硬编码的绝对路径，以及全部手写 `Python3_INCLUDE_DIRS` / `Python3_LIBRARIES` / `Python3_LIBRARY` / `PYTHON_EXECUTABLE`。
- 新增 `find_package(Boost 1.70 CONFIG REQUIRED)` 与 `find_package(Python3 COMPONENTS Interpreter Development REQUIRED)`；实测命中 Boost 1.92.0、Python 3.13.7。
- 依赖通过 imported target 传递：`Boost::headers`、`Python3::Python`（`pyrun` / `winmingwrun`）、`Python3::Module`（`cpp11runtest`）。
- 删除全局 `include_directories`，改由 INTERFACE target `cpp11_config` 统一携带 `<repo>/src`、生成的 `projectconfig.h` 目录与 `Boost::headers`。
- 机器专属路径不再入库：走 `CMakeUserPresets.json`（gitignore）、`-DCMAKE_PREFIX_PATH` / `-DPython3_ROOT_DIR`，或 `CPP11_BOOST_ROOT` / `CPP11_PYTHON_ROOT` 环境变量。
- **不设置 `DEV_NICKNAME` 也能配置成功**；负向用例（错误 Boost 路径）得到清晰的 `find_package` 报错而非静默用错库。

### 阶段 3 ｜消灭手工路径与运行时脚本

- 所有 `target_link_directories`、手工 `LINK_DIRS` / `INC_DIRS` 全部删除，依赖由 `target_link_libraries` 推导。
- 统一输出目录：`build/bin/` 放 exe、DLL/SO、`.pyd`、`run.py`、`top.py` 及拷贝来的第三方 DLL；`build/lib/` 放导入库；Linux 侧设 `$ORIGIN` RPATH。
- `$<TARGET_RUNTIME_DLLS:mymainrun>` + POST_BUILD 把 `python313.dll` 自动拷到 `build/bin/`。**这是「零 PATH 运行」的关键**：默认 PATH 里没有 `D:/proc/Python313`，Windows 加载器在进程启动前就要解析该 DLL。
- 因为 CPython 会从 DLL 位置推导 `sys.prefix`，拷贝后会报 `Could not find platform independent libraries <prefix>`；因此在 `pyrun.cpp` 用现代 `PyConfig.home`（而非已废弃的 `Py_SetPythonHome`）把 home 固定为 `CPP11_PYTHON_HOME`。实测 `sys.prefix = D:/proc/Python313`，标准库路径正确。
- `run.py` / `top.py` 用 `configure_file(... COPYONLY)` 放进 `build/bin/`；`pyrun.cpp` 通过 CMake 提供的 `CPP11_PYRUN_SCRIPT` 编译宏拿到绝对路径，并保留相对路径兜底。
- `set_shared_lib_path.sh` 保留为兼容兜底，但不再是运行前置条件。

### 阶段 4 ｜顶层布局与 Presets

- 新增仓库根 `CMakeLists.txt`：`cmake_minimum_required` / `project(cpp11 VERSION 0.1 LANGUAGES CXX)` / 全局策略 / `enable_testing()` / `add_subdirectory(src)`。
- `src/CMakeLists.txt` 移除重复的 `project()` 与最低版本声明，改为纯子配置；`CMAKE_SOURCE_DIR` 变化后，原先的 `${PROJECT_SOURCE_DIR}` 相对包含改由 `cpp11_config` 的 `$<BUILD_INTERFACE:...>` 统一处理。
- 新增 `CMakePresets.json`（`msys2` / `linux` / `debug` / `cxx11` / `cxx17` / `cxx20` / `asan`）。
- 新增 `CMakeUserPresets.json`（**gitignore、不入库**），`local` 预设把本机前缀写入 cache 且 `binaryDir = ${sourceDir}/build`，使 `cmake --preset local` 正好落在要求的 `build/` 目录。
- README 增加 Quick Start，并把旧章节整体标注为「Historical notes」；修正 `$ g remote -v` 笔误，更新依赖表。

### 阶段 5 ｜适配「验证 C++ 特性」的定位

- `CPP11_CXX_STANDARD` cache 变量（11/14/17/20/23，默认 14），`CMAKE_CXX_EXTENSIONS OFF` → 编译参数为 `-std=c++14` 而非 `-std=gnu++14`（已核对 `compile_commands.json`）。
- INTERFACE target `cpp11_warnings`：`-Wall -Wextra -Wpedantic`（MSVC 用 `/W4`），由 `CPP11_ENABLE_WARNINGS` 控制，**不设 `-Werror`**。
- `CPP11_ENABLE_ASAN` 提供 ASan + UBSan 开关。
- `enable_testing()` + `add_test(NAME smoke COMMAND mymainrun)`，`ctest` 冒烟用例通过。
- `AUX_SOURCE_DIRECTORY(./ MY_DIR_SRC)` 改为显式 `add_executable(mymainrun main.cpp)`。
- `EffectiveModernCpp` 的 35 个源文件改用 `file(GLOB ... CONFIGURE_DEPENDS)`，新增 chapter 无需再改 CMakeLists。
- 单配置生成器未指定时默认 `Release`，不覆盖用户显式指定。

### 阶段 6 ｜清理与文档同步

- `.gitignore` 补齐 `build*/`、`CMakeUserPresets.json`、`*.pyd`、`*.dll.a`、`out/`、`cmake-build-*/`、`.vs/`、`_deps/`、`.workbuddy/`。
- 两个 shell 脚本 shebang 修正并加说明；`cmakeMyTest.cmake` 加注「独立试验脚本，不参与构建」。
- `BUILD_CONFIG_REVIEW.md` 头部状态更新为「已执行」。
- **未执行**：`no_use/` 移入 `archive/`（D9）、双仓库 `git push`（评审文档阶段 6.6，本次只做本地提交，未推送远端）。

---

## 5. 决策点 D1–D12 的落地结论

| 编号 | 结论 |
| --- | --- |
| D1 | **A**：顶层 `CMakeLists.txt` 移到仓库根 |
| D2 | **3.21**（工具链为 4.0.2，可用 `$<TARGET_RUNTIME_DLLS>` 与 Presets v3） |
| D3 | **A**：`find_package(Boost CONFIG)`，本机确有 `BoostConfig.cmake`；未引包管理器 |
| D4 | **保留全部**（未精简）：`pyrun` / `cpp11runtest` / `winmingwrun` / `pymods` 全部修好并启用；归档或改用 pybind11 留作后续议题 |
| D5 | **A + B**：统一 `build/bin/`；`$<TARGET_RUNTIME_DLLS>` 拷第三方 DLL；Linux 加 `$ORIGIN`；脚本降级为兜底 |
| D6 | **A**：CMake 侧彻底删除 `DEV_NICKNAME`；源码与 `run.py` 里的机器分派也一并清理 |
| D7 | **B**：`file(GLOB CONFIGURE_DEPENDS)`；`gen_emcxx_new_chapter.sh` 仍需手动在 `effectiveModernCpp.cpp` 中注册（遗留项） |
| D8 | **B**：未批量改 40+ 处 `#include "../xxx"`，改由 `cpp11_config` 统一提供 `<repo>/src` 包含路径 |
| D9 | **暂缓**：`no_use/` 保留原位，仅修 shebang |
| D10 | **暂不加 CI** |
| D11 | **默认保持 14**，另提供 11/17/20/23 标准矩阵 |
| D12 | **B**：`utilities` 仍为 `SHARED`，避免同时改变产物形态 |

两个事实性问题的答案按本轮环境推定：主力环境为 **MSYS2 MINGW64**；`src/pymods/` 与 `winmingwrun` 未确认仍在使用，但因删除风险大于收益，选择「修好并保留」而非归档。

---

## 6. 改动文件清单（第三次提交）

**新增（纳入版本库）**

| 文件 | 作用 |
| --- | --- |
| `CMakeLists.txt` | 顶层工程入口、全局策略、依赖发现 |
| `CMakePresets.json` | 机器无关的构建预设 |
| `BUILD_CONFIG_REVIEW.md` | 上一轮的评审文档（状态已更新） |
| `OPTIMIZATION_REPORT.md` | 本日志报告 |

**新增但不入库（gitignore）**：`CMakeUserPresets.json` —— 本机 `local` 预设，含 `D:/proc/...` 路径。换机器时需自行创建，或改用 `-D` 参数。这一点是双仓库保持一致的前提。

**修改**

`.gitignore`、`README.md`、`cmakeMyTest.cmake`、`no_use/run.sh`、`set_shared_lib_path.sh`、`src/CMakeLists.txt`、`src/projectconfig.h.in`、`src/{utilities,normal,boosttest,cppfeatures,EffectiveModernCpp,pymods,pyrun}/CMakeLists.txt`、`src/pyrun/pyrun.cpp`。

> `run.py` 的改动已在第二次提交 `c738bf5` 中完成，本次未再改动。

---

## 7. 验证结果

### 7.1 最终命令与结果

```bash
MSYSTEM=MINGW64 /d/proc/msys64/usr/bin/bash.exe -lic '
cd /d/Pyrad/Gitee/cpp11
cmake --fresh -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH="D:/proc/boost_1_97_0_msys" \
      -DPython3_ROOT_DIR="D:/proc/Python313"
cmake --build build --clean-first -j 4
cd build/bin && PATH="/d/proc/msys64/mingw64/bin:/usr/bin" ./mymainrun.exe
ctest --test-dir ../ --output-on-failure'
```

| 检查项 | 结果 |
| --- | --- |
| 配置 | 退出码 0，打印 `C++14 / Boost 1.92.0 / Python 3.13.7 / Release / build/bin` |
| 构建 | `54/54`，**0 错误** |
| 运行（PATH 仅含 MSYS2 工具链） | **退出码 0**，804 行输出，`retcode=0`，`Finished running Python function: run_main`，无 Traceback |
| `sys.prefix` | `D:/proc/Python313`（标准库路径正确，无 `Could not find platform independent libraries`） |
| `ctest` | `1/1 Passed`，0.03 s |
| `ldd build/bin/mymainrun.exe` | 无 `not found` |
| `compile_commands.json` | `-std=c++14`（无 `gnu++`） |
| `bash -n` 两个脚本 | 通过 |
| `projectconfig.h` | `CPP11_VERSION_MAJOR 0` / `MINOR 1`，5 个 `ENB_*` 宏正确 |

**产物（`build/bin/`，共 12 项）**：`mymainrun.exe`、`lib{utilities,normal,boosttest,cppfeatures,EffectiveModernCpp,pyrun,winmingwrun}.dll`、`cpp11runtest.pyd`、`python313.dll`、`run.py`、`top.py`。导入库位于 `build/lib/`。

### 7.2 回归矩阵

| 组合 | configure | build |
| --- | --- | --- |
| 全开（默认） | 0 | 0 |
| `-DENB_NORMAL=OFF` | 0 | 0 |
| `-DENB_BOOSTTEST=OFF` | 0 | 0 |
| `-DENB_CPP_FEATURES=OFF` | 0 | 0 |
| `-DENB_EFFECTIVE_MODERN_CPP=OFF` | 0 | 0 |
| `-DENB_PYRUN_CPP=OFF` | 0 | 0 |

C++ 标准：`11` 失败（见 7.3）、`14` / `17` / `20` 均通过。`cmake --preset local` 与 `cmake --build --preset local` 均返回 0；错误的 `CMAKE_PREFIX_PATH` 会得到明确的 `Could not find a package configuration file provided by "Boost"`。

### 7.3 已知失败与说明

**C++11 构建失败**（作为有价值的实验结论保留）：

```
src/normal/maintest.cpp:528: error: use of 'auto' in lambda parameter
  declaration only available with '-std=c++14' or '-std=gnu++14'
boost/math/tools/type_traits.hpp: error: 'is_final' / 'remove_cv_t'
  has not been declared in 'std'
```

原因是源码使用泛型 lambda，且 Boost 1.92 本身要求 ≥ C++14。CMake 现在会在选择 `< 14` 时给出明确 `WARNING`，而不是让人去猜编译错误。

### 7.4 警告情况（重要）

启用 `-Wall -Wextra -Wpedantic` 后最终构建有 **143 条警告**：8 条来自 Boost 头文件内联实例化，135 条来自项目既有测试代码（主要是 `printf` 格式串与实参类型不匹配、复合字面量、隐藏虚函数、未初始化成员、`PyModuleDef` 缺初始化项等）。

**这些不是本次优化新引入的缺陷，而是新打开的警告把它们暴露出来了**。与评审文档的建议一致：不加 `-Werror`，构建不受影响，清理留作单独任务。需要安静构建时可用 `-DCPP11_ENABLE_WARNINGS=OFF`（矩阵验证中即使用该开关）。

---

## 8. 遗留项与后续建议

| 优先级 | 事项 | 说明 |
| --- | --- | --- |
| 中 | 清理 135 条项目源码警告 | 其中格式串不匹配属于真实未定义行为，建议逐文件修；可与「验证 C++ 特性」的实验目的结合 |
| 中 | `gen_emcxx_new_chapter.sh` 同步注册 | GLOB 解决了「新增 .cpp 要改 CMakeLists」，但 `effectiveModernCpp.cpp` 里的调用注册仍需手动 |
| 中 | Windows 符号导出宏（`CPP11_API`） | 目前仍依赖 MinGW ld 自动导出全部符号；接 MSVC 前必须补 `__declspec(dllexport)` |
| 低 | `pybind11` 重写 / 归档 `winmingwrun`、`pymods` | D4 未决，需你确认这两个产物是否还在使用 |
| 低 | `no_use/` 旧 Makefile 体系归档 | D9 未执行 |
| 低 | 40+ 处 `#include "../xxx"` 渐进规范化 | D8 选择渐进，当前由统一包含路径兜底 |
| 低 | 双仓库推送 | 评审文档阶段 6.6。本次只做本地 `git commit`，**未执行 `git push`**；推送后需在 Gitee / GitHub 核对 commit hash 一致 |
| 低 | CI | D10 暂不加 |

---

## 9. 回滚方式

```bash
# 回滚本次优化（保留前两次提交）
git revert <第三次提交 hash>

# 回滚段错误修复
git revert c738bf5

# 回滚平台分流
git revert 36d6c06

# 查看这次优化相对上一提交的完整改动
git show --stat <第三次提交 hash>
```

注意：回滚第三次提交后，顶层 `CMakeLists.txt` / Presets 会消失，需同步切回旧命令 `cd build && cmake ../src`，且 `build/` 需重新 `--fresh` 配置。`CMakeUserPresets.json` 不受 git 管理，回滚后仍在本地。

---

## 10. 复现速查

```bash
# 免参数（本机、已配置 CMakeUserPresets.json）
cmake --preset local
cmake --build --preset local -j 4
ctest --preset local --output-on-failure
./build/bin/mymainrun.exe

# 显式参数（任意新克隆）
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH="<boost 安装前缀，需含 lib/cmake/Boost-*/BoostConfig.cmake>" \
      -DPython3_ROOT_DIR="<python 安装前缀>"
cmake --build build -j 4
./build/bin/mymainrun.exe

# 常用开关
cmake --list-presets
cmake -S . -B build -DCPP11_CXX_STANDARD=17
cmake -S . -B build -DENB_NORMAL=OFF
cmake -S . -B build -DCPP11_ENABLE_WARNINGS=OFF
```
