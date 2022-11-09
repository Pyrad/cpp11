#include "chapter6_item31.hpp"

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_31 {

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

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_what_is_lambda_and_closure();
}


} // namespace item_31

} // namespace chapter_6

} // namespace effective_mordern_cpp

