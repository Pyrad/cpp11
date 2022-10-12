#include "chapter3_item10.hpp"
#include <cstddef>
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_10 {

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

