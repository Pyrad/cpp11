#include "chapter5_item25.hpp"
#include <string>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_25 {

uint32_t Foobar::id_cnt = 0;

/**
 * In order to keep its lvalue-ness or rvalue-ness, we use std::move
 * on rvalue reference, and we use std::forward on universal reference
 */
void test_std_move_on_rvalue_ref_std_forward_on_universal_ref() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foobar fb0("sun");
    Foobar fb1(Foobar("moon"));

    std::string ss("sea");

    fb0.set_name(ss);
    fb1.set_name(std::string("ocean"));
}

/**
 * A function to show how to use a universal reference mutiple times
 */
void test_use_universal_ref_mutiple_times() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foobar fb("sun");
    fb.echo();
    // check_set_name uses a universal reference mutiple times
    fb.check_set_name(std::string("ocean"));
    fb.echo();
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_std_move_on_rvalue_ref_std_forward_on_universal_ref();

    test_use_universal_ref_mutiple_times();
}


} // namespace item_25

} // namespace chapter_5

} // namespace effective_mordern_cpp

