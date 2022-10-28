#include "chapter4_item20.hpp"

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_20 {

uint32_t Foo::id_cnt = 0;

/**
 * Why weak_pt?
 * -------------------------------------------------------------------
 * Because sometimes a shared_ptr might point to an object which has
 * already been released, while shared_ptr doesn't know that. In this
 * situation, a dangle point appears.
 *
 * So weak_ptr deals with such cases, it doesn't increase reference count,
 * (no management for the resource).
 *
 * A weak_ptr can neither be dereferenced nor be checked for null.
 */
void test_create_weak_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // after spf is constructed, the pointed-to Widget's ref count (RC) is 1
    auto spf = std::make_shared<Foo>("test");

    // wpf points to same Foo as spf. RC remains 1
    std::weak_ptr<Foo> wpf(spf);

    // RC goes to 0, and the Foo is destroyed. wpf now dangles
    spf = nullptr;

    // if wpf doesn't point, to an object
    if (wpf.expired()) {
        fprintf(stdout, "A weak_ptr now points to a dangle pointer\n");
    }
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_create_weak_ptr();
}


} // namespace item_20

} // namespace chapter_4

} // namespace effective_mordern_cpp

