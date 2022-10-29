#include "chapter4_item21.hpp"
#include <memory>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_21 {

uint32_t Foo::id_cnt = 0;

/**
 * std::make_* functions to create smart pointers,
 * (1) std::make_unique(), C++11 doesn't have it, C++14 has it.
 * (2) std::make_shared(), C++11 and C++14 both have it.
 * (3) std::allocate_shared(), C++11 and C++14 both have it.
 *
 * The difference between (1) and (3) is that (3) accepts a custom allocator.
 */
void test_std_make_funcs() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    std::allocator<Foo> alloc; // For allocate_shared's first argument

    // Use Foo's default ctor
    auto sp0 = std::make_unique<Foo>();
    auto sp1 = std::make_shared<Foo>();
    auto sp2 = std::allocate_shared<Foo>(alloc);

    // Use Foo's ctor with a name
    auto sp3 = std::make_unique<Foo>("tiger");
    auto sp4 = std::make_shared<Foo>("fish");
    auto sp5 = std::allocate_shared<Foo>(alloc, "bird");
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_std_make_funcs();

}


} // namespace item_21

} // namespace chapter_4

} // namespace effective_mordern_cpp

