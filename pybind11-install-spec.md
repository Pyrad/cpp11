# pybind11 3.1.0 安装与验证说明

本文记录 `pybind11 3.1.0` 从本地 release 包安装到 `D:\proc\pybind11` 的完整过程、所用工具、注意事项，以及后续可手动复现的验证步骤。

---

## 1. 安装方式结论

**pybind11 是 header-only（纯头文件）库，不需要编译出 `.lib` / `.dll` / `.so` 形式的独立库文件。**

这一点可由源码包内 `README.rst` 确认：

> pybind11 is a lightweight header-only library that exposes C++ types in Python and vice versa …

因此所谓“安装”本质上是把以下内容放到目标目录：

1. C++ 头文件（`include/pybind11/*.h`）
2. 构建系统集成文件（CMake config、pkg-config `.pc`）
3. Python 帮助包（提供 `import pybind11`、`pybind11-config`、`get_include()` 等）

真正需要编译的是**使用者自己编写的 C++ 扩展模块**，那是使用阶段的事情，与本次安装无关。

本次采用的方式是：**从本地源码 zip 出发，用 pip 构建 wheel 并以 `--target` 安装到指定目录**。相比手工解压，这种方式会自动生成 CMake / pkg-config 配置，并留下标准的 dist-info 元数据，便于后续管理。

---

## 2. 环境与所用工具

| 工具 | 版本 | 作用 |
| --- | --- | --- |
| Python | 3.13.14（位于 `C:\Users\Pyrad\.workbuddy\binaries\python\versions\3.13.12`） | 运行 pip、导入 pybind11、加载测试扩展 |
| pip | 26.1.2 | 从本地 zip 构建并安装，`--target` 指定目标目录 |
| scikit-build-core | 1.1.2 | pybind11 的 PEP 517 构建后端（pip 构建时自动安装） |
| CMake | 4.4.5 | 由 scikit-build-core 构建环境以 wheel 形式提供，非系统全局安装 |
| Ninja | 从 PyPI 临时安装 | 作为 CMake 生成器；系统无 `nmake` / `cl`，必须显式指定 |
| MinGW-w64 g++ | 15.2.0 | 仅用于验证，编译最小 C++ 扩展模块 |

> 说明：Ninja 只安装在系统临时目录中用于本次构建，安装完成后已删除；本机全局 Python 环境未安装 pybind11，也未安装 Ninja / CMake。

---

## 3. 实际安装步骤

### 3.1 检查源码包

确认包内包含 `include/pybind11/`、`pybind11/`（Python 包）、`pyproject.toml`、`tools/` 等关键目录：

```bash
unzip -l "D:/download/pybind11-3.1.0.zip" | head -40
```

`pyproject.toml` 中指定了构建后端：

```toml
[build-system]
requires = ["scikit-build-core >=0.11.2"]
build-backend = "scikit_build_core.build"
```

### 3.2 首次尝试（失败，供参考）

```bash
python -m pip install --target "D:/proc/pybind11" "D:/download/pybind11-3.1.0.zip"
```

失败原因：CMake 在 Windows 上自动选择了 **NMake Makefiles** 生成器，而本机没有 `nmake`，报错：

```
' nmake ' -?  failed with: no such file or directory
CMake Error: CMAKE_CXX_COMPILER not set, after EnableLanguage
```

### 3.3 临时准备 Ninja 并重新安装

```bash
# 1) 在临时目录安装 ninja（记录输出的 TMPD 路径）
TMPD=$(mktemp -d)
python -m pip install --quiet --target "$TMPD/deps" ninja
echo "$TMPD"

# 2) 显式指定 Ninja 生成器后重新安装
#    下例中的 TMPD 请替换为上一步实际输出的路径
NINJADIR="<TMPD>/deps/bin"
PATH="$NINJADIR:$PATH" \
CMAKE_GENERATOR=Ninja \
CMAKE_MAKE_PROGRAM="<TMPD>/deps/bin/ninja.exe" \
python -m pip install --target "D:/proc/pybind11" "D:/download/pybind11-3.1.0.zip"

# 3) 清理临时构建工具
rm -rf "<TMPD>"
```

