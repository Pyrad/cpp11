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

/**
 * Use atomic instead of mutex to improve performance
 */
void test_use_atomic_to_improve_performance() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    AtomicPoint p(3, 4);
    fprintf(stdout, "Distancef from p to origin: %.3f\n", p.dist_from_origin());
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_set_mutable_member_in_const_member_func();

    test_use_atomic_to_improve_performance();
}


} // namespace item_16

} // namespace chapter_3

} // namespace effective_mordern_cpp

