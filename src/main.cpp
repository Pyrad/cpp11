#include "main.hpp"
#include "template.hpp"
#include <memory>

#include "projectconfig.h"

// ------------------------------------------------------
// The following header files are included conditionally
// based on the macros defined in CMakeLists.txt
// ------------------------------------------------------
#ifdef ENB_NORMAL
#include "./normal/normal.hpp"
#endif // ENB_NORMAL

#ifdef ENB_BOOSTTEST
#include "./boosttest/boosttest.hpp"
#endif // ENB_BOOSTTEST

#ifdef ENB_CPP_FEATURES
#include "./cppfeatures/lvalue_rvalue.hpp"
#endif // ENB_CPP_FEATURES

#ifdef ENB_EFFECTIVE_MODERN_CPP
#include "./EffectiveModernCpp/effectiveModernCpp.hpp"
#endif // ENB_EFFECTIVE_MODERN_CPP
// ------------------------------------------------------
// Headers included conditionally end
// ------------------------------------------------------

/**************************************/
/**  Enable normal feature testing   **/
/**  correspond to noraml.[h|c]pp    **/
/**************************************/
#ifndef ENABLE_TEST_NORMAL_FEATURES
#define ENABLE_TEST_NORMAL_FEATURES 0
#endif // ENABLE_TEST_NORMAL_FEATURES

/**************************************/
/**  Enable boost feature testing    **/
/** correspond to boosttest.[h|c]pp  **/
/**************************************/
#ifndef ENABLE_TEST_BOOST_FEATURES
#define ENABLE_TEST_BOOST_FEATURES 0
#endif // ENABLE_TEST_BOOST_FEATURES

int main() {
    namespace M = mainspace;
    namespace T = template_test;

#ifdef ENB_NORMAL
    #if ENABLE_TEST_NORMAL_FEATURES
    namespace N = NORMAL;
    N::tester_constexpr();
    N::normal_test_all();
    M::test_std_regex();
    #endif // ENABLE_TEST_NORMAL_FEATURES
#endif // ENB_NORMAL

    M::tester_alias_declaration();
    M::tester_auto();
    T::tempalte_test_all();
    M::test_all();

#ifdef ENB_BOOSTTEST
    #if ENABLE_TEST_BOOST_FEATURES
    namespace B = boost_test;
    B::test_boost_all();
    B::boost_normal_test_all();
    #endif // ENABLE_TEST_BOOST_FEATURES
#endif // ENB_BOOSTTEST

#ifdef ENB_CPP_FEATURES
    lvalue_rvalue::test_all();
#endif // ENB_CPP_FEATURES

#ifdef ENB_EFFECTIVE_MODERN_CPP
    effective_mordern_cpp::test_all();
#endif // ENB_EFFECTIVE_MODERN_

    std::cout << "\n\n---end of function main---\n" << std::endl;

    return 0;
}
