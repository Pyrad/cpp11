#include "chapter4_item21.hpp"
#include <memory>
#include <stdio.h>

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

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_std_make_funcs();

    test_use_std_make_funcs();
}


} // namespace item_21

} // namespace chapter_4

} // namespace effective_mordern_cpp

