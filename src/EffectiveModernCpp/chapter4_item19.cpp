#include "chapter4_item19.hpp"
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <unordered_set>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_19 {

uint32_t Foo::id_cnt = 0;

/**
 * The size of a std::shared_ptr is 2 size of a raw ptr.
 * That's because it has 2 (raw) pointers, a pointer to the address of the object,
 * and the other pointer to a reference count (indeed it is a pointer to a control
 * block).
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

/**
 * Since the customized deleter for a shared_ptr appears in its arguments list,
 * thus they (shared_ptr) are a same type, which can be assigned to each other,
 * act as the elements in a same container, for a same function's argument.
 */
void test_diff_custom_deleter_one_shared_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    auto del_foo_A = [](Foo *p) {
        if (!p) { return ; }
        fprintf(stdout, "(Type A) Deleting a Foo object(ID = %u)\n", p->id());
        delete p;
    };
    auto del_foo_B = [](Foo *p) {
        if (!p) { return ; }
        fprintf(stdout, "(Type B) Deleting a Foo object(ID = %u)\n", p->id());
        delete p;
    };

    std::shared_ptr<Foo> sp0(new Foo, del_foo_A);
    std::shared_ptr<Foo> sp1(new Foo, del_foo_B);
    std::shared_ptr<Foo> sp2(new Foo, del_foo_B);
    sp2 = sp1; // Though diff deleter, can assign to sp2

    std::vector<std::shared_ptr<Foo>> vpw{sp0, sp1};
}

/**
 * As showed in function test_size_of_shared_ptr, a std::shared_ptr has 2 pointer,
 * one for object itself, the other is actually a pointer to a control block, which
 * contains reference count, weak count, custom deleter and other data.
 *
 * So the size of a shared_ptr is always 2 words (16 bytes in 64bit machine), even
 * it has a custom deleter.
 *
 * Below is a figure of a std::shared_ptr
 *
 * A std::shared_ptr
 * +------------------------+            +------------------------+
 * |       Ptr to T         | -------->  |       T Object         |
 * +------------------------+            +------------------------+
 * | Ptr to Control Block   | -----+        Control Block
 * +------------------------+      |     +------------------------+
 *                                 +---> |   Reference count      |
 *                                       +------------------------+
 *                                       |      Weak count        |
 *                                       +------------------------+
 *                                       |      Other data        |
 *                                       | (e.g custom deleter,   |
 *                                       |    allocator, etc)     |
 *                                       +------------------------+
 *
 */
void test_size_of_shared_ptr_custom_deleter() {
    auto del_foo_C = [](Foo *p) {
        if (!p) { return ; }
        fprintf(stdout, "(Type C) Deleting a Foo object(ID = %u)\n", p->id());
        delete p;
    };

    std::shared_ptr<Foo> sp0(new Foo);
    std::shared_ptr<Foo> sp1(new Foo, del_foo_C);

    fprintf(stdout, "Size of sp0 (shared_ptr) = %d\n", sizeof sp0); // Result is 16
    fprintf(stdout, "Size of sp1 (shared_ptr) = %d\n", sizeof sp1); // Result is 16
}

/**
 * There are a few rules when creating a std::shared_ptr
 *
 * (1) When created by std::make_shared, always create a control block
 * (2) When created by a unique-owned pointer(e.g., unique_ptr),
 *     create a control block.
 * (3) When created by a raw pointer, create a control block.
 *     So if to create another shared_ptr with an object which already has
 *     a control, USE std::shared_ptr or std::weak_ptr, NOT raw pointer
 */
void test_control_block_rules() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    auto del_foo_D = [](Foo *p) {
        if (!p) { return ; }
        fprintf(stdout, "(Type D) Deleting a Foo object(ID = %u)\n", p->id());
        delete p;
    };

    // !!!NOTE!!!
    // This is just for demo use, as we should avoid creating multiple std:shared_ptr
    // with a same raw pointer!
    //
    // auto p = new Foo("sky");
    // std::shared_ptr<Foo> spw1(p, del_foo_D);
    // std::shared_ptr<Foo> spw2(p, del_foo_D);
    //
    // Instead, use a existing shared_ptr to creat another shared_ptr

    std::shared_ptr<Foo> spw1(new Foo("sky"), del_foo_D);
    std::shared_ptr<Foo> spw2(spw1);
}


void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_size_of_shared_ptr();

    test_diff_custom_deleter_for_shared_unique();

    test_diff_custom_deleter_one_shared_ptr();

    test_size_of_shared_ptr_custom_deleter();

    test_control_block_rules();
}


} // namespace item_19

} // namespace chapter_4

} // namespace effective_mordern_cpp

