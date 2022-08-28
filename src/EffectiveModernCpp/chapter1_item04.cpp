#include "chapter1_item04.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_04 {

void test_boost_type_index() {
    const auto mf = foo::create_foo(0);
    func_by_ref_const(mf);
}

void test_show_runtime_types() {
    test_boost_type_index();
}

} // namespace item_04

} // namespace chapter_1

} // namespace effective_mordern_cpp
