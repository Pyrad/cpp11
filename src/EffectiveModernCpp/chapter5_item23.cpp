#include "chapter5_item23.hpp"
#include <string>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_23 {

uint32_t Foo::id_cnt = 0;

void test_my_own_version_move() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foo f0("sun");
    Foo f1("moon");
    Foo f2(f0);
    Foo f3(my_move_cxx11(f0));
    Foo f4(my_move_cxx14(f1));

    f2.echo();
    f3.echo();
    f4.echo();
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_my_own_version_move();
}


} // namespace item_23

} // namespace chapter_5

} // namespace effective_mordern_cpp

