#include "chapter3_item11.hpp"
#include <stdio.h>
#include <string>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_11 {

/**
 * To show how a function is deleted in C++11 & C++98 style
 */
void test_cxx98_forbid_func() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foo f(10);

    // If the following statement exist, then compilition fails.
    // That's because we declared the copy-ctor in private and
    // didn't define it.
    //----------
    // Foo f1(f);
    //----------

    fprintf(stdout, "f.id() = %d\n", f.id());

    Foobar f1(10);
    // If the following statement exist, then compilition fails.
    // That's because we declared the copy-ctor as deleted
    // (C++11 styel)
    //----------
    // Foobar f3(f1);
    //----------

    fprintf(stdout, "f1.id() = %d\n", f1.id());

} // test_cxx98_forbid_func

/**
 * Define a function which only accepts an integer, but doesn't
 * accepts an argument that can be converted to an integer
 */
bool is_lucky(int number) {
    fprintf(stdout, "The lucky number is %d\n", number);
    return true;
} // is_lucky

/**
 * To show a normal function can be declared as deleted in C++11
 */
void test_cxx11_forbid_normal_func() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    const int num = 12;
    is_lucky(num);

    // Since "bool is_lucky(double)" is deleted, so the following
    // can't pass compilition
    // ---------------
    // const double num_1 = 1.3;
    // is_lucky(num_1);
    // ---------------

    // Since "bool is_lucky(double)" is deleted, so the following
    // can't pass compilition either, that's because though "bool is_lucky(float)"
    // is not declared as deleted, compiler will convert a float variable into
    // a double variable, not an int variable, thus the compile error (use of
    // delete function) appears
    // ---------------
    // const float num_2 = 2.8;
    // is_lucky(num_2);
    // ---------------

} // test_cxx11_forbid_normal_func


/**
 * A certain type of template instantiation can be declared as
 * forbidden in C++11 using "delete" keyword
 */
void test_cxx11_forbid_template_instantiation() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    std::string s("kindle");
    process_pointer<std::string>(&s);

    // The following call can't be instantiated, because the template instantiation
    // for type "const char" has been forbidden
    // ---------------
    // const char *sp = "good";
    // process_pointer<const char>(sp);
    // ---------------

    PtrProcess pp;
    pp.process_pointer<std::string>(&s);

    // Like above, the following call can't be instantiated, because the template instantiation
    // for type "const char" has been forbidden
    // ---------------
    // const char *sp = "good";
    // pp.process_pointer<const char>(sp);
    // ---------------

} // test_cxx11_forbid_template_instantiation

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_cxx98_forbid_func();

    test_cxx11_forbid_normal_func();

    test_cxx11_forbid_template_instantiation();

} // test_all

} // namespace item_11

} // namespace chapter_3

} // namespace effective_mordern_cpp


