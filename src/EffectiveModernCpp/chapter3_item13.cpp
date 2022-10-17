#include "chapter3_item13.hpp"
#include <algorithm>
#include <stdio.h>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_13 {

/**
 * In C++98, it's not practical to use const_iterator
 * In C++11, it's more convenient
 */
void test_use_const_iterator_cxx11() {
    std::vector<int> values;

    // Use const_iterator to find element
    auto itr = std::find(values.cbegin(), values.cend(), 1983);
    // Use const_iterator to insert an element after it
    values.insert(itr, 1998);

    // Show elements in "values"
    for (const auto i : values) { printf("i = %d\n", i); }

} // test_use_const_iterator_cxx11


void test_use_non_member_cbegin_cend() {

    std::vector<int> values;

    // Use customized cbeginMyOwn and cendMyOwn to find and insert
    findAndInsert<std::vector<int>>(values, 1983, 1998);

    // Show elements in "values"
    for (const auto i : values) { printf("i = %d\n", i); }
} // test_use_non_member_cbegin_cend

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_use_const_iterator_cxx11();

    test_use_non_member_cbegin_cend();

} // test_all

} // namespace item_13

} // namespace chapter_3

} // namespace effective_mordern_cpp


