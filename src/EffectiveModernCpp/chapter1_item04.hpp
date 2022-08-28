#ifndef EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM04
#define EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM04

#include <iostream>
#include <boost/type_index.hpp>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_04 {

/**
 * \class foo
 * \brief A demo class
 *
 *  Just a demo class
 */
class foo {
public:
    foo() : m_a(0) { }
    foo(const int i) : m_a(i) { }
    virtual ~foo() { }

public:
    static foo create_foo(int a) { return foo(a); }

protected:
    int m_a; /*!< Just a demo member value */
}; // end of class foo

/**
 * @brief A function to accept a const T & input
 *
 * @param[in] param An input to show its type
 *
 * @note Print the type of the T and the type of param
 */
template<typename T>
void func_by_ref_const(const T &param) {

    namespace bti = boost::typeindex;
    // fprintf(stdout, "T = %s\n", bti::type_id_with_cvr<T>().pretty_name());
    // fprintf(stdout, "param = %s\n", bti::type_id_with_cvr<decltype(param)>().pretty_name());
    std::cout << "T = " << bti::type_id_with_cvr<T>().pretty_name() << "\n";
    std::cout << "param = " << bti::type_id_with_cvr<decltype(param)>().pretty_name() << "\n";

} // func_by_ref_const


void test_boost_type_index();

void test_show_runtime_types();

} // namespace item_04

} // namespace chapter_1

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM04
