#include "chapter5_item30.hpp"

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_30 {

/**
 * @brief A simple function to be passed into other functions as an argument
 */
int arg_func(int k) {
    return k > 0 ? k + 100 : k + 200;
}

///< A function's argument is another function
int func_arg_is_func_0(int (*af)(int), int val) {
    return val > 0 ? af(val + 300) : af(val - 300);
}

///< A function's argument is another function (different format)
int func_arg_is_func_1(int af(int), int val) {
    // return val > 0 ? af(val + 300) : af(val - 300);
    return func_arg_is_func_0(af, val);
}

/**
 * Perfect forwarding for a single argument and any number of arguments
 */
void test_perfect_forwarding_samples() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    const std::string s("sky");
    const int value = 10;

    // Perfect forwarding (only 1 argument)
    fwd_show_type(s);
    fwd_show_type(std::string("ocean"));

    // Perfect forwarding (any arguments)
    fwd_show_types(s, value);
    fwd_show_types(s, value, 20);

} // test_perfect_forwarding_samples

/**
 * @brief Perfect forwarding fails on braced initializers
 *
 * (1) Perfect forwarding fails on braced initializers
 * (2) Perfect forwarding fails on 0 or NULL for pointer types
 */
void test_perfect_forwarding_fail_cases() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // If directly use "fwd_show_type({0, 1, 2, 3})", compile fails. Why?
    // ------------------------
    // Because compiler doesn't check the real parameter's type it passed in, but
    // the type it deduces. At this point, C++ standard points out, passing a braced
    // initializer to a function which argument is not std::initializer_list, will
    // result in a non-deduced context, thus compiler was forbidden to deduce, thus
    // compile error.
    // So the workaround is to use auto variable to define a braced initializer. In
    // this circumstances, the compiler is allowed to deduce it as std::initializer_list
    auto ilist = {0, 1, 2, 3};
    fwd_show_type(ilist);

    // For null pointer type, 0 or NULl will be deduced as integral types, because they
    // indeed are integral types.
    // So use nullptr instead;
    fwd_show_type(0);
    fwd_show_type(NULL);
    fwd_show_type(nullptr);

    // In general, "static const" integral types can be declared only in
    // a class w/o definition, and compiler will help propagate the value of
    // it to every occurrances.
    //
    // So actually it may have no memory, so no memory address for it. Thus
    // if to pass it to a function of universal reference, a compiler error
    // will occur, because a reference is actually a pointer, which needs
    // memory address.
    //
    // But some compilers might support retrieving the address of it.
    //
    // Here we can see Foo::value is "static const int", which is just declared in
    // class, w/o definition outside the class.
    // While gcc12 supports retrieving the address of it, so no compiler error.
    int value = Foo::value;
    fwd_show_type(value);
    fwd_show_type(Foo::value);


    // Pass a function (pointer) to another function that accepts a
    // function (pointer) as an argument
    func_arg_is_func_0(arg_func, 25);
    func_arg_is_func_1(arg_func, 25);


} // test_perfect_forwarding_fail_cases

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_perfect_forwarding_samples();

    test_perfect_forwarding_fail_cases();
}


} // namespace item_30

} // namespace chapter_5

} // namespace effective_mordern_cpp

