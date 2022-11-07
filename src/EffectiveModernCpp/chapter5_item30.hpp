#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM30
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM30

#include "../utilities/utilities.hpp"
#include <utility>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_30 {

/**
 * A basic perfect forwarding sample (only one parameter as universal reference)
 *
 * Perfect forwarding doesn't include pass-by-value and pass-by-pointer
 */
template<typename T>
void fwd_show_type(T &&param) {
    utilities::show_boost_type_index_with_cvr(std::forward<T>(param));
}

/**
 * A function with no argument is needed, because finally variadic template
 * function show_types_impl will call this.
 */
inline void show_types_impl() { 
    fprintf(stdout, "show_types_impl with no argument is called\n");
    return ;
}

template<typename T, typename ... Types>
void show_types_impl(T&& var, Types... vars) {
    if (sizeof ... (vars) == 0) {
        fprintf(stdout, "Argument number is 0\n");
    } else {
        fprintf(stdout, "Argument number is %d\n", sizeof ... (vars));
    }

    utilities::show_boost_type_index_with_cvr(std::forward<T>(var));

    show_types_impl(vars...);
}

/**
 * A basic perfect forwarding sample (variadic parameters as universal reference)
 */
template<typename... Ts>
void fwd_show_types(Ts && ...params) {
    show_types_impl(std::forward<Ts>(params)...);
}

/**
 * Perfect forwarding for a single argument and any number of arguments
 */
void test_perfect_forwarding_samples();

/**
 * @brief Shows different cases in which perfect forwarding might fail
 *
 * (1) Perfect forwarding fails on braced initializers
 * (2) Perfect forwarding fails on 0 or NULL for pointer types
 */
void test_perfect_forwarding_fail_cases();

void test_all();

} // namespace item_30

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM30
