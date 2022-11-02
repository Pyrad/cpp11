#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM24
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM24

#include "../utilities/utilities.hpp"
#include <stdint.h>
#include <stdio.h>

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

    /**
     * Should we return a type without rvalue reference or rvalue reference?
     *
     * See
     * https://www.appsloveworld.com/cplus/100/51/return-value-or-rvalue-reference
     *
     * -----------------------------------------------------------------
     * DataType data() && { return std::move(values); } // why DataType instead of DataType&& ?
     * auto values = makeWidget().data();
     * -----------------------------------------------------------------
     *
     * The temporary that holds the return value will be initialized through the
     * move-constructor, copy-initialized from move(values).
     * 
     * Then that temporary initializes values, but since makeWidget().data() is an
     * rvalue (prvalue to be precise) the move-constructor is called again - with the
     * temporary as its argument.
     * 
     * Now consider copy-elision:
     * 
     * When a nameless temporary, not bound to any references, would be moved or copied
     * into an object of the same cv-unqualified type, the copy/move is omitted. When
     * that temporary is constructed, it is constructed directly in the storage where it
     * would otherwise be moved or copied to. When the nameless temporary is the argument
     * of a return statement, this variant of copy elision is known as RVO,
     * "return value optimization".
     * 
     * So the second move will (presumably) be completely elided, and only one is left -
     * the one we would have had anyway, had the return type been the rvalue reference.
     * 
     * The problem with returning an rvalue reference is that if we write,
     * 
     * -----------------------------------------------------------------
     * auto&& values = makeWidget().data();
     * -----------------------------------------------------------------
     *
     * values will be dangling as binding an xvalue to a reference doesn't extend anythings
     * lifetime. When we return the object type, the temporaries lifetime is extended.
     */
    // Don't use "std::string && name() { return std::move(m_name); }"
    std::string name() { return std::move(m_name); }

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

/**
 * Just a function for generic lambda (C++14) use
 */
void demo_time_start(uint32_t i, const std::string &n);

/**
 * Shows the usage of "auto &&" works as an universal reference
 */
void test_universal_ref_in_auto();

void test_all();

} // namespace item_24

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM24
