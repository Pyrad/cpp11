"""
Entry script executed by the embedded CPython interpreter in src/pyrun/pyrun.cpp.

The build copies this file next to the runtime artifacts and executes it with
PyRun_SimpleStringFlags(), which sets __file__ but not sys.path[0]. Paths are
derived from __file__ instead of the old per-machine DEV_NICKNAME dispatch
with hard-coded absolute paths such as D:/Gitee/cpp11/build/pyrun or
/home/pyrad/gitee/cpp11/build/pyrun.
"""

import os
import sys


def add_module_directories(script_dir):
    """Make both the Python module and its native dependencies importable.

    Since Python 3.8 on Windows, LoadLibraryEx no longer searches PATH for an
    extension module's dependent DLLs; os.add_dll_directory() is required.
    Registration is derived from the script location so it works with the
    historical layout (DLLs in build/<module>/) and with the optimized layout
    where every DLL sits next to this script in build/bin/.
    """
    candidates = {os.path.abspath(script_dir)}
    parent = os.path.dirname(os.path.abspath(script_dir))
    for root in (os.path.abspath(script_dir), parent):
        try:
            entries = list(os.scandir(root))
        except OSError:
            continue
        for entry in entries:
            if entry.is_dir():
                candidates.add(entry.path)

    for path in sorted(candidates):
        if hasattr(os, "add_dll_directory"):
            try:
                os.add_dll_directory(path)
            except OSError:
                pass
        if path not in sys.path:
            sys.path.insert(0, path)


def run_main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    add_module_directories(script_dir)

    # cpp11runtest is the Python extension module built by CMake.
    import cpp11runtest

    cpp11runtest.run()
    cpp11runtest.run_normal()
    print("Finished running Python function: run_main")
    return True


if __name__ == "__main__":
    run_main()
