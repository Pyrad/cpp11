#ifndef EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM03
#define EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM03

#include <iostream>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_03 {

/**
 * @brief A simple function to show how keyword decltype is used
 *        to declare the return type of a function.
 * 
 * @param[in] c A container like object
 * @param[in] i The index of the value to be accessed in the container
 *
 * @return The reference to the value being indexed in the container.
 *
 * This trailing return type is in format of C++11.
 *
 * @note, here "wtrt" means "with trailing return type"
 */
template<typename Container, typename Index>
auto authAndAccess_wtrt(Container& c, Index i) -> decltype(c[i]) {
	// authenticateUser();
	return c[i];
}

/**
 * @brief A simple function to show how a function's return type is deduced
 *        from keyword auto
 * 
 * @param[in] c A container like object
 * @param[in] i The index of the value to be accessed in the container
 *
 * @return !!!Attention!!! The intended return value is the reference to the
 *         value being indexed in the container. While the auto type deduction
 *         might bring surprises, as the auto type deduction in C++14 follows
 *         the rules of template type deduction.
 *
 * This is WITHOUT trailing return type.
 *
 * @note, here "wotrt" means "without trailing return type"
 * @note, This format (return type is auto) needs C++14 support.
 *
 */
template<typename Container, typename Index>
auto authAndAccess_wotrt(Container& c, Index i) {
	// authenticateUser();
	return c[i];
}

/**
 * @brief A simple function to show how a function's return type is deduced
 *        from keyword decltype with auto in its parenthesis.
 * 
 * @param[in] c A container like object
 * @param[in] i The index of the value to be accessed in the container
 *
 * @return The reference to the value being indexed in the container.
 *         Pay attention to what's the difference between this function and the
 *         function authAndAccess_wotrt above.
 *
 * @note, This format (return type is auto) needs C++14 support.
 *
 */
template<typename Container, typename Index>
decltype(auto) authAndAccess_cxx14_rt(Container& c, Index i) {
	// authenticateUser();
	return c[i];
}

void test_container_access_value();


void test_decltype();


} // namespace item_03

} // namespace chapter_1

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM03

