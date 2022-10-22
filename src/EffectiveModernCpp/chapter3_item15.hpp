#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM15
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM15

#include "../utilities/utilities.hpp"

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
constexpr int test_func_get_compile_time_value(const int x, const int y);

/**
 * @brief A function to show a constexpr function can be used
 *        as the literal template argument
 */
template<int K>
int test_func_use_int_template(const int z) {
    if (K == 0) {
        return K + z;
    } else {
        return K - z;
    }
}

/**
 * @brief Show a compile-time value is needed for a constexpr function when
 *        it is in a context where a compile-time return value is needed.
 *        If it's not in a compile-time context, the arguments can be either
 *        compile-time values or runtime values.
 */
void test_use_constexpr_func();

void test_all();

} // namespace item_15

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM15
