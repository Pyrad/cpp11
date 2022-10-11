#include "chapter3_item10.hpp"
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_10 {

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
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_enum_class_no_leak();
} // test_all

} // namespace item_10

} // namespace chapter_3

} // namespace effective_mordern_cpp

