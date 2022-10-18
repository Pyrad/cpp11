#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM14
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM14

#include "../utilities/utilities.hpp"
#include <algorithm>
#include <iterator>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_14 {


/**
 * Shows the no except style in C++98
 */
inline int test_func_no_except_cxx98(const int x) throw() {
    return x * x;
}

/**
 * Shows the no except style in C++11
 */
inline int test_func_no_except_cxx11(const int x) throw() {
    return x * x;
}


void test_all();


} // namespace item_14

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM14

