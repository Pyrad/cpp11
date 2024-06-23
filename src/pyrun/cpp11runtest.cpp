#include "pyrun/winmingwrun.hpp"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

// #include "object.h"
// #include "pyport.h"
// #include "methodobject.h"
// #include "moduleobject.h"
// #include "longobject.h"
// #include "modsupport.h"

#include <iostream>
#include <stdio.h>

// Must include this to find the following ENB_* macros
// #include "../projectconfig.h"
#include "projectconfig.h"

// ------------------------------------------------------
// The following header files are included conditionally
// based on the macros defined in CMakeLists.txt
// ------------------------------------------------------
#ifdef ENB_NORMAL
#include "../normal/normal.hpp"
#include "../normal/template.hpp"
#endif // ENB_NORMAL

#ifdef ENB_BOOSTTEST
#include "../boosttest/boosttest.hpp"
#endif // ENB_BOOSTTEST

#ifdef ENB_CPP_FEATURES
#include "../cppfeatures/lvalue_rvalue.hpp"
#endif // ENB_CPP_FEATURES

#ifdef ENB_EFFECTIVE_MODERN_CPP
#include "../EffectiveModernCpp/effectiveModernCpp.hpp"
#endif // ENB_EFFECTIVE_MODERN_CPP

#include "../normal/maintest.hpp"

#ifdef ENB_PYRUN_CPP
#include "pyrun.hpp"
#endif // ENB_PYRUN_CPP
// ------------------------------------------------------
// Headers included conditionally end
// ------------------------------------------------------

namespace expose_functions {

/**
 * @brief To show if a function has been executed successfully or not
 *
 * @param[in] success Flag to indicate if a function ran successfully
 * @param[in] function The name of function to show if it ran successfully
 *
 * @return void
 */
void show_compiled_msg(const bool success, const char *funcname) {
    if (success) {
        fprintf(stdout, "[%s] Succeeded to run\n", funcname);
    } else {
        fprintf(stdout, "[%s] Failed to run, code not compiled\n", funcname);
    }
} // show_compiled_msg

/**
 * @brief Run normal test function
 */
int run_normal() {
    bool run_success = false;

#ifdef ENB_NORMAL
    namespace M = mainspace;
    namespace T = template_test;

    namespace N = NORMAL;
    N::tester_constexpr();
    N::normal_test_all();
    M::test_std_regex();

    // To indicate this code snippet has been activated
    run_success = true;
#else
    fprintf(stdout, "Warning: macro %s is not defined, code in function %s not "
                    "compiled\n", "ENB_NORMAL", __FUNCTION__);
#endif // ENB_NORMAL

    show_compiled_msg(run_success, "run_normal");
    return 0;

} // run_normal

/**
 * @brief Run boost test function
 */
int run_boosttest() {
    bool run_success = false;

#ifdef ENB_BOOSTTEST
    namespace B = boost_test;
    B::test_boost_all();
    B::boost_normal_test_all();

    // To indicate this code snippet has been activated
    run_success = true;
#else
    fprintf(stdout, "Warning: macro %s is not defined, code in function %s not "
                    "compiled\n", "ENB_BOOSTTEST", __FUNCTION__);
#endif // ENB_BOOSTTEST

    show_compiled_msg(run_success, "run_boosttest");
    return 0;

} // run_boosttest

/**
 * @brief Run L-value & R-value related function
 */
int run_lvalue_rvalue() {
    bool run_success = false;

#ifdef ENB_CPP_FEATURES
    lvalue_rvalue::test_all();

    // To indicate this code snippet has been activated
    run_success = true;
#else
    fprintf(stdout, "Warning: macro %s is not defined, code in function %s not "
                    "compiled\n", "ENB_CPP_FEATURES", __FUNCTION__);
#endif // ENB_CPP_FEATURES

    show_compiled_msg(run_success, "run_lvalue_rvalue");
    return 0;
} // run_lvalue_rvalue

/**
 * @brief Run function related to Effective Modern C++
 */
int run_effective_modern_cpp() {
    bool run_success = false;

#ifdef ENB_EFFECTIVE_MODERN_CPP
    effective_mordern_cpp::test_all();

    // To indicate this code snippet has been activated
    run_success = true;
#else
    fprintf(stdout, "Warning: macro %s is not defined, code in function %s not "
                    "compiled\n", "ENB_EFFECTIVE_MODERN_CPP", __FUNCTION__);
#endif // ENB_EFFECTIVE_MODERN_CPP

    show_compiled_msg(run_success, "run_lvalue_rvalue");
    return 0;
} // run_effective_modern_cpp


/**
 * @brief A simple function to test various Py_* functions
 *
 * @return True if success, false otherwise
 */
bool run_pytest() {
    Py_Initialize();
    PyRun_SimpleString("print(\"Hello, world!\")");
    Py_Finalize();

    return true;
} // run_pytest


} // namespace expose_functions

