#include "pyrun.hpp"
// #include "pylifecycle.h"
#include <Python.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>

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
    const std::string dev_nickname(getenv("DEV_NICKNAME"));
    if (dev_nickname.empty()) {
        fprintf(stdout, "DEV_NICKNAME does NOT exist\n");
        return false;
    }

    if (dev_nickname == "Asus_Win7_MSYS2") {
        fprintf(stdout, "Error: Platform not supported, DEV_NICKNAME is %s\n", dev_nickname.c_str());
        return false;
    } else if (dev_nickname == "Asus_Win7_VBox_Ubuntu_20_04_LTS") {
        fprintf(stdout, "Error: Platform not supported, DEV_NICKNAME is %s\n", dev_nickname.c_str());
        return false;
    } else if (dev_nickname == "LenovoXiaoXin_Win10_MSYS2") {
        fprintf(stdout, "Error: Platform not supported, DEV_NICKNAME is %s\n", dev_nickname.c_str());
        return false;
    } else if (dev_nickname == "Asus_tuf_gaming_b650m_plus_Win11_WSL_Ubuntu_24_04_LTS") {
	// const char *pyhome = "/home/pyrad/proc/Python-3.13.7/";
	const char *pyhome = "/usr/bin/";
        fprintf(stdout, "INFO: Setting python home path to: %s\n", pyhome);
        Py_SetPythonHome((wchar_t*) pyhome);
        fprintf(stdout, "INFO: Python home path is set to: %s\n", pyhome);
    } else {
        fprintf(stdout, "Error: Unrecognized platform, DEV_NICKNAME is %s\n", dev_nickname.c_str());
        return false;
    }

    // Py_SetPythonHome((wchar_t*)L"D:/procs/msys64/mingw64");

    Py_Initialize();
    fprintf(stdout, "INFO: Python was initialized by function Py_Initialize()\n");

    PyRun_SimpleString("print(\"===== Start running =====\")");
    show_env_info();

    struct stat stat_block;
    const char *fname = "./pyrun/run.py";
    int retcode = 1;
    if (stat(fname, &stat_block) == 0 && !(stat_block.st_mode & S_IFDIR)) {
        fprintf(stdout, "File exist, start to run: %s\n", fname);
#if 0
        // 2024-01-06 21:44
        // This code snippet to run Python would cause segment fault crash, currently I don't
        // know why.
        //
        // 2024-01-06 20:18
        // Mentioned in: https://docs.python.org/3/c-api/veryhigh.html#c.PyRun_SimpleFileExFlags
        // On Windows, the file to execute should be opened as "rb"
        FILE *fp = fopen(fname, "rb");
        if (fp) {
            retcode = PyRun_SimpleFile(fp, fname);
            fclose(fp);
        } else {
            fprintf(stdout, "Failed to open file, skipp running: %s\n", fname);
        }
#endif // 0

#if 1
        // 2024-01-06 21:52
        // Refer to the following URL to see how to read a file into a string.
        // https://www.delftstack.com/howto/cpp/read-file-into-string-cpp/
        std::ifstream infile(fname);
        if (infile.is_open()) {
            std::ostringstream oss;
            oss << infile.rdbuf();
            const std::string &fcont = oss.str();
            fprintf(stdout, "Read file length: %d\n", fcont.size());
            fprintf(stdout, "Running file: %s\n", fname);
            fprintf(stdout, "File content:\n");
            fprintf(stdout, "%s\n", fcont.c_str());
            PyRun_SimpleString(fcont.c_str());
            fprintf(stdout, "Succeeded running file: %s\n", fname);
        } else {
            fprintf(stdout, "Failed to open file: %s\n", fname);
        }
#endif // 0
    } else {
        fprintf(stdout, "File NOT found, skipp running: %s\n", fname);
    }

    Py_Finalize();

    return (retcode == 0);
} // run_python_script

/**
 * @brief For main thread to call
 */
bool pyrun() {
    return run_python_script();
} // pyrun

} // namespace pyrun

