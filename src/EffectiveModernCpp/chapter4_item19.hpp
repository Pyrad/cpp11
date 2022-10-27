#ifndef EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM19
#define EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM19

#include "../utilities/utilities.hpp"
#include <stdint.h>
#include <stdio.h>
#include <string>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_19 {

class Foo {
public:
    static uint32_t id_cnt;

public:
    Foo() : m_id(id_cnt++), m_name("unknown") { }
    Foo(const std::string &n) : m_id(id_cnt++), m_name(n) { }
    ~Foo() { }

public:
    uint32_t id() const { return m_id; }

    void echo() const { fprintf(stdout, "A foo (ID = %u)\n", m_id); }

private:
    uint32_t m_id = 0;
    std::string m_name = "unknown";
};

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
void test_size_of_shared_ptr();

/**
 * The customized deleter appears in the type of a std::unique_ptr,
 * but the customized deleter for a shared_ptr appears in its arguments list.
 *
 * This means that a same shared_ptr can point to a pointer with different
 * customized deleters
 */
void test_diff_custom_deleter_for_shared_unique();

/**
 * Since the customized deleter for a shared_ptr appears in its arguments list,
 * thus they (shared_ptr) are a same type, which can be assigned to each other,
 * act as the elements in a same container, for a same function's argument.
 */
void test_diff_custom_deleter_one_shared_ptr();

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
void test_size_of_shared_ptr_custom_deleter();

void test_all();

} // namespace item_19

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM19
