#ifndef EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM20
#define EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM20

#include "../utilities/utilities.hpp"
#include <memory>
#include <stdint.h>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_20 {

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
 * Why weak_ptr?
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
void test_create_weak_ptr();

/**
 * Why weak_ptr doesn't have dereference operator?
 * -------------------------------------------------------------------
 * Because data racing might happen, which is that if it has a dereference
 * operator, after dereference, it still can be released by other threads,
 * that lead to a bad situation for this thread.
 *
 * Create shared_ptr by a weak_ptr
 * -------------------------------------------------------------------
 * (1) Use member function weak_ptr::lock
 * (2) Use a weak_ptr to initialize a shared_ptr
 */
void test_create_shared_ptr_by_weak_ptr();

/**
 * This mimics a basic heavy loading operation which might cost a lot of time
 */
std::unique_ptr<const Foo> loadFoo(const uint32_t id);

/**
 * This takes advantage of std::weak_ptr to cache the Foo object loaded by
 * a heavy but basic operation which cost a lot of time
 */
std::shared_ptr<const Foo> loadFooFast(const uint32_t id);

void test_use_weak_ptr_for_cache();

void test_all();

} // namespace item_20

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM20
