#include "main.hpp"
#include "normal.hpp"
#include "template.hpp"
#include "boosttest.hpp"
// #include <boost/shared_ptr.hpp>
#include <memory>

/**************************************/
/**  Enable normal feature testing   **/
/**  correspond to noraml.[h|c]pp    **/
/**************************************/
#ifndef ENABLE_TEST_NORMAL_FEATURES
#define ENABLE_TEST_NORMAL_FEATURES 1
#endif // ENABLE_TEST_NORMAL_FEATURES

/**************************************/
/**  Enable boost feature testing    **/
/** correspond to boosttest.[h|c]pp  **/
/**************************************/
#ifndef ENABLE_TEST_BOOST_FEATURES
#define ENABLE_TEST_BOOST_FEATURES 0
#endif // ENABLE_TEST_BOOST_FEATURES

#ifndef SWITCH_ORDER
#define SWITCH_ORDER 0
#endif // SWITCH_ORDER

int main() {

    namespace M = mainspace;

    namespace T = template_test;

    namespace B = boost_test;

    M::tester_constexpr();
    M::tester_alias_declaration();
    M::tester_auto();

#if SWITCH_ORDER
    M::test_all();

    T::tempalte_test_all();
#else
    T::tempalte_test_all();

    M::test_all();
#endif // SWITCH_ORDER


#if ENABLE_TEST_NORMAL_FEATURES

    B::test_boost_all();

    M::test_std_regex();

    NORMAL::normal_test_all();

#endif // ENABLE_TEST_NORMAL_FEATURES

#if ENABLE_TEST_BOOST_FEATURES
    B::boost_normal_test_all();
#endif // ENABLE_TEST_BOOST_FEATURES

    std::cout << "\n\n---end of function main---\n" << std::endl;

    return 0;
}
