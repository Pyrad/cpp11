#ifndef EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM21
#define EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM21

#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_21 {

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
 * std::make_* functions to create smart pointers,
 * (1) std::make_unique(), C++11 doesn't have it, C++14 has it.
 * (2) std::make_shared(), C++11 and C++14 both have it.
 * (3) std::allocate_shared(), C++11 and C++14 both have it.
 *
 * The difference between (1) and (3) is that (3) accepts a custom allocator.
 */
void test_std_make_funcs();

void test_all();

} // namespace item_21

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM21
