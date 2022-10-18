#include "chapter3_item14.hpp"
#include <algorithm>
#include <stdio.h>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_14 {


/**
 * Compared with C++98 style, the potential benefit for declaring
 * a function as "noexcept",
 *
 * 1. Maybe the call stack will not be kept
 * 2. Maybe no need to guarantee the reverse order of destroying objects
 */
int test_f_noexcept_cxx11(const int x) noexcept { // Most optimized
    return x * x;
}
int test_f_noexcept_cxx98(const int x) throw() { // Less optimized
    return x * x;
}
int test_f_may_except(const int x) { // Less optimized
    return x * x;
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Use of no exception functions (C++98 style and C++11 style)
    fprintf(stdout, "Square of %d is %d\n", 10, test_func_no_except_cxx98(10));
    fprintf(stdout, "Square of %d is %d\n", 10, test_func_no_except_cxx11(10));

    // Use of no exception and with-exception functions (C++98 style and C++11 style)
    fprintf(stdout, "Square of %d is %d\n", 10, test_f_noexcept_cxx98(10));
    fprintf(stdout, "Square of %d is %d\n", 10, test_f_noexcept_cxx11(10));
    fprintf(stdout, "Square of %d is %d\n", 10, test_f_may_except(10));

} // test_all

} // namespace item_14

} // namespace chapter_3

} // namespace effective_mordern_cpp


