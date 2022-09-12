#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM09
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM09

#include <iostream>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_09 {

/**
 * @brief Just a template class to show how to use
 *        "using name = qualifier" to define alias template
 *
 */
template<typename T, typename S>
class foo {
public:
    foo() { }
    virtual ~foo () { }

public:
    T value0() const { return m_value0; }
    void set_value0(const T val) { m_value0 = val; }

    S value1() const { return m_value1; }
    void set_value1(const S val) { m_value1 = val; }

private:
    T m_value0 = 0;
    S m_value1 = 0;
}; // class foo

void test_define_alias_template();

void test_all();

} // namespace item_09

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM09

