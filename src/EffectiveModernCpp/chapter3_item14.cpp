#include "chapter3_item14.hpp"
#include <algorithm>
#include <stdio.h>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_14 {


void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Use of no exception functions (C++98 style and C++11 style)
    fprintf(stdout, "Square of %d is %d\n", 10, test_func_no_except_cxx98(10));
    fprintf(stdout, "Square of %d is %d\n", 10, test_func_no_except_cxx11(10));

} // test_all

} // namespace item_14

} // namespace chapter_3

} // namespace effective_mordern_cpp


