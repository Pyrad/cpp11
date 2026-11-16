# cpp11 跨模块头文件包含方式整改报告

| 项目 | 内容 |
| --- | --- |
| 报告日期 | 2026-11-16 |
| 触发问题 | `#include "utilities/utilities.hpp"` 编译不通过，必须写成 `#include "../utilities/utilities.hpp"` |
| 结论 | 两种写法都合法，差别只在于「模块根是否在搜索路径上」。已把 `src` 确立为唯一模块根、统一全部写法并完成验证 |
| 改动规模 | 48 个文件（7 个 CMake 配置 + 40 个源文件 + 1 个生成脚本），`+86 / −61` |
| 验证环境 | MSYS2 MINGW64 / CMake 4.0.2 / Ninja 1.12.1 / g++ 15.1.0 / Boost 1.92.0 / CPython 3.13.7 |

---

## 0. 一页速览

- **机制**：`#include "..."` 的搜索顺序是「包含者所在目录 → `-iquote` → `-I` → 系统目录」。
  `../utilities/utilities.hpp` 命中的是第一条（与文件物理位置绑定），`utilities/utilities.hpp` 命中的是第三条（依赖 `-I <repo>/src`）。
- **实测**：整改前当前 `build/` 的编译命令里已经有 `-I D:/Pyrad/Gitee/cpp11/src`，两种写法都能编译通过（2.4 节有证据）。因此「新写法编译失败」在当前配置下无法复现，但它出现的前提很明确——**模块根不在搜索路径上**，2.3 节用隔离实验复现了该失败。
- **配置**：新增 `CPP11_SOURCE_ROOT`（`<repo>/src`）作为唯一跨模块包含根，并加 configure 期自检；6 个模块把自身目录由 `PUBLIC` 收紧为 `PRIVATE`，使 `-I` 列表收敛为「自身目录 + src + build/src」。
- **源码**：51 处 `../模块/文件` / `./模块/文件` 统一为 `模块/文件`（40 个文件），生成脚本 `gen_emcxx_new_chapter.sh` 的模板同步更新。
- **验证**：configure 0，clean build **54/54、0 错误**，零 PATH 运行 **rc=0（804 行、retcode=0）**，`ctest` **1/1**。

---

## 1. 问题描述

`src/EffectiveModernCpp/chapter1_item01.cpp` 里包含工具模块的头文件：

```cpp
#include "../utilities/utilities.hpp"   // 旧写法：能编译
#include "utilities/utilities.hpp"      // 期望写法：报告为编译不通过
```

期望的行为是「以 `src` 为根、按模块名包含」，这样头文件被移动到别的目录时不用改。

## 2. 原因分析

### 2.1 编译器对引号形式 `#include "..."` 的搜索顺序

GCC / Clang（MSVC 的 `/I` 行为一致）：

| 顺序 | 搜索位置 | 由谁决定 |
| --- | --- | --- |
| 1 | **包含这条指令的文件所在目录** | 文件自身的物理位置 |
| 2 | `-iquote` 指定的目录 | 命令行 |
| 3 | `-I` 指定的目录 | 命令行 / CMake 的 `target_include_directories` |
| 4 | 系统标准目录 | 工具链 |

尖括号形式 `<...>` **跳过第 1 步**，只查 2–4。

### 2.2 两种写法的本质差别

| 写法 | 命中位置 | 是否依赖 `-I` | 文件移动后 | 语义 |
| --- | --- | --- | --- | --- |
| `"../utilities/utilities.hpp"` | 第 1 步：相对包含者所在目录 | 否 | **失效**（相对层级变了） | 与物理布局耦合 |
| `"utilities/utilities.hpp"` | 第 3 步：`-I<repo>/src` 下拼出 `src/utilities/utilities.hpp` | 是 | 有效 | 与模块名耦合 |

所以「`../` 能编、`模块/` 不能编」不是写法对错问题，而是**后者缺少一个 `-I` 根**。

