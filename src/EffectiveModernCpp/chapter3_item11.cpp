#include "chapter3_item11.hpp"
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_11 {

void test_cxx98_forbid_func() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foo f(10);

    // If the following statement exist, then compilition fails.
    // That's because we declared the copy-ctor in private and
    // didn't define it.
    //----------
    // Foo f2(f);
    //----------

    fprintf(stdout, "f.id() = %d\n", f.id());

} // test_cxx98_forbid_func


void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_cxx98_forbid_func();

} // test_all

} // namespace item_11

} // namespace chapter_3

} // namespace effective_mordern_cpp


