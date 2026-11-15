#include "pyrun.hpp"

#include <Python.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

namespace pyrun {
namespace {

/**
 * @brief Print a string attribute of the sys module.
 *
 * Py_GetPythonHome() and Py_GetProgramName() are deprecated since Python
 * 3.13 and, more importantly, return wchar_t* -- printing them with "%s"
 * interprets a UTF-16 string as a narrow one (garbled output, and undefined
 * behaviour when the value is NULL). Reading the equivalents from sys is
 * both type-safe and forward compatible.
 */
void print_sys_attr(const char *label, const char *name) {
    PyObject *value = PySys_GetObject(name);   // borrowed reference
    if (value == nullptr) {
        fprintf(stdout, "%s: (not available)\n", label);
        return;
    }
    const char *text = PyUnicode_AsUTF8(value);
    if (text == nullptr) {
        PyErr_Clear();
        fprintf(stdout, "%s: (not a string)\n", label);
        return;
    }
    fprintf(stdout, "%s: %s\n", label, text);
}

/**
 * @brief Locate run.py without assuming a particular working directory.
 *
 * CPP11_PYRUN_SCRIPT is supplied by CMake after the optimization that puts
 * all runtime artifacts into a single directory; the relative fallbacks keep
 * this file usable with the historical build layout too.
 */
const char *locate_run_script() {
    static const char *const candidates[] = {
#ifdef CPP11_PYRUN_SCRIPT
        CPP11_PYRUN_SCRIPT,
#endif
        "./pyrun/run.py",
        "./run.py",
        "../pyrun/run.py",
        nullptr,
    };
    for (const char *const *p = candidates; *p != nullptr; ++p) {
        struct stat st;
        if (stat(*p, &st) == 0 && (st.st_mode & S_IFMT) != S_IFDIR) {
            return *p;
        }
    }
    return nullptr;
}

/** @brief Return the directory part of a path. */
std::string dirname_of(const char *path) {
    const std::string s(path);
    const std::string::size_type pos = s.find_last_of("/\\");
    if (pos == std::string::npos) {
        return ".";
    }
    if (pos == 0) {
        return "/";
    }
    return s.substr(0, pos);
}

/**
 * @brief Execute a Python file without handing a FILE* to CPython.
 *
 * This is the fix for the runtime SIGSEGV. The official CPython Windows
 * build is compiled against the UCRT, while a MinGW-built host links
 * msvcrt.dll. FILE is an opaque structure owned by the C runtime that
 * created it: PyRun_SimpleFile() expands to PyRun_SimpleFileExFlags(),
 * which calls fgetc() from ucrtbase.dll on a FILE* produced by msvcrt's
 * fopen(). ucrtbase then dereferences a garbage lock pointer in
 * RtlEnterCriticalSection and the process dies with SIGSEGV:
 *
 *   PyRun_SimpleFileExFlags -> _Py_UniversalNewlineFgetsWithSize
 *     -> ucrtbase!fgetc -> ntdll!RtlEnterCriticalSection -> SIGSEGV
 *
 * Reading the source with C++ streams (our own CRT) and passing CPython a
 * plain NUL-terminated char buffer via PyRun_SimpleStringFlags() removes
 * the CRT boundary completely. The same code works on Linux/WSL.
 */
int run_script_in_memory(const char *fname) {
    std::ifstream in(fname, std::ios::in | std::ios::binary);
    if (!in) {
        fprintf(stdout, "Failed to open file, skip running: %s\n", fname);
        return 1;
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    const std::string source = buffer.str();
    fprintf(stdout, "Read file length: %zu\n", source.size());

    // PyRun_SimpleString* executes in __main__'s globals but, unlike
    // PyRun_SimpleFile(), does not set __file__ or sys.path[0]. Restore both
    // so the script can import the extension modules sitting next to it.
    PyObject *main_module = PyImport_AddModule("__main__");   // borrowed
    if (main_module == nullptr) {
        PyErr_Print();
        return 1;
    }
    PyObject *globals = PyModule_GetDict(main_module);         // borrowed
    PyObject *py_file = PyUnicode_FromString(fname);
    if (py_file != nullptr) {
        PyDict_SetItemString(globals, "__file__", py_file);
        Py_DECREF(py_file);
    } else {
        PyErr_Clear();
    }

    const std::string dir = dirname_of(fname);
    PyObject *py_dir = PyUnicode_FromString(dir.c_str());
    if (py_dir != nullptr) {
        PyObject *sys_path = PySys_GetObject("path");           // borrowed
        if (sys_path != nullptr && PyList_Check(sys_path)) {
            PyList_Insert(sys_path, 0, py_dir);
        }
        Py_DECREF(py_dir);
    } else {
        PyErr_Clear();
    }

    fprintf(stdout, "Running file: %s\n", fname);
    const int retcode = PyRun_SimpleStringFlags(source.c_str(), nullptr);
    fprintf(stdout, "%s running file: %s (retcode=%d)\n",
            retcode == 0 ? "Succeeded" : "Failed", fname, retcode);
    fflush(stdout);
    return retcode;
}

}  // namespace

/**
 * @brief Show basic environmental information
 */
bool show_env_info() {
    fprintf(stdout, "Python version: %s\n", Py_GetVersion());
    print_sys_attr("Python prefix", "prefix");
    fprintf(stdout, "Platform: %s\n", Py_GetPlatform());
    print_sys_attr("Executable", "executable");
    fprintf(stdout, "Copyright: %s\n", Py_GetCopyright());
    fprintf(stdout, "Compiler: %s\n", Py_GetCompiler());
    fprintf(stdout, "Build Info: %s\n", Py_GetBuildInfo());
    return true;
}  // show_env_info

bool run_python_script() {
    const char *nickname = getenv("DEV_NICKNAME");
    if (nickname == nullptr || nickname[0] == '\0') {
        fprintf(stdout, "DEV_NICKNAME is not set; using auto-detected Python\n");
    } else {
        fprintf(stdout, "DEV_NICKNAME = %s\n", nickname);
    }

    // Python home is deliberately left to CPython's own detection. The old
    // code printed a hard-coded WSL path but never actually called
    // Py_SetPythonHome(), which was misleading on Windows/MSYS2.
    Py_Initialize();
    if (!Py_IsInitialized()) {
        fprintf(stderr, "Failed to initialize the Python interpreter\n");
        return false;
    }
    fprintf(stdout, "INFO: Python was initialized by function Py_Initialize()\n");

    PyRun_SimpleString("print(\"===== Start running =====\")");
    show_env_info();

    const char *fname = locate_run_script();
    int retcode = 1;
    if (fname != nullptr) {
        fprintf(stdout, "File exist, start to run: %s\n", fname);
        retcode = run_script_in_memory(fname);
    } else {
        fprintf(stdout, "File NOT found, skip running run.py\n");
    }

    Py_Finalize();
    fflush(stdout);
    return (retcode == 0);
}  // run_python_script

/**
 * @brief For main thread to call
 */
bool pyrun() {
    return run_python_script();
}  // pyrun

}  // namespace pyrun
