#include "winmingwrun.hpp"

// Must include this to find the following ENB_* macros
// #include "../projectconfig.h"

#include <Python.h>
#include <iostream>

#include "../normal/normal.hpp"
#include "../normal/template.hpp"
#include "../boosttest/boosttest.hpp"
#include "../cppfeatures/lvalue_rvalue.hpp"
#include "../EffectiveModernCpp/effectiveModernCpp.hpp"
#include "../normal/maintest.hpp"

// // ------------------------------------------------------
// // The following header files are included conditionally
// // based on the macros defined in CMakeLists.txt
// // ------------------------------------------------------
// #ifdef ENB_NORMAL
// #include "../normal/normal.hpp"
// #endif // ENB_NORMAL
// 
// #ifdef ENB_BOOSTTEST
// #include "../boosttest/boosttest.hpp"
// #endif // ENB_BOOSTTEST
// 
// #ifdef ENB_CPP_FEATURES
// #include "../cppfeatures/lvalue_rvalue.hpp"
// #endif // ENB_CPP_FEATURES
// 
// #ifdef ENB_EFFECTIVE_MODERN_CPP
// #include "../EffectiveModernCpp/effectiveModernCpp.hpp"
// #endif // ENB_EFFECTIVE_MODERN_CPP
// 
// #include "../normal/maintest.hpp"

// ------------------------------------------------------
// Headers included conditionally end
// ------------------------------------------------------

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

// int __stdcall run_normal() { return 0; }
// int __stdcall run_boosttest() { return 0; }
// int __stdcall run_lvalue_rvalue() { return 0; }
// int __stdcall run_effective_modern_cpp() { return 0; }

int __stdcall run_normal() {
    bool run_success = false;

    namespace M = mainspace;
    namespace T = template_test;

    namespace N = NORMAL;
    N::tester_constexpr();
    N::normal_test_all();
    M::test_std_regex();

    // To indicate this code snippet has been activated
    run_success = true;

    show_compiled_msg(run_success, "run_normal");
    return 0;

} // run_normal

int __stdcall run_boosttest() {
    bool run_success = false;

    namespace B = boost_test;
    B::test_boost_all();
    B::boost_normal_test_all();

    // To indicate this code snippet has been activated
    run_success = true;

    show_compiled_msg(run_success, "run_boosttest");
    return 0;

} // run_boosttest

int __stdcall run_lvalue_rvalue() {
    bool run_success = false;

    lvalue_rvalue::test_all();

    // To indicate this code snippet has been activated
    run_success = true;

    show_compiled_msg(run_success, "run_lvalue_rvalue");
    return 0;
} // run_lvalue_rvalue

int __stdcall run_effective_modern_cpp() {
    bool run_success = false;

    effective_mordern_cpp::test_all();

    // To indicate this code snippet has been activated
    run_success = true;

    show_compiled_msg(run_success, "run_lvalue_rvalue");
    return 0;
} // run_effective_modern_cpp


/**
 * @brief A simple function to test various Py_* functions
 *
 * @return True if success, false otherwise
 */
bool __stdcall run_pytest() {
    Py_Initialize();
    PyRun_SimpleString("print(\"Hello, world!\")");
    Py_Finalize();

    return true;
} // run_pytest


