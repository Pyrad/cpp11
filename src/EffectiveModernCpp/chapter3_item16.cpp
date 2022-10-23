#include "chapter3_item16.hpp"
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_16 {

/**
 * @brief Shows that a data member can be changed in 'const' member function
 *        if this data member is mutable
 */
void test_set_mutable_member_in_const_member_func() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Polynomial p;
    fprintf(stdout, "Polynomial object root size : %lu\n", p.roots().size());
}

void test_all() {
    test_set_mutable_member_in_const_member_func();
}


} // namespace item_16

} // namespace chapter_3

} // namespace effective_mordern_cpp

