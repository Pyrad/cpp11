#include "chapter5_item23.hpp"
#include <stdio.h>
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
 *
 * This function show that, if to move thing, don't declare it as "const"
 */
void test_std_move_can_not_move_something() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foo f0("sun");
    Annotation aobj(f0); // Indeed the "f0" is not moved, but copied to "aobj"
    aobj.echo();
}

// lvalue and rvalue overloads
void process_foo(const Foo &f) {
    fprintf(stdout, "Processing Foo object (lvalue ref, id = %u)\n", f.id());
}
void process_foo(Foo &&f) {
    fprintf(stdout, "Processing Foo object (rvalue ref, id = %u)\n", f.id());
}

/**
 * In this function, we'd like to pass the "param" to the lvalue overload of
 * function "process_foo" if "param" is an lvalue, and pass the "param" to the
 * rvalue overload of function "process_foo" if "param" is an rvalue.
 *
 * Since param is an lvalue reference, we'd like to keep its lvalue-ness if it
 * is bound to an lvalue, and convert it to an rvalue if it is bound to an rvalue,
 * thus we use std::forward, it converts a parameter on condition.
 */
void test_std_forwad() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foo fobj("sky");
    load_and_process(fobj);
    load_and_process(std::move(fobj));
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_my_own_version_move();

    test_std_move_can_not_move_something();

    test_std_forwad();
}


} // namespace item_23

} // namespace chapter_5

} // namespace effective_mordern_cpp

