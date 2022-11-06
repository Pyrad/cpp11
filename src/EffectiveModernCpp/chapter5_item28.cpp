#include "chapter5_item28.hpp"
#include <boost/type_index.hpp>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_28 {

/**
 * The encode mechanism
 * ---------------------
 *
 * For template function with a universal reference
 * (1) If an lvalue is passed in, type "T" is deduced as lvalue reference
 * (2) If an rvalue is passed in, type "T" is deduced as non-reference type
 *
 * For auto type definition
 * (1) If it is bound to an lvalue, "auto" is deduced as lvalue reference (T&)
 * (2) If it is bound to an rvalue, "auto" is deduced as non-reference (T)
 *
 * Rules of reference collapsing
 * ------------------------------
 * If any one of them is lvalue-reference, the result is an lvalue-reference.
 * Otherwise, the result is an rvalue-reference.
 */
template<typename T>
void encode_mechanism_func(T &&param) {
    utilities::show_boost_type_index_with_cvr(std::forward<T>(param));
}

/**
 * @brief Shows the deduced types of the parameter passed into a function with
 *        a universal reference
 */
void test_show_deduced_types_of_universal_ref() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foobar fobj("sky");
    fprintf(stdout, "\nPassing in an lvalue to a func with universal reference\n");
    encode_mechanism_func(fobj);
    utilities::show_boost_type_index_with_cvr(fobj);

    fprintf(stdout, "\nPassing in an rvalue to a func with universal reference\n");
    encode_mechanism_func(Foobar::make("sun"));
    utilities::show_boost_type_index_with_cvr(Foobar::make("sun"));
}

/**
 * @brief Using a custom forward to perfect forwarding a parameter to
 *        other functions.
 */
void test_show_deduced_types_of_universal_ref_by_my_forward() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foobar fobj("sky");
    fprintf(stdout, "\nPassing in an lvalue to a func with universal reference\n");
    show_type_with_my_forward(fobj);

    fprintf(stdout, "\nPassing in an rvalue to a func with universal reference\n");
    show_type_with_my_forward(Foobar::make("sun"));
}

/**
 * Reference collapsing occurs in 4 cases
 * ------------------------------------------
 * (1) Template parameter type deduction
 * (2) auto type deduction
 * (3) typedef
 * (4) decltype
 *
 * Universal reference exists in 2 places
 * ------------------------------------------
 * (1) Function template (function arguments)
 * (2) auto type variables
 */
void test_type_deduction() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    namespace bti = boost::typeindex;

    // Type deduction of "auto"
    Foobar w("sky");
    const Foobar cw("ocean");
    auto &&fobj0 = w; // Bound to lvalue
    auto &&fobj1 = Foobar::make("sun"); // Bound to rvalue
    auto &&fobj2 = cw; // Bound to lvalue (const)

    fprintf(stdout, "Type deduction of auto\n");
    fprintf(stdout, "type of 'fobj0' = %s\n", bti::type_id_with_cvr<decltype(fobj0)>().pretty_name().c_str());
    fprintf(stdout, "type of 'fobj1' = %s\n", bti::type_id_with_cvr<decltype(fobj1)>().pretty_name().c_str());
    fprintf(stdout, "type of 'fobj2' = %s\n", bti::type_id_with_cvr<decltype(fobj2)>().pretty_name().c_str());

    // Type deduction of template function with universal references
    fprintf(stdout, "Type deduction of template func with univ ref\n");
    encode_mechanism_func(w);
    encode_mechanism_func(Foobar::make("sun"));
    encode_mechanism_func(cw);

} // test_type_deduction

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_show_deduced_types_of_universal_ref();

    test_show_deduced_types_of_universal_ref_by_my_forward();

    test_type_deduction();
}


} // namespace item_28

} // namespace chapter_5

} // namespace effective_mordern_cpp

