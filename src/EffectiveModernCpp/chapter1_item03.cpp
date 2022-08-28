#include "chapter1_item03.hpp"
#include <stdio.h>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_03 {

/**
 * @brief A simple function to return a temporary std::vector for testing
 *        rvalue use
 *
 * @return A std::vector, which is an rvalue indeed
 *
 */
std::vector<int> get_temp_ivec() {
    std::vector<int> int_vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    return int_vec;
}

/**
 * @brief A function to show how to use a function with a trailing return type
 *        to access a value in a container, by using keyword decltype.
 */
void test_container_access_value() {
    std::vector<int> ivec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    authAndAccess_wtrt(ivec, 0) = 100;

    // Note the following line can't be compiled.
    // As the return value of authAndAccess_wotrt is a new object, due to
    // the reference is removed during auto type deduction, so the return
    // value is no longer a reference to the element in the container, thus
    // assignment to an rvalue is forbidden in C++11.
    // authAndAccess_wotrt(ivec, 1) = 200;

    // This line compiles, as decltype(auto) in C++14 returns exactly the same
    // type of ivec[1] (which is actually a reference to its 2nd element)
    authAndAccess_cxx14_rt(ivec, 1) = 200;

    // "get_temp_ivec" returns an rvalue(vector), and authAndAccess_cxx14_rt_uf
    // returns the 2nd value of that rvalue, then assign it to val
    auto val0 = authAndAccess_cxx14_rt_uf(get_temp_ivec(), 2);
    // The following function can be used in C++11
    auto val1 = authAndAccess_cxx11_rt_uf(get_temp_ivec(), 3);
}

void test_decltype() {
    test_container_access_value();

} // test_decltype


} // namespace item_03

} // namespace chapter_1

} // namespace effective_mordern_cpp

