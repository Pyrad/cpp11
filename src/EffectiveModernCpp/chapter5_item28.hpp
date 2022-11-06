#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM28
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM28

#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_28 {

/**
 * @brief A sample class for type identification use
 *        (lvalue-reference or rvalue-reference)
 */
class Foobar {
public:
    Foobar() = default;
    Foobar(const std::string &n) : m_name(n) { }

public:
    static Foobar make(const std::string &n) {
        Foobar obj(n);
        return obj;
    }

private:
    std::string m_name = "unkown";
}; // end class Foobar

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
void encode_mechanism_func(T &&param);

/**
 * @brief Shows the deduced types of the parameter passed into a function with
 *        a universal reference
 */
void test_show_deduced_types_of_universal_ref();

/**
 * @brief My own implemenation of std::forward, i.e., custom forward
 */
template<typename T>
T&& my_forward(typename std::remove_reference<T>::type &param) {
    return static_cast<T&&>(param);
}

/**
 * @brief Perfect forwarding by a custom forward
 */
template<typename T>
void show_type_with_my_forward(T &&param) {
    utilities::show_boost_type_index_with_cvr(my_forward<T>(param));
}

/**
 * @brief Using a custom forward to perfect forwarding a parameter to
 *        other functions.
 */
void test_show_deduced_types_of_universal_ref_by_my_forward();

void test_all();

} // namespace item_28

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM28