### 2.3 隔离实验（可复现）

造一个最小目录 `src/{utilities,EffectiveModernCpp}`，`a.cpp` 用模块写法、`b.cpp` 用相对写法，只给编译器「自身目录」，再逐步加根：

| # | 写法 | `-I` 列表 | 结果 |
| --- | --- | --- | --- |
| 1 | `"utilities/u.hpp"` | 仅 `EffectiveModernCpp` | **fatal error: utilities/u.hpp: No such file or directory** |
| 2 | `"../utilities/u.hpp"` | 仅 `EffectiveModernCpp` | **rc=0**（走第 1 步） |
| 3 | `"utilities/u.hpp"` | `EffectiveModernCpp` + `src` | **rc=0**（走第 3 步） |
| 4 | `"../utilities/u.hpp"` | 同上 | rc=0 |

实验 1 与实验 2 的对比就是本问题的全部原因：**同一份代码、同样的 `-I`，只有相对写法能过，因为相对路径根本不经过 `-I`。**

### 2.4 本项目的实际情况（需要说明）

整改前，`build/compile_commands.json` 里该文件的实际编译命令是：

```
c++.exe -DEffectiveModernCpp_EXPORTS
        -ID:/Pyrad/Gitee/cpp11/src/EffectiveModernCpp
        -ID:/Pyrad/Gitee/cpp11/src
        -ID:/Pyrad/Gitee/cpp11/build/src
        ...
```

其中 `-ID:/Pyrad/Gitee/cpp11/src` 就来自 `cpp11_config` 的 INTERFACE 包含目录，所以**两种写法在当前构建里都能通过**。我先把 `chapter1_item01.cpp` 改成 `utilities/utilities.hpp` 单独编译该目标，结果 rc=0（只有两条与该问题无关的 `unused variable` 警告）。优化前的配置（`git show 36d6c06^:src/EffectiveModernCpp/CMakeLists.txt`）同样有 `PRIVATE "${PROJECT_SOURCE_DIR}"`（当时 `PROJECT_SOURCE_DIR` = `src`），所以历史上这条根也在。

结论：**「新写法编译不通过」不是一个稳定的现状，而是一个条件性结果**。它会在这几种情况下出现：

1. 手工 `g++ -c xxx.cpp` 且没有带 `-I <repo>/src`；
2. IDE / clangd / IntelliSense 没有读取 `compile_commands.json`，或读了旧的构建数据库；
3. 用了别的、未按新布局配置的构建目录——例如仓库里的 `build2/`（`CMAKE_HOME_DIRECTORY=.../src` 的旧布局缓存，且 Boost 指向已不存在的 `boost_1_97_0_msys`）；
4. 新增模块的目标没有链接 `cpp11_config`，因而没继承这条根。

真正的缺陷不是写法，而是**「模块根是隐式约定」**：没有任何地方声明它、也没有任何检查保证它存在，所以换个环境就可能退化成 `No such file or directory`。本次整改就是把这条约定变成显式、可校验的契约。

## 3. 编译配置的改动

### 3.1 把模块根显式化，并在配置期自检

`src/CMakeLists.txt`：

```cmake
set(CPP11_SOURCE_ROOT "${PROJECT_SOURCE_DIR}/src")
if(NOT EXISTS "${CPP11_SOURCE_ROOT}/utilities/utilities.hpp")
    message(FATAL_ERROR
        "[cpp11] ${CPP11_SOURCE_ROOT}/utilities/utilities.hpp is missing, so "
        "CPP11_SOURCE_ROOT is wrong and quoted includes such as "
        "\"utilities/utilities.hpp\" would fail to compile.")
endif()

add_library(cpp11_config INTERFACE)
target_include_directories(cpp11_config INTERFACE
    $<BUILD_INTERFACE:${CPP11_SOURCE_ROOT}>        # "utilities/utilities.hpp" etc.
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>) # generated projectconfig.h
```