static PyObject *
cpp11runtest_run_normal(PyObject *self, PyObject *args) {
    std::cout << "----- " << __FUNCTION__ << "-----\n";
    expose_functions::run_normal();

    return PyLong_FromLong(0);
} // cpp11runtest_run_normal

static PyObject *
cpp11runtest_run_boosttest(PyObject *self, PyObject *args) {
    std::cout << "----- " << __FUNCTION__ << "-----\n";
    expose_functions::run_boosttest();

    return PyLong_FromLong(0);
} // cpp11runtest_run_boosttest

static PyObject *
cpp11runtest_run_lvalue_rvalue(PyObject *self, PyObject *args) {
    std::cout << "----- " << __FUNCTION__ << "-----\n";
    expose_functions::run_lvalue_rvalue();

    return PyLong_FromLong(0);
} // cpp11runtest_run_boosttest

static PyObject *
cpp11runtest_run_effective_modern_cpp(PyObject *self, PyObject *args) {
    std::cout << "----- " << __FUNCTION__ << "-----\n";
    expose_functions::run_lvalue_rvalue();

    return PyLong_FromLong(0);
} // cpp11runtest_run_effective_modern_cpp

static PyObject *
cpp11runtest_run_pytest(PyObject *self, PyObject *args) {
    std::cout << "----- " << __FUNCTION__ << "-----\n";
    expose_functions::run_lvalue_rvalue();

    return PyLong_FromLong(0);
} // cpp11runtest_run_pytest

static PyObject *
cpp11runtest_run_all(PyObject *self, PyObject *args) {
    std::cout << "Running all by cpp11runtest_run_all\n";

    return PyLong_FromLong(0);

} // cpp11runtest_run_all

static PyMethodDef cpp11runtest_methods[] = {
    {"run", cpp11runtest_run_all, METH_VARARGS, "Run tests of cpp11 project."},
    {"run_normal", cpp11runtest_run_normal, METH_VARARGS, "Run test run_normal."},
    {"run_boosttest", cpp11runtest_run_boosttest, METH_VARARGS, "Run test run_boosttest."},
    {"run_lvalue_rvalue", cpp11runtest_run_lvalue_rvalue, METH_VARARGS, "Run test run_lvalue_rvalue."},
    {"run_effective_modern_cpp", cpp11runtest_run_effective_modern_cpp, METH_VARARGS, "Run test run_effective_modern_cpp."},
    {"run_pytest", cpp11runtest_run_pytest, METH_VARARGS, "Run test run_pytest."},
    { NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef cpp11runtest_module = {
    PyModuleDef_HEAD_INIT,
    "cpp11runtest", /* name of module */
    "<This is the doc string of cpp11runtest module>", /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module,
           or -1 if the module keeps state in global variables. */
    cpp11runtest_methods
};

PyMODINIT_FUNC
PyInit_cpp11runtest(void) {
    PyObject *m = nullptr;
    m = PyModule_Create(&cpp11runtest_module);
    if (!m) {
        return nullptr;
    }

    return m;
}
