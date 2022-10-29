#include "chapter4_item21.hpp"
#include <memory>
#include <stdio.h>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_21 {

uint32_t Foo::id_cnt = 0;

/**
 * Mimics a process to get a priority by a heavy computation
 */
int32_t compute_priority() {
    return 10;
}
/**
 * Just a function to show a potential mem leak might happen
 */
void process_foo_obj(std::shared_ptr<Foo> sp, int32_t priority) {
    if (priority > 5) {
        sp->echo();
    } else {
        fprintf(stdout, "Priority is low\n");
    }
}

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

/**
 * Why using std::make_* functions?
 * ------------------------------------
 * (1) Avoid code duplication
 * (2) Avoid memory leak risk
 * (3) Help compiler generate smaller, faster and leaner data structures.
 */
void test_use_std_make_funcs() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // -------------------------------
    // (1) Avoid code duplication
    // -------------------------------
    // With make func, write Foo once
    auto upw1(std::make_unique<Foo>());
    // Without make func, write Foo twice
    std::unique_ptr<Foo> upw2(new Foo);
    // With make func, write Foo once
    auto spw1(std::make_shared<Foo>());
    // Without make func, write Foo twice
    std::shared_ptr<Foo> spw2(new Foo);

    // -------------------------------
    // (2) Avoid memory leak risk
    // -------------------------------
    // Potential memory leak here, why?
    // After memory for Foo is allocated, but a shared_ptr has not been created,
    // compute_priority might run, and an exception might be thrown and caught
    // by other functions, thus a memory leak happens
    process_foo_obj(std::shared_ptr<Foo>(new Foo("fish")), compute_priority());
    // Avoided potential memory leak here, why?
    // Because make_shared allocates the memory for Foo and create the shared_ptr
    // in its own function body, not separated.
    process_foo_obj(std::make_shared<Foo>("fish"), compute_priority());

    // -------------------------------
    // (3) Help compiler generate smaller, faster and leaner data structures
    // -------------------------------
    // Create a shared_ptr by "new" is less efficient than make_shared
    // Because first "new" allocates the memory for Foo, and then shared_ptr is
    // created with a control block allocated, thus 2 times of memory allocation
    std::shared_ptr<Foo> spw3(new Foo);
    // If using make_shared, compiler will optimize the code to allocate memory just 
    // once, allocate the memory for Foo and control block at the same time
    auto spw4(std::make_shared<Foo>());
}

/**
 * There are a few cases in which std::make_* functions would not be used
 * ------------------------------------------------------------------------
 * (1) When a smart pointer needs a custom deleter
 * (2) If a class has contructors for parathesis and curly braces at the same time
 * (3) If operator "new" and "delete" are overloaded by a class 
 * (4) If a weak_ptr is still in use (be referenced)
 */
void test_not_use_std_make_funcs() {

    // -------------------------------
    // (1) When a smart pointer needs a custom deleter
    // -------------------------------
    auto del_foo = [](Foo *p) {
        if (!p) { return ; }
        fprintf(stdout, "Delete a Foo object by del_foo\n");
        delete p;
    };
    std::unique_ptr<Foo, decltype(del_foo)> up(new Foo, del_foo);
    std::shared_ptr<Foo> sp(new Foo, del_foo);

    // -------------------------------
    // (2) If a class has contructors for parathesis and curly braces at the same time
    // -------------------------------
    // std::vector has a constructor of parathesis and curly braces at the same time,
    // but if to use std::make_*, compiler decides to use parathesis ctor, not curly
    // braces ctor.
    // Actually braced initializers can not be perfectly forwarded, while std::make_*
    // use perfect forwarding, so braced initializers are not applicable here.
    auto upv = std::make_unique<std::vector<int>>(10, 20);
    auto spv = std::make_shared<std::vector<int>>(10, 20);
    fprintf(stdout, "upv->size() = %lu\n", upv->size());
    fprintf(stdout, "spv->size() = %lu\n", spv->size());
    // But there's a workaround to use braced initializers, which is to create a
    // std::initializer_list, and the pass that variable to std::make_* functions
    auto ilist = {10, 20};
    auto spv1 = std::make_shared<std::vector<int>>(ilist);
    fprintf(stdout, "spv1->size() = %lu\n", spv1->size());

    // -------------------------------
    // (3) If operator "new" and "delete" are overloaded by a class
    // -------------------------------
    // If operator "new" and "delete" are overloaded by a class, usaully that means
    // this it wants to allocate memory of just the size of its own, but std::make_*
    // would also allocate memory for the control blocks too

    // -------------------------------
    // (4) If a weak_ptr is still in use (be referenced)
    // -------------------------------
    // Besides the reference count in a std::shared_ptr's control block, a second 
    // reference count (i.e. weak count) is used to track the count of weak_ptrs
    // referred to current object.
    // So, if a shared_ptr is created by std::make_*, then the control block is created
    // at the same time the T object itself is allocated, thus this second reference count
    // will be there too. As a result, only after the final shared_ptr and the final weak_ptr
    // are released, the memory for the T object is released.
    // But if this object requires a lot of memory, then it's a potential issue.
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_std_make_funcs();

    test_use_std_make_funcs();

    test_not_use_std_make_funcs();
}


} // namespace item_21

} // namespace chapter_4

} // namespace effective_mordern_cpp

