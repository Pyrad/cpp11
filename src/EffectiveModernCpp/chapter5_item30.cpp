#include "chapter5_item30.hpp"
#include <cstdint>
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_30 {

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
// Added at 2023-10-01, looks g++ 9.3 on Ubuntu doesn't support retrieving
// address of it
const int Foo::value = 10;

void check_ipv4_header(std::size_t hdr) {
    fprintf(stdout, "hdr is %u\n", hdr);
}

/**
 * @brief A simple function to be passed into other functions as an argument
 */
int arg_func(int k) {
    return k > 0 ? k + 100 : k + 200;
}

///< Overloading functions 1
int func_process(int k) {
    return k > 0 ? k + 100 : k + 200;
}

///< Overloading functions 2
int func_process(int k, int priority) {
    return k - priority > 0 ? k + 100 : k + 200;
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

///< A function's argument is another overloading function
int func_arg_is_func_2(int (*af)(int, int), int val) {
    return af(val, val +100);
}

///< A function's argument is another overloading function (different format)
int func_arg_is_func_3(int af(int, int), int val) {
    return func_arg_is_func_2(af, val);
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

    // Pass an overloading function (pointer) to a function, compiler will
    // help us find the correct overloading function
    func_arg_is_func_0(func_process, 25);
    func_arg_is_func_1(func_process, 25);
    func_arg_is_func_2(func_process, 25);
    func_arg_is_func_3(func_process, 25);

    // 'arg_func' is not an overloading function, so compiler will help use find
    // it. But 'func_process' is an overloading function's name, passing it alone
    // to the perfect forwarding function won't work, as compiler has nothing about
    // its type to deduce, so we need to pass the function signature instead to let
    // compiler deduce its type
    fwd_show_type(arg_func); // works, as arg_func is not an overloading function
    // fwd_show_type(func_process); // fails, as func_process is an overloading function
    using ProcessFuncType = int (*)(int);
    ProcessFuncType pf = func_process;
    fwd_show_type(pf); // now works, as pf has signature info for compiler to deduce types

    // Similar as above, arg_func_template is a function template, which means it can
    // be instantiated to different functions, so it doesn't work just passing its
    // name alone to another function as an argument, we must pass an instantiated function
    // to it
    // fwd_show_type(arg_func_template);// fails, as arg_func_template is a template, which means
                                     // a lot functions, compiler doesn't know which one
    fwd_show_type(static_cast<ProcessFuncType>(arg_func_template));// works

} // test_perfect_forwarding_fail_cases

/**
 * @brief Shows how to pass a bit field to a perfect forwarding function
 */
void test_bitfield_as_arg() {
    IPv4Header h = {8, 6, 28, 1, 1023};
    check_ipv4_header(h.totalLength);

    // The following fails because a pointer or reference can't point
    // to a bitfield. (The min unit a pointer/reference can point to is a char)
    // ---------------------------------------
    // fwd_show_type(h.totalLength); // fails

    auto tlen = static_cast<std::uint16_t>(h.totalLength);
    fwd_show_type(tlen); // works
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_perfect_forwarding_samples();

    test_perfect_forwarding_fail_cases();

    test_bitfield_as_arg();
}


} // namespace item_30

} // namespace chapter_5

} // namespace effective_mordern_cpp