本次执行时 `<TMPD>` 实际为 `C:\Users\Pyrad\AppData\Local\Temp/tmp.w7srHNzvmU`，该目录已清理。

### 3.4 安装成功标志

pip 输出应包含：

```
Successfully built pybind11
Successfully installed pybind11-3.1.0
```

---

## 4. 安装结果目录结构

```
D:\proc\pybind11\
├─ bin\
│  └─ pybind11-config.exe              # 命令行查询工具
├─ pybind11\                            # Python 包（注意这一层！）
│  ├─ __init__.py
│  ├─ __main__.py
│  ├─ setup_helpers.py
│  ├─ include\
│  │  └─ pybind11\
│  │     └─ pybind11.h                 # 核心头文件，其余头文件同级/子目录
│  └─ share\
│     ├─ cmake\pybind11\
│     │  ├─ pybind11Config.cmake
│     │  ├─ pybind11ConfigVersion.cmake
│     │  ├─ pybind11Targets.cmake
│     │  └─ pybind11Common.cmake 等
│     └─ pkgconfig\
│        └─ pybind11.pc
└─ pybind11-3.1.0.dist-info\            # 版本元数据，勿删
```

关键信息：

- 头文件根目录：`D:\proc\pybind11\pybind11\include`
- CMake 配置目录：`D:\proc\pybind11\pybind11\share\cmake\pybind11`
- pkg-config 文件：`D:\proc\pybind11\pybind11\share\pkgconfig\pybind11.pc`

---

## 5. 使用方式

### 5.1 Python

由于是 `--target` 安装，不在全局 `site-packages` 中，使用时必须把安装目录加入 `PYTHONPATH`：

```bash
# Git Bash
PYTHONPATH="D:\\proc\\pybind11" python -c "import pybind11; print(pybind11.get_include())"

# Windows cmd
set PYTHONPATH=D:\proc\pybind11;%PYTHONPATH%
```

### 5.2 CMake

由于轮子的布局是 `pybind11/include`、`pybind11/share/cmake/pybind11`，`CMAKE_PREFIX_PATH` 要指向**内层**的 `pybind11` 目录：

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=D:/proc/pybind11/pybind11
```

```cmake
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(my_module my_module.cpp)
# 或：target_link_libraries(my_module PRIVATE pybind11::module)
```

### 5.3 直接调用 g++

用 `pybind11-config --includes` 获取所需 include 参数：

```bash
pybind11-config --includes
# 输出示例：
# -IC:\Users\Pyrad\.workbuddy\binaries\python\versions\3.13.12\Include -ID:\proc\pybind11\pybind11\include
```

---

## 6. 注意事项

1. **不需要编译 pybind11 库本身**：它是 header-only，安装阶段只是放置头文件与配置。
2. **`--target` 安装不是全局安装**：新开终端后若要 `import pybind11`，需重新设置 `PYTHONPATH`。
3. **CMake 前缀路径多一层**：是 `D:/proc/pybind11/pybind11`，不是 `D:/proc/pybind11`。这是 wheel 布局（`wheel.install-dir = "pybind11"`）决定的，写错会导致 `find_package` 失败。
4. **保留 `pybind11-3.1.0.dist-info`**：pip 依赖它识别版本，删除后可能造成管理混乱。
5. **本机构建环境缺 `nmake` / `cl`**：日后若需重新构建/重装，必须继续显式指定 Ninja 生成器（见 3.3），否则会重演 NMake 报错。
6. **MinGW 编译扩展的坑**：用本机 g++ 编译 `.pyd` 时必须加 `-static-libgcc -static-libstdc++`，否则生成的模块会因缺少 `libgcc_s_seh-1.dll` / `libstdc++-6.dll` 而报 `ImportError: DLL load failed`。
7. **Python 扩展后缀**：本机为 `.cp313-win_amd64.pyd`，测试模块命名需与之匹配。
8. **验证文件已清理**：安装目录中曾临时创建 `_verify\` 用于编译测试，验证完成后已删除，不属于安装内容。

---

## 7. 手动验证方法（可复现）

以下三步按依赖顺序执行，全部通过即可确认安装成功。

### 7.1 验证 Python 包与头文件路径

```bash
PYTHONPATH="D:\\proc\\pybind11" python -c "import pybind11; print('版本:', pybind11.__version__); print('头文件:', pybind11.get_include())"
```

**预期输出：**

```
版本: 3.1.0
头文件: D:\proc\pybind11\pybind11\include
```

同时确认该目录下存在 `pybind11.h`：

```bash
ls "D:/proc/pybind11/pybind11/include/pybind11/pybind11.h"
```

### 7.2 验证命令行工具

```bash
# 先把 bin 加入 PATH
PATH="/d/proc/pybind11/bin:$PATH"

