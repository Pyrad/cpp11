#include <iostream>
// #include <stdio.h>

#include "effectiveModernCpp.hpp"

namespace effective_mordern_cpp {

void test_template_type_deduction() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
}

void test_all() {
    fprintf(stdout, "\n----- BEGIN of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

    test_template_type_deduction();

    fprintf(stdout, "----- END of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

} // test_all

} // namespace effective_mordern_cpp
