#include "pyrun.hpp"
#include <Python.h>
#include <iostream>
#include <stdio.h>

namespace pyrun {

/**
 * @brief Show basic environmental information
 */
bool show_env_info() {
    fprintf(stdout, "Python version: %s\n", Py_GetVersion());
    fprintf(stdout, "Python Home: %s\n", Py_GetPythonHome());
    fprintf(stdout, "Platform: %s\n", Py_GetPlatform());
    fprintf(stdout, "Program Name: %s\n", Py_GetProgramName());
    fprintf(stdout, "Copyright: %s\n", Py_GetCopyright());
    fprintf(stdout, "Compiler: %s\n", Py_GetCompiler());
    fprintf(stdout, "Build Info: %s\n", Py_GetBuildInfo());
    return true;
} // show_env_info

bool run_python_script() {
    Py_Initialize();
    PyRun_SimpleString("print(\"===== Start running =====\")");
    show_env_info();
    Py_Finalize();

    return true;
} // run_python_script

/**
 * @brief For main thread to call
 */
bool pyrun() {
    return run_python_script();
} // pyrun

} // namespace pyrun

