#include <iostream>
#include <ostream>
#include <stdio.h>
#include <wchar.h>
#include "lvalue_rvalue.hpp"

#define MSG_BAR function_msg_printer(__FUNCTION__)

namespace lvalue_rvalue {

/**
 * Run all test functions in this (header) file
 */
void test_all() {
    // MSG_BAR;
    // function_msg_printer(__FUNCTION__);
    fprintf(stdout, "----- Start of function of lvalue_rvalue::test_all -----\n");
    test_lvalue_rvalue();
    fprintf(stdout, "----- End of function of lvalue_rvalue::test_all -----\n");
} // test_all


void lrvalue_overload_ftest(int& x) {
    std::cout << "lvalue reference overload f(" << x << ")\n";
}
void lrvalue_overload_ftest(const int& x) {
    std::cout << "lvalue reference to const overload f(" << x << ")\n";
}
void lrvalue_overload_ftest(int&& x) {
    std::cout << "rvalue reference overload f(" << x << ")\n";
}

/**
 * lvalue, prvalue and xvalue tests
 */
void test_lvalue_rvalue() {
    fprintf(stdout, "----- Start of function of lvalue_rvalue::test_lvalue_rvalue -----\n");
    // MSG_BAR;
    // function_msg_printer(__FUNCTION__);

    // --------------------------------------------------------------------
    // A value is lvalue if its address can be accessed
    // A lvalue can be assigned by a value, except for char string literals
    const char *mystr = "hello, world";
    fprintf(stdout, "&mystr = %p\n", &mystr);

    // char literal "hello, world" is a lvalue too, even it can't
    // be assigned by a value
    fprintf(stdout, "&\"hello, world\" = %p\n", &"hello, world");

    // --------------------------------------------------------------------
    // lvalue reference examples
    // --------------------------------------------------------------------
    int a = 1;
    int& lref_a = a;
    // Change value by non-const lvalue reference
    lref_a ++;
    const int& lref_const_a = a;
    // Can NOT change value by const lvalue reference
    // lref_const_a++; // error

    // rvalue can be bound to const lvalue reference
    // Here integer 23 is a rvalue
    const int& lref_const_rvalue = 23;
    std::cout << "lref_const_rvalue = " << lref_const_rvalue << std::endl;

    // --------------------------------------------------------------------
    // rvalue reference examples
    // --------------------------------------------------------------------
    int b = 2;
    // Error below, a rvalue reference can ONLY be bound to a rvalue
    // int &&rref_b = b; // error, here b is a lvalue

    int &&rref_2 = 2; // ok
    std::cout << "rref_2 = " << rref_2 << std::endl; // output 2
    rref_2++;
    std::cout << "rref_2 = " << rref_2 << std::endl; // output 3

    // --------------------------------------------------------------------
    // Use std::move to bind a rvalue reference to a lvalue
    // --------------------------------------------------------------------
    int k = 2;

    // rvalue reference only binds to rvalue, while here k is a lvalue
    // int&& rref_k = k; // error
    //
    // std::move(k) is a rvalue, can be bound to a rvalue reference
    int&& rref_k = std::move(k); // ok

    // --------------------------------------------------------------------
    // Function overloading for lvalue and rvalue arguments
    // --------------------------------------------------------------------
    int i = 1;
    const int ci = 2;
    lrvalue_overload_ftest(i);  // calls f(int&)
    lrvalue_overload_ftest(ci); // calls f(const int&)
    lrvalue_overload_ftest(3);  // calls f(int&&) even if f(const int&) exists,
                                // but it would call f(const int&) if f(int&&)
                                // overload wasn't provided
    lrvalue_overload_ftest(std::move(i)); // calls f(int&&)

    // rvalue reference variables are lvalues when used in expressions
    int&& x = 1;
    lrvalue_overload_ftest(x);            // calls f(int& x)
    lrvalue_overload_ftest(std::move(x)); // calls f(int&& x)

    fprintf(stdout, "----- End of function of lvalue_rvalue::test_lvalue_rvalue -----\n");

} // test_lvalue_rvalue

} // namespace lvalue_rvalue