pybind11-config --version
pybind11-config --cmakedir
pybind11-config --includes
```

**预期输出：**

```
3.1.0
D:\proc\pybind11\pybind11\share\cmake\pybind11
-IC:\Users\Pyrad\.workbuddy\binaries\python\versions\3.13.12\Include -ID:\proc\pybind11\pybind11\include
```

### 7.3 编译并运行一个最小 C++ 扩展（端到端验证）

这是最有力的验证：直接证明头文件可用、能编译出 Python 模块并能被导入调用。

**步骤 1：创建测试源码** `D:\proc\pybind11\example.cpp`

```cpp
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int a, int b) { return a + b; }

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 smoke test";
    m.def("add", &add, "Add two integers");
}
```

**步骤 2：编译为 `.pyd`**

```bash
PY="C:/Users/Pyrad/.workbuddy/binaries/python/versions/3.13.12"

g++ -O0 -shared -std=c++17 -fPIC -static-libgcc -static-libstdc++ \
  -I"D:/proc/pybind11/pybind11/include" \
  -I"$PY/include" \
  "D:/proc/pybind11/example.cpp" \
  -L"$PY/libs" -lpython313 \
  -o "D:/proc/pybind11/example.cp313-win_amd64.pyd"
```

编译应无报错。可用 `objdump -p example.cp313-win_amd64.pyd | grep "DLL Name"` 检查依赖，预期只看到 `python313.dll`、`KERNEL32.dll`、`msvcrt.dll`（若有 `libgcc_s_seh-1.dll` 或 `libstdc++-6.dll` 说明漏加静态链接参数）。

**步骤 3：导入并调用**

```bash
PYTHONPATH="D:\\proc\\pybind11" python -c "import example; print('example.add(2, 3) =', example.add(2, 3)); print(example.__doc__)"
```

**预期输出：**

```
example.add(2, 3) = 5
pybind11 smoke test
```

**步骤 4：清理测试文件**

```bash
rm -f "D:/proc/pybind11/example.cpp" "D:/proc/pybind11/example.cp313-win_amd64.pyd"
```

### 7.4 （可选）CMake find_package 验证

本机没有全局 CMake，因此安装时未实际执行此步。若后续安装了 CMake，可创建最小工程验证：

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(pybind11_check)
find_package(pybind11 CONFIG REQUIRED)
message(STATUS "pybind11 found: ${pybind11_VERSION}")
```

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=D:/proc/pybind11/pybind11
```

预期输出中包含 `pybind11 found: 3.1.0`。若报找不到包，先检查 `CMAKE_PREFIX_PATH` 是否误写成了外层 `D:/proc/pybind11`。

---

## 8. 故障排查速查

| 现象 | 原因 | 处理 |
| --- | --- | --- |
| `pip install` 报 `nmake failed` / `CMAKE_CXX_COMPILER not set` | CMake 选中 NMake 生成器，本机无 nmake | 临时安装 Ninja，设置 `CMAKE_GENERATOR=Ninja` 与 `CMAKE_MAKE_PROGRAM` 后重试 |
| `ImportError: DLL load failed while importing example` | MinGW 运行时 DLL 不在 PATH | g++ 增加 `-static-libgcc -static-libstdc++` 重新编译 |
| `ModuleNotFoundError: No module named 'pybind11'` | 未设置 `PYTHONPATH` | 将 `D:\proc\pybind11` 加入 `PYTHONPATH` |
| CMake `find_package(pybind11)` 失败 | `CMAKE_PREFIX_PATH` 指向了外层目录 | 改为 `D:/proc/pybind11/pybind11` |
| `pybind11-config` 找不到命令 | `D:\proc\pybind11\bin` 不在 PATH | 把该目录加入 PATH，或用完整路径调用 |
