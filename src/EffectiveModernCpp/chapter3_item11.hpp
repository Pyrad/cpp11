#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM11
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM11

#include "../utilities/utilities.hpp"
#include <stdint.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_11 {

/**
 * This class shows in C++98, how to forbid using a function.
 * And this is achieved by delcaring it in private but don't
 * define it.
 */
class Foo {
public:
    Foo() : m_id(0) { }
    Foo(const int32_t i) : m_id(i) { }

public:
    int32_t id() const { return m_id; }
    void set_id(const int32_t i) { m_id = i; }

private:
    /**
     * C++98 style
     * Declare the copy-ctor, but don't define it, thus it is
     * forbidden to use.
     */
    Foo(const Foo &);

private:
    int32_t m_id;

}; // class Foo


void test_cxx98_forbid_func();


void test_all();


} // namespace item_11

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM11

