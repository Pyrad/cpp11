#include "chapter4_item19.hpp"
#include <memory>
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_19 {

uint32_t Foo::id_cnt = 0;

/**
 * The size of a std::shared_ptr is 2 size of a raw ptr.
 * That's because it has 2 (raw) pointers, a pointer to the address of the object,
 * and the other pointer to a reference count.
 *
 * A reference count is indeed a word (4 bytes for 32-bit machine, or 8 bytes for
 * 64-bit machine), thus a shared_ptr is 8 bytes for 32-bit machine (4 bytes by 2
 * pointers), or 16 bytes for 64-bit machine (8 bytes by 2 pointers)
 * The pointer to reference count is dynamically allocated, and it can be avoided
 * by std::make_shared().
 *
 * The increment or decrement of a reference count is atomic operation(thread safe)
 */
void test_size_of_shared_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    std::shared_ptr<Foo> p0 = std::make_shared<Foo>();
    std::shared_ptr<Foo> p1 = std::make_shared<Foo>("good");

    p0->echo();
    p1->echo();

    fprintf(stdout, "Size of p0 (shared_ptr) = %d\n", sizeof p0); // Result is 16
    fprintf(stdout, "Size of p1 (shared_ptr) = %d\n", sizeof p1); // Result is 16
}

/**
 * The customized deleter appears in the type of a std::unique_ptr,
 * but the customized deleter for a shared_ptr appears in its arguments list.
 *
 * This means that a same shared_ptr can point to a pointer with different
 * customized deleters
 */
void test_diff_custom_deleter_for_shared_unique() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    auto del_foo = [](Foo *p) {
        if (!p) { return ; }
        fprintf(stdout, "Deleting a Foo object(ID = %u)\n", p->id());
        delete p;
    };

    std::unique_ptr<Foo, decltype(del_foo)> up(new Foo, del_foo);

    std::shared_ptr<Foo> sp(new Foo, del_foo);
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_size_of_shared_ptr();

    test_diff_custom_deleter_for_shared_unique();
}


} // namespace item_19

} // namespace chapter_4

} // namespace effective_mordern_cpp