两点变化：

- 原来写的是 `${CMAKE_SOURCE_DIR}/src`，改为 `${PROJECT_SOURCE_DIR}/src`。二者在本仓库等价（根 `CMakeLists.txt` 有 `project()`），但 `PROJECT_SOURCE_DIR` 表示「本工程」而非「最外层」，将来把 `src` 作为子工程嵌入时不会错。
- 增加 `FATAL_ERROR` 自检：根设错时在 **configure 阶段**就报错并说明后果，而不是等到编译期看到几十条 `No such file or directory`。这也是本次问题的直接防护——下次任何重构改坏了根，错误信息会直接指向 `CPP11_SOURCE_ROOT`。

### 3.2 各模块的自身目录由 `PUBLIC` 收紧为 `PRIVATE`

| 文件 | 改动 |
| --- | --- |
| `src/utilities/CMakeLists.txt` | `target_include_directories(utilities PUBLIC ...)` → `PRIVATE` |
| `src/normal/CMakeLists.txt` | 同上（`normal`） |
| `src/boosttest/CMakeLists.txt` | 同上（`boosttest`） |
| `src/cppfeatures/CMakeLists.txt` | 同上（`cppfeatures`） |
| `src/EffectiveModernCpp/CMakeLists.txt` | 同上（`EffectiveModernCpp`） |
| `src/pyrun/CMakeLists.txt` | 同上（`pyrun`） |

`PUBLIC` 会把模块自身目录传播给所有下游目标。结果就是 `mymainrun` 曾经同时获得 6 个模块目录，可以写 `#include "utilities.hpp"` 或 `#include "chapter1_item01.hpp"` 这种裸名——能编，但**看不出文件属于哪个模块**，一旦不同模块出现同名头文件就会互相遮蔽。改成 `PRIVATE` 后：

- 模块内部仍可用裸文件名（`#include "chapter1_item01.hpp"`），靠引号搜索的「包含者所在目录」这一条即可，不需要 `-I`；
- 跨模块只能用 `模块/文件.hpp`，由 `CPP11_SOURCE_ROOT` 提供唯一入口；
- 写法由编译器强制，不依赖人工约定。

### 3.3 效果：编译命令行收敛

整改前后各目标的 `-I` 列表（`compile_commands.json` 实测）：

| 目标 | 整改前 | 整改后 |
| --- | --- | --- |
| `mymainrun` | `src/EffectiveModernCpp, src/boosttest, src/cppfeatures, src/normal, src/pyrun, src/utilities, src, build/src` | **`src, build/src`** |
| `cpp11runtest` / `winmingwrun` | 6 个模块目录 + `src` + `build/src` | **`src, build/src`** |
| `utilities` / `normal` / `boosttest` / `cppfeatures` / `EffectiveModernCpp` / `pyrun` | 自身目录 + `src` + `build/src`（自身目录还向外传播） | 自身目录（`PRIVATE`）+ `src` + `build/src` |

`mymainrun` 从 8 条 `-I` 降到 2 条，说明「模块可见性」不再靠 include 路径兜底，而是由链接关系表达。

## 4. 源码写法的统一

### 4.1 替换规则

| 场景 | 规范写法 | 例子 |
| --- | --- | --- |
| 跨模块包含 | `模块/头文件.hpp` | `#include "utilities/utilities.hpp"` |
| 同模块内包含 | 裸文件名 | `#include "chapter1_item01.hpp"` |
| CMake 生成的头文件 | 裸文件名 | `#include "projectconfig.h"` |
| 第三方库 | 尖括号 | `#include <boost/type_index.hpp>` |

不使用 `../`，也不使用 `<utilities/utilities.hpp>`（尖括号语义上表示外部依赖，且同样依赖 `-I`，但会失去 IDE 对项目内头文件的优先索引）。

### 4.2 改动清单

