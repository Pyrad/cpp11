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
void test_capture_by_ref_dangling();

/**
 * @brief A function to show capture by value still might lead to dangling issue
 */
void test_capture_by_value_dangling();

/**
 * @brief A function to show static variables can't be captured by lambda
 */
void test_not_capture_static_var();

void test_all();

} // namespace item_31

} // namespace chapter_6

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM31
