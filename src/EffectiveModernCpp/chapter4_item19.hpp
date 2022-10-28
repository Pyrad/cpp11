#ifndef EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM19
#define EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM19

#include "../utilities/utilities.hpp"
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

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
 * Why enable_shared_from_this?
 * ----------------------------
 *   If we want to create a shared_ptr inside a class method, normally we don't
 * create a shared_ptr directly by contructing it.
 * The reason is that it always create a new control block, but we don't know
 *   if there is any other shared_ptr has already been created using this object
 * (this).
 *   Thus if we create it directly, very likely we will create a duplicated control
 * block, then we have troubles.
 *   So the answer is using enable_shared_from_this (inherit it as a parent class),
 * then use this->shared_from_this() function in that class method.
 *
 * Prerequisite to use shared_from_this function?
 * ----------------------------------------------
 *   In order to use "this->shared_from_this()", we should make sure there must be
 * an existing std::shared_ptr pointing to current object (*this). If not, undefined
 * behavoir happens (normally shared_from_this() function throws an exception).
 *   So usually we create a public factory method to create an object of this class,
 * and move the constructor to private scope.
 */
class FooShared : public std::enable_shared_from_this<FooShared> {
public:
    static uint32_t id_cnt;

public:
#if 0
    // A factory method to create a FooShared object
    static std::shared_ptr<FooShared> create(const std::string &n) {
        std::shared_ptr<FooShared> p(new FooShared(n));
        // return std::make_shared<FooShared>(n);
        return p;
    }
#endif // 0

    // A factory method with universal reference to create a FooShared object
    template<typename T, typename ... Ts>
    static std::shared_ptr<FooShared> create_from(Ts && ... params) {
        std::shared_ptr<FooShared> p(new FooShared(std::forward<Ts>(params)...));
        return p;
    }

    ~FooShared() { }

public:
    uint32_t id() const { return m_id; }

    void echo() const { fprintf(stdout, "A FooShared (ID = %u)\n", m_id); }

    void process(std::vector<std::shared_ptr<FooShared>> &fvec);

private:
    FooShared() : m_id(id_cnt++), m_name("unknown") { }
    FooShared(const std::string &n) : m_id(id_cnt++), m_name(n) { }

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
void test_control_block_rules();

/**
 * (Same comments for class FooShared)
 *
 * Why enable_shared_from_this?
 * ----------------------------
 *   If we want to create a shared_ptr inside a class method, normally we don't
 * create a shared_ptr directly by contructing it.
 * The reason is that it always create a new control block, but we don't know
 *   if there is any other shared_ptr has already been created using this object
 * (this).
 *   Thus if we create it directly, very likely we will create a duplicated control
 * block, then we have troubles.
 *   So the answer is using enable_shared_from_this (inherit it as a parent class),
 * then use this->shared_from_this() function in that class method.
 *
 * Prerequisite to use shared_from_this function?
 * ----------------------------------------------
 *   In order to use "this->shared_from_this()", we should make sure there must be
 * an existing std::shared_ptr pointing to current object (*this). If not, undefined
 * behavoir happens (normally shared_from_this() function throws an exception).
 *   So usually we create a public factory method to create an object of this class,
 * and move the constructor to private scope.
 */
void test_shared_from_this();

void test_all();

} // namespace item_19

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM19
