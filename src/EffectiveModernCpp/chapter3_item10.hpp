#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10

#include <iostream>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_10 {

enum class Animal { monkey, tiger, birds, lion, goat };
enum class Beast { tiger, lion, cheetah };

/**
 * The enumeration values in a enum class won't leak to the scope
 * where the enum class is defined.
 */
void test_enum_class_no_leak();

void test_all();


} // namespace item_10

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10

