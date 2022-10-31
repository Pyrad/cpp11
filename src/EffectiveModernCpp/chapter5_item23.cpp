#include "chapter5_item23.hpp"
#include <string>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_23 {

uint32_t Foo::id_cnt = 0;

/**
 * Use my own version of std::move to move something
 */
void test_my_own_version_move() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foo f0("sun");
    Foo f1("moon");
    Foo f2(f0);                 // copy constructed
    Foo f3(my_move_cxx11(f0));  // move constructed
    Foo f4(my_move_cxx14(f1));  // move constructed

    f2.echo();
    f3.echo();
    f4.echo();
}

/**
 * Since the argument of the Annotation's ctor is an lvalue "const Foo" type,
 * after std::move, it becomes an rvalue "const Foo" type, but Foo's constructor
 * doesn't have a overload with "const Foo" type (just ctor with const lvalue
 * reference, and rvalue reference), so here the const rvalue will be bound to
 * the const lvalue reference, thus the Foo's copy constructor is called,
 * not the move constructor.
 *
 * This function shows this, see Annotation's ctor for details
 */
void test_std_move_can_not_move_something() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foo f0("sun");
    Annotation aobj(f0); // Indeed the "f0" is not moved, but copied to "aobj"
    aobj.echo();
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_my_own_version_move();

    test_std_move_can_not_move_something();
}


} // namespace item_23

} // namespace chapter_5

} // namespace effective_mordern_cpp

