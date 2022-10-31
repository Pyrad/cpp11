#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM24
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM24

#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_24 {

class Foo {
public:
    static uint32_t id_cnt;

public:
    Foo() : m_id(id_cnt++), m_name("unknown") { }
    Foo(const std::string &n) : m_id(id_cnt++), m_name(n) { }
    Foo(const Foo &f) : m_id(id_cnt++), m_name(f.name()) {
        fprintf(stdout, "Copy constructing Foo object (id = %u)\n", m_id);
    }
    Foo(Foo &&f) : m_id(id_cnt++), m_name(f.name()) {
        fprintf(stdout, "Move constructing Foo object (id = %u)\n", m_id);
    }
    ~Foo() { }

public:
    uint32_t id() const { return m_id; }

    const std::string & name() const { return m_name; }
    std::string && name() { return std::move(m_name); }

    void echo() const { fprintf(stdout, "A foo (ID = %u)\n", m_id); }

private:
    uint32_t m_id = 0;
    std::string m_name = "unknown";
};

/**
 * A function takes an rvalue reference as an argument
 */
void myf(Foo &&f);

/**
 * Distinguish rvalue references from universal references
 *
 * When "T&&" represents a universal reference,
 * (1) It can be bound to either an lvalue or an rvalue
 * (2) It can be bound to either const value or non-const value
 * (3) It can be bound to either volatile value or non-volatile value
 *
 * Generally, a universal reference "T&&" appears in the following 2 cases,
 * (1) Function template parameters
 * (2) auto declaration
 *
 * Universal reference "T&&" is still a reference, it can be either an lvalue
 * reference or an rvalue reference, on condition that it is initialized with
 * an lvalue or an rvalue,
 * (1) Using an lvalue to initialize a universal reference, it is bound to an lvalue,
 *     thus it is an lvalue reference
 * (2) Using an rvalue to initialize a universal reference, it is bound to an vvalue,
 *     thus it is an rvalue reference
 *
 * 2 conditions that "T&&" is a universal reference,
 * (1) There is a type deduction
 * (2) The form must be "T&&"
 */
void test_rvalue_ref_and_universal_ref();

void test_all();

} // namespace item_24

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM24
