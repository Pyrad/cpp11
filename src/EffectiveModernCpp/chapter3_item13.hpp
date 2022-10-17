#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM13
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM13

#include "../utilities/utilities.hpp"
#include <algorithm>
#include <iterator>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_13 {

/**
 * In C++98, it's not practical to use const_iterator
 * In C++11, it's more convenient
 */
void test_use_const_iterator_cxx11();

/**
 * C++11 provided non-member functions for: begin(), end(), but
 * C++11 didn't provide non-member functions for: cbegin(), cend(),
 * rbegin(), rend(), crbegin(), crend(), so the following template
 * can't compile
 *
 * ----------------------------------------------------------
 * template<typename C, typename V>
 * void findAndInsert(C &container, const V &targetVal, const V &insertVal) {
 *     using std::cbegin;
 *     using std::cend;
 *     // Non-member function cbegin() and cend() were not provided by C++
 *     auto itr = std::find(cbegin(container), cend(container), targetVal);
 *     container.insert(itr, insertVal);
 * }
 * ----------------------------------------------------------
 *
 * So in order to use cbegin and cend in C++11, we should write our own cbegin()
 * and cend().
 *
 * There are 2 things to notice for the below implemantation.
 * 1. "container" might be a container-like data structure, so maybe it does have
 *    a member function "begin", but probably it doesn't have a member function
 *    "cbegin()", so here "cbegin" member function is not used
 * 2. When "std::begin" is applied to a "const" object, it results in a const_iterator 
 */
template<typename C>
auto cbeginMyOwn(const C &container) -> decltype(std::begin(container)) {
    return std::begin(container);
}
template<typename C>
auto cendMyOwn(const C &container) -> decltype(std::end(container)) {
    return std::end(container);
}

template<typename C, typename V>
void findAndInsert(C &container, const V &targetVal, const V &insertVal) {
    using std::cbegin;
    using std::cend;
    // Non-member function cbegin() and cend() were not provided by C++
    auto itr = std::find(cbegin(container), cend(container), targetVal);
    container.insert(itr, insertVal);
}


void test_use_non_member_cbegin_cend();






void test_all();


} // namespace item_13

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM13

