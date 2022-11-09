#ifndef EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM31
#define EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM31

#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_31 {

/**
 * What is a lambda ?
 * --------------------------------- 
 * A lambda is just an expression, which is a part of the source code.
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
void test_what_is_lambda_and_closure();

void test_all();

} // namespace item_31

} // namespace chapter_6

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM31
