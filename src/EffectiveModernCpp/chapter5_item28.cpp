#include "chapter5_item28.hpp"
#include <boost/type_index.hpp>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_28 {

/**
 * The encode mechanism
 * ---------------------
 * (1) If an lvalue is passed in, type "T" is deduced as lvalue reference
 * (2) If an rvalue is passed in, type "T" is deduced as non-reference type
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
void show_deduced_types_of_universal_ref() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Foobar fobj("sky");
    fprintf(stdout, "\nPassing in an lvalue to a func with universal reference\n");
    encode_mechanism_func(fobj);
    utilities::show_boost_type_index_with_cvr(fobj);

    fprintf(stdout, "\nPassing in an rvalue to a func with universal reference\n");
    encode_mechanism_func(Foobar::make("sun"));
    utilities::show_boost_type_index_with_cvr(Foobar::make("sun"));
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    show_deduced_types_of_universal_ref();
}


} // namespace item_28

} // namespace chapter_5

} // namespace effective_mordern_cpp