| 位置 | 处数 | 说明 |
| --- | --- | --- |
| `src/EffectiveModernCpp/*.hpp`、`*.cpp` | 37 | 全部为 `../utilities/utilities.hpp`（36 处由脚本批量替换，`chapter1_item01.cpp` 先行手工验证 1 处） |
| `src/pyrun/cpp11runtest.cpp` | 6 | `../normal/normal.hpp`、`../normal/template.hpp`、`../boosttest/boosttest.hpp`、`../cppfeatures/lvalue_rvalue.hpp`、`../EffectiveModernCpp/effectiveModernCpp.hpp`、`../normal/maintest.hpp` |
| `src/pyrun/winmingwrun.cpp` | 6 | 同上六项 |
| `src/main.cpp` | 2 | `"./normal/template.hpp"`、`"./pyrun/pyrun.hpp"` → 去掉 `./` |
| **合计** | **51 处 / 40 个文件** | 另有 2 行注释里的 `../projectconfig.h` 一并规范（不参与编译） |

批量替换用一次性脚本完成，规则严格限定为 `#include "\.\./(\w+)/([^"]+)"` 与 `#include "\./(\w+)/([^"]+)"` 两种模式，不触碰其它字符串。

### 4.3 生成模板同步

`gen_emcxx_new_chapter.sh` 里写新章节头文件的 heredoc 也用的是旧写法，已改为：

```cpp
#include "utilities/utilities.hpp"
```

否则以后每生成一个新章节都会重新引入旧写法，本次整改就会失效。

## 5. 验证结果

```bash
MSYSTEM=MINGW64 /d/proc/msys64/usr/bin/bash.exe -lic '
cd /d/Pyrad/Gitee/cpp11
cmake --preset local --fresh
cmake --build build --clean-first -j 4
cd build/bin && PATH="/d/proc/msys64/mingw64/bin:/usr/bin" ./mymainrun.exe
ctest --preset local --output-on-failure'
```

| 检查项 | 结果 |
| --- | --- |
| configure | rc=0，正常打印 `C++14 / Boost 1.92.0 / Python 3.13.7 / build/bin`，自检未触发 |
| clean build | **54/54，0 错误**，143 条既有警告（与整改前完全一致，未新增） |
| 零 PATH 运行 | **rc=0**，804 行输出，`sys.prefix=D:/proc/Python313`，脚本 `retcode=0`，无 Traceback |
| `ctest` | **1/1 Passed** |
| 仓库内 `#include "../` | 0 处（仅剩 2 行已规范的注释） |
| 模块根写法 | 51 处跨模块包含全部为 `模块/文件.hpp` |

143 条警告全部是既有代码的 `printf` 格式串 / 未使用变量等问题（详见 `OPTIMIZATION_REPORT.md` 7.4 节），本次未新增也未尝试清理。

## 6. 后续必须遵守的约定

1. **跨模块包含只用 `模块/头文件.hpp`**，不写 `../`，不依赖别的模块的 `-I`。
2. **新增模块**时：`target_link_libraries(<target> ... cpp11_config)`，且**不要再把自身目录设成 `PUBLIC`**。
3. 同模块内部用裸文件名即可，无需改 CMake。
4. 若将来把 `src` 整体挪位置，只需改 `CPP11_SOURCE_ROOT` 一处；改错会立刻在 configure 阶段报 `FATAL_ERROR`。
5. `gen_emcxx_new_chapter.sh` 已同步为正确模板，新增章节无需再手改 include。

## 7. 回滚方式

```bash
# 回滚本次整改（源码写法 + 配置 + 报告）
git revert <本次提交 hash>
```

回滚后 `../` 写法恢复，`cpp11_config` 回到 `${CMAKE_SOURCE_DIR}/src`，各模块自身目录恢复 `PUBLIC`；构建需 `cmake --preset local --fresh` 重新配置。由于旧写法本身也能编译，回滚不会导致构建失败。
