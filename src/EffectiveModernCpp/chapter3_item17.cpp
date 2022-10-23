#include "chapter3_item17.hpp"
#include <utility>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_17 {


/**
 * A class without any user-defined ctor or dtor, compiler
 * helps us create 6 special member functions (see class ValObj)
 */
void test_compiler_generated_special_member_funcs() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Use compiler generated default ctor
    ValObj v0;
    v0.echo();

    // Use compiler generated copy ctor
    ValObj v1(v0);
    ValObj v2 = v0;
    v1.echo();
    v2.echo();

    // Use compiler generated copy assignment operator
    ValObj v3;
    v3 = v1;

    // Use compiler generated move ctor
    ValObj v4;
    ValObj v5(std::move(v4));
    v5.echo();

    // Use compiler generated move ctor
    ValObj v6, v7;
    v6.set_xy(2.8, 3.9);
    v7 = std::move(v6);
    v7.echo();
} // test_compiler_generated_special_member_funcs

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_compiler_generated_special_member_funcs();
}


} // namespace item_17

} // namespace chapter_3

} // namespace effective_mordern_cpp

