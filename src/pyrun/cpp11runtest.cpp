#define PY_SSIZE_T_CLEAN
#include <Python.h>

// #include "object.h"
// #include "pyport.h"
// #include "methodobject.h"
// #include "moduleobject.h"
// #include "longobject.h"
// #include "modsupport.h"

#include <iostream>

static PyObject *
cpp11runtest_run_all(PyObject *self, PyObject *args) {
    std::cout << "Running all by cpp11runtest_run_all\n";

    return PyLong_FromLong(0);

} // cpp11runtest_run_all

static PyMethodDef cpp11runtest_methods[] = {
    {"run", cpp11runtest_run_all, METH_VARARGS, "Run tests of cpp11 project."},
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
