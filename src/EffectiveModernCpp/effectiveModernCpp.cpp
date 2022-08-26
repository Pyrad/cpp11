#include <iostream>

#include "effectiveModernCpp.hpp"
#include "chapter1_item01.hpp"
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

void test_all() {
    fprintf(stdout, "\n----- BEGIN of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

    chapter_1::item_01::test_template_type_deduction();

    fprintf(stdout, "----- END of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

} // function test_all

} // namespace effective_mordern_cpp
