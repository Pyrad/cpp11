#include "chapter6_item31.hpp"
#include <functional>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_31 {

// Define a global filter whose elements are function objects
using FilterContainer = std::vector<std::function<bool(int)>>;
FilterContainer g_filters;

/**
 * What is a lambda ?
 * --------------------------------- 
 * A lambda is just an expression, which is a part of the source code.
 * Compiler generates an unique closure type/class for each lambda expression.
 *
 * What is a closure ?
 * --------------------------------- 
 * A closure is a runtime object, it is an instance of a closure class/type.
 * A closure can be copied, so there might be a few closures which are instances
 * of a same closure class.
 *
 * What is a closure class?
 * --------------------------------- 
 * A closure class is a class, from which a closure is instantiated.
 * A closure class may have more than 1 instances, which are closures. (Because
 * a closure can be copied)
 */
void test_what_is_lambda_and_closure() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    int x = 25;

    // c0 is a copy of the closure produced by the lambda
    auto c0 = [x](int y) { return x * y > 55; };
    // c1 is a copy of c0
    auto c1 = c0;
    // c2 is a copy of c1
    auto c2 = c1;

} // test_what_is_lambda_and_closure

/**
 * Lambda has 2 types of default capture mode
 * (1) Capture by reference
 * (2) Capture by value
 * 
 * Both capture mode can lead to the dangling issue. Why?
 * Because lambda only captures the local variables or references, and these
 * variables or references are defined where the lambda exists.
 * It won't capture static variables or data members.
 * In other words, if the life time of those local variables and references
 * run out, the captured references or variables are dangling.
 */
void test_capture_by_ref_dangling() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    int var = 100;
    // DANGER!
    // A lambda that captures a reference of a local variable,
    // and then it is stored in a global container.
    // When it goes out of this function scope, that local variable
    // no longer exists, thus dangling issue
    g_filters.emplace_back([&](int value){ return value % var; });
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_what_is_lambda_and_closure();

    test_capture_by_ref_dangling();
}


} // namespace item_31

} // namespace chapter_6

} // namespace effective_mordern_cpp

