#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10

#include <cstddef>
#include <iostream>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_10 {

enum Color { black, white, red }; // unscoped enum
enum class EColor { black, white, red }; // scoped enum
                                         //
/**
 * A function just to show an unscoped enumrator will be implicitly
 * converted to integral types(int, uint ...)
 */
void pseudo_ret(std::size_t x);

/**
 * Defines 2 enum classes, which will have a same name "tiger",
 * but because the enumrator values won't leak to where they are
 * defined, so the same names won't conflict
 */
enum class Animal { monkey, tiger, birds, lion, goat };
enum class Beast { tiger, lion, cheetah };

/**
 * The enumeration values in a enum class won't leak to the scope
 * where the enum class is defined.
 */
void test_enum_class_no_leak();

/**
 * To show that unscoped enum will be implicitly converted, while
 * the scoped enum won't
 */
void test_enum_class_no_implicit_convertion();

void test_all();


} // namespace item_10

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10

