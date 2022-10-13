#include "chapter3_item10.hpp"
#include <cstddef>
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_10 {

/**
 * An unscoped enum can be declared before defined, but the type
 * must be specified when declared, because the compiler need to
 * know the size it will take to optimize, for example, it needs
 * to know if a char is enough or an integer is needed.
 *
 * An scoped enum can be declared before defined, and a type can
 * be skipped for it.
 * If the type is not specified, then the default type will be int.
 */

enum FLAG_ALPHA : std::uint32_t { F_ONE, F_TWO, F_THREE };
enum class FLAG_BETA { EF_ONE, EF_TWO, EF_THREE };
enum class FLAG_GAMMA : std::uint32_t { EF_1, EF_2, EF_3 };


/**
 * A function just to show an unscoped enumrator will be implicitly
 * converted to integral types(int, uint ...)
 */
void pseudo_ret(std::size_t x) {
    return;
} // pseudo_ret

/**
 * To show that unscoped enum will be implicitly converted, while
 * the scoped enum won't
 */
void test_enum_class_no_implicit_convertion() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Unscoped enum
    Color c = white;
    if (c < 14.5) {
        pseudo_ret(c);
    }

    // Scoped enum
    EColor ec = EColor::white;
    if (static_cast<double>(c) < 14.5) {
        pseudo_ret(static_cast<std::size_t>(c));
    }

} // test_enum_class_no_implicit_convertion

/**
 * The enumeration values in a enum class won't leak to the scope
 * where the enum class is defined.
 */
void test_enum_class_no_leak() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    const auto a = Animal::tiger;
    const auto b = Beast::tiger;

    fprintf(stdout, "a = %d\n", int(a));
    fprintf(stdout, "b = %d\n", int(b));
} // test_enum_class_no_leak

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_enum_class_no_leak();

    test_enum_class_no_implicit_convertion();

} // test_all

} // namespace item_10

} // namespace chapter_3

} // namespace effective_mordern_cpp

