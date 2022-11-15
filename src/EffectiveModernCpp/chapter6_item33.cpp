#include "chapter6_item33.hpp"

#include "../utilities/utilities.hpp"

#include <string>

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_33 {

///< Just a helper function to show generic lambda usage in C++14
template<typename T>
bool fwd_show_type(T &&param) {
    utilities::show_boost_type_index_with_cvr(std::forward<T>(param));

    return std::is_integral<typename std::remove_reference<T>::type>();
}

///< Just a helper function to show generic lambda usage in C++14
std::string is_intgeral_test(bool is_str) {
    std::string s(is_str ? "IS_INTEGRAL" : "IS_NOT_INTEGRAL");
    return s;
}

/**
 * C++14 introduced generic lambda, in which keyword 'auto'
 * can be used for the argument types
 */
void test_generic_lambda_cxx14() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Since the x's type is declared as 'auto', so it's a generic lambda
    // (since C++14)
    //
    // This generic lambda is capture-by-value
    //
    // This equals to a following template member function which is created
    // by the compiler
    // ------------------------------------------------------
    // class SomeCompilerGeneratedClassName {
    // public:
    // 	template<typename T> // see Item 3 for auto return type (C++14)
    // 	auto operator()(T x) const { return func(normalize(x)); }
    // 	/* ... */ // other closure class functionality
    // };
    // ------------------------------------------------------
    auto f = [](auto x) { return is_intgeral_test(fwd_show_type(x)); };
    fprintf(stdout, "generic lambda test is int = %s\n", f("sky").c_str());

    // Usage universal reference if needed
    // This will keep the lvalue-ness or rvalue-ness
    auto ff = [](auto &&x) { return is_intgeral_test(fwd_show_type(std::forward<decltype(x)>(x))); };
    fprintf(stdout, "generic lambda test is int = %s\n", f("sky").c_str());

} // test_generic_lambda_cxx14

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_generic_lambda_cxx14();
}


} // namespace item_33

} // namespace chapter_6

} // namespace effective_mordern_cpp

