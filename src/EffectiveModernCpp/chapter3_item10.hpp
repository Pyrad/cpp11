#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10

#include <cstddef>
#include <cstdint>
#include <iostream>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_10 {

/**
 * An unscoped enum can be declared before defined, but the type
 * must be specified when declared, because the compiler need to
 * know the size it will take to optimize, for example, it needs
 * to know if a char is enough or an integer is needed.
 */
enum FLAG_ALPHA : std::uint32_t;

/**
 * An scoped enum can be declared before defined, and a type can
 * be skipped for it.
 * If the type is not specified, then the default type will be int.
 */
enum class FLAG_BETA;
enum class FLAG_GAMMA : std::uint32_t;

enum Color { black, white, red }; // unscoped enum
enum class EColor { black, white, red }; // scoped enum

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


enum AnimalInfo { ANI_NAME, ANI_YEAR, ANI_WEIGHT };
enum class EAnimalInfo { E_NAME, E_YEAR, E_WEIGHT };
/**
 * The enumrator of unscoped enum can be directly used as an integral for
 * indexing use, but the scoped enumrator can't.
 */
void test_unscoped_enum_used_as_index();

void test_all();


} // namespace item_10

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM10

