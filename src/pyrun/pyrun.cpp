#include "pyrun.hpp"
#include <Python.h>
#include <iostream>

namespace pyrun {

void pytest() {
    Py_Initialize();
    PyRun_SimpleString("print(\"Hello, world!\")");
    Py_Finalize();
}

void test_all() {
    std::cout << "--- Start running pyrun::test_all ---\n";
    pytest();
    std::cout << "--- End running pyrun::test_all ---\n";
}

} // end namespace pyrun
