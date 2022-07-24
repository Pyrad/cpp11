#include <iostream>
#include "lvalue_rvalue.hpp"

namespace lvalue_rvalue {

/**
 * Run all test functions in this (header) file
 */
void test_all() {
    test_lvalue_rvalue();
} // test_all

/**
 * lvalue, prvalue and xvalue tests
 */
void test_lvalue_rvalue() {
    const char *mystr = "hello, world";
    fprintf(stdout, "&mystr = %p\n", &mystr);
    fprintf(stdout, "&\"hello, world\" = %p\n", &"hello, world");
} // test_lvalue_rvalue

} // namespace lvalue_rvalue

