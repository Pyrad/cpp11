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

/**
 * See function Foobar::add_name
 *
 * When to use std::move on function's return value?
 * -------------------------------------------------
 * (1) The form is return-by-value
 * (2) An rvalue reference is returned, or the universal
 *     reference passed in is returned.
 */
void test_use_move_forward_on_return_values() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Foobar fa("sun");
    Foobar fb("sea");
    Foobar fc(Foobar::add_name(Foobar("sun"), fb));
    fc.echo();
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_std_move_on_rvalue_ref_std_forward_on_universal_ref();

    test_use_universal_ref_mutiple_times();

    test_use_move_forward_on_return_values();
}


} // namespace item_25

} // namespace chapter_5

} // namespace effective_mordern_cpp

