#include <iostream>
// #include <stdio.h>

#include "../utilities/utilities.hpp"

#include "effectiveModernCpp.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

void test_template_type_deduction() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);

    // Test T&
    int x = 27;         // x is an int
    const int cx = x;   // cx is a const int
    const int &rx = x;  // rx is a reference to x as a const int

    // Test "T&" as the parameter type
    func_by_ref(x);
    func_by_ref(cx);
    func_by_ref(rx);

    // Test "const T&" as the parameter type
    func_by_ref_const(x);
    func_by_ref_const(cx);
    func_by_ref_const(rx);

    // Test T*
    int y = 23;
    int *p = &y;
    const int *cp = &y;

    // Test "T*" as the parameter type
    func_by_ptr(&y);
    func_by_ptr(p);
    func_by_ptr(cp);

    // Test "const T*" as the parameter type
    func_by_ptr_const(&y);
    func_by_ptr_const(p);
    func_by_ptr_const(cp);

    // Test T&&
    int z = 27;         // z is an int
    const int cz = z;   // cz is a const int
    const int &rz = z;  // rz is a reference to z as a const int

    // Test "T&&" as the parameter type
    func_by_ref_universal(z);
    func_by_ref_universal(cz);
    func_by_ref_universal(rz);
    func_by_ref_universal(27);

    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
} // test_template_type_deduction

} // namespace chapter_1

void test_all() {
    fprintf(stdout, "\n----- BEGIN of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

    chapter_1::test_template_type_deduction();

    fprintf(stdout, "----- END of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

} // test_all

} // namespace effective_mordern_cpp
