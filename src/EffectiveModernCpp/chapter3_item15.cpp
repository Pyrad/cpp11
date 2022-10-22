#include "chapter3_item15.hpp"
#include "../utilities/utilities.hpp"
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_15 {

/**
 * @brief constexpr on functions
 *
 * A function declared as "constexpr", can mean 2 things,
 *
 * (1) If it is used in a context where a compile-time value is needed,
 *     then the argument(s) of it must be compile-time value(s) too,
 *     otherwise compile fails
 * (2) If it is used in a context where a run time value is needed,
 *     then the argument(s) of it can be either runtime value or
 *     compile-time value(s)
 *
 * @note, in C++11, only one return statement is allowed, in C++14,
 *        this limitation is loosed, which means you can write more than
 *        one statements in the constexpr function, and then return
 */
constexpr int test_func_get_compile_time_value(const int x, const int y) {
    if (x > 0) {
        return x * y;
    } else {
        return x + y;
    }
}

/**
 * @brief Show a compile-time value is needed for a constexpr function when
 *        it is in a context where a compile-time return value is needed.
 *        If it's not in a compile-time context, the arguments can be either
 *        compile-time values or runtime values.
 */
void test_use_constexpr_func() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // 2 runtime arguments
    const int x = 10;
    const int y = 20;
    // Since here a compile-time value is not needed, then the arguments of
    // a constexpr function can be either runtime or compile-time arguments
    const int rval = test_func_get_compile_time_value(x, y);
    fprintf(stdout, "rval = %d\n", rval);

    // A compile-time value
    constexpr const int a = 15;
    constexpr const int b = 35;
    constexpr const int z = 50;
    // Since here the template argument is a literal type, and it must be known
    // during compile-time, so the arguments of the constexpr function must be
    // compile-time values
    const int rval2 = test_func_use_int_template<test_func_get_compile_time_value(a, b)>(z);
    fprintf(stdout, "rval2 = %d\n", rval2);

} // test_use_constexpr_func

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_use_constexpr_func();

}

} // namespace item_15

} // namespace chapter_3

} // namespace effective_mordern_cpp
