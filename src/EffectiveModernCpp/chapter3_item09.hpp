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


/**
 * Alias template can't be defined inside a function, while it
 * can be declared inside a namespace or a class
 */
template<typename T>
using foo_double = foo<T, double>;


/**
 * To compare with alias template, define a class which defines
 * a type dependent on another type
 */
template <typename T>
struct foo_bool {
    typedef foo<T, bool> type;
}; // foo_bool


/**
 * @brief To show "typename" must be added before a dependent type.
 */
template <typename T>
class myfoo {
public:
    /**
     * Here are 2 non-static member data to show alias template and the the template alias
     * defined by typedef.
     *
     * Here "typename" must be added before the type definition,
     * otherwise a compile error will be issued
     *  error: need 'typename' before 'effective_mordern_cpp::chapter_3::item_09::foo_bool<T>::type'
     *  because 'effective_mordern_cpp::chapter_3::item_09::foo_bool<T>' is a dependent scope
     */
    typename foo_bool<T>::type m_foo_bool;

    /**
     * Here a "typename" is neither need nor permitted, because compiler knows
     * that it is a non-dependent type, which is defined as alias template
     */
    foo_double<T> m_foo_double;

public:
    myfoo() { }
    virtual ~myfoo () { }

    T id() const { return m_id; }
    void set_id(T id) { m_id = id; }

    T get_bool_version_value0() const { return m_foo_bool.value0(); }
    T get_double_version_value0() const { return m_foo_double.value0(); }

    void set_bool_version_value0(T t) { m_foo_bool.set_value0(t); }
    void set_double_version_value0(T t) { m_foo_double.set_value0(t); }

private:
    T m_id = 0;
}; // class myfoo

void test_define_alias_template();

void test_typedef_template_alias();

void test_dependent_type();

void test_all();

} // namespace item_09

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM09

