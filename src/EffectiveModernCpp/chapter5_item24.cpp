#include "chapter5_item24.hpp"
#include <boost/type_index.hpp>
#include <stdint.h>
#include <stdio.h>
#include <utility>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_24 {

uint32_t Foo::id_cnt = 0;

/**
 * A function takes an rvalue reference as an argument
 */
void myf(Foo &&f) {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    namespace bti = boost::typeindex;
    std::cout << "f = " << bti::type_id_with_cvr<decltype(f)>().pretty_name() << "\n";
}

/**
 * Distinguish rvalue references from universal references
 *
 * When "T&&" represents a universal reference,
 * (1) It can be bound to either an lvalue or an rvalue
 * (2) It can be bound to either const value or non-const value
 * (3) It can be bound to either volatile value or non-volatile value
 *
 * Generally, a universal reference "T&&" appears in the following 2 cases,
 * (1) Function template parameters
 * (2) auto declaration
 *
 * Universal reference "T&&" is still a reference, it can be either an lvalue
 * reference or an rvalue reference, on condition that it is initialized with
 * an lvalue or an rvalue,
 * (1) Using an lvalue to initialize a universal reference, it is bound to an lvalue,
 *     thus it is an lvalue reference
 * (2) Using an rvalue to initialize a universal reference, it is bound to an vvalue,
 *     thus it is an rvalue reference
 *
 * 2 conditions that "T&&" is a universal reference,
 * (1) There is a type deduction
 * (2) The form must be "T&&"
 */
void test_rvalue_ref_and_universal_ref() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    myf(Foo());         // Pass an rvalue to myf, which
                        // accepts an rvalue reference
    Foo &&fobj = Foo(); // Binds to an rvalue

    auto &&val0 = Foo(); // It's an universal reference, which
                         // binds to an rvalue
    Foo fobj0("sky");
    auto &&val1 = fobj0; // It's an universaly referencey, which
                         // binds to an lvalue

}

/**
 * Just a function for generic lambda (C++14) use
 */
void demo_time_start(uint32_t i, const std::string &n) {
    fprintf(stdout, "[demo_time_start] Integer = %u, string = %s\n", i, n.c_str());
}

/**
 * Shows the usage of "auto &&" works as an universal reference
 */
void test_universal_ref_in_auto() {
    // A generic lambda of C++14 style, here there're 2 universal references,
    // (1) "auto &&func" is the 1st one, which is a function
    // (2) "auto &&... params" is the 2nd one, which is the function's arguments
    auto timeFuncInvocation = [](auto &&func, auto &&... params) {
        // Time start
        fprintf(stdout, "timeFuncInvocation start\n");
        std::forward<decltype(func)>(func)( std::forward<decltype(params)>(params)... );
        fprintf(stdout, "timeFuncInvocation end\n");
    };

    uint32_t i = 10;
    std::string name("sky");
    timeFuncInvocation(demo_time_start, i, name);
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_rvalue_ref_and_universal_ref();

    test_universal_ref_in_auto();
}


} // namespace item_24

} // namespace chapter_5

} // namespace effective_mordern_cpp

