#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM30
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM30

#include "../utilities/utilities.hpp"
#include <cstddef>
#include <utility>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_30 {

/**
 * @brief A bit field
 */
struct IPv4Header {
    std::uint32_t version:4,    // first 4 bits
                  IHL:4,        // 4 bits followed
                  DSCP:6,       // 6 bits followed
                  ECN:2,        // 2 bits followed
                  totalLength:16;   // 16 remaining
};

void check_ipv4_header(std::size_t hdr);

class Foo {
public:
    // In general, "static const" integral types can be declared only in
    // a class w/o definition, and compiler will help propagate the value of
    // it to every occurrances.
    //
    // So actually it may have no memory, so no memory address for it. Thus
    // if to pass it to a function of universal reference, a compiler error
    // will occur, because a reference is actually a pointer, which needs
    // memory address.
    //
    // But some compilers might support retrieving the address of it.
    static const int value = 10;
};

template<typename T>
T arg_func_template(T k) {
    return k > 0 ? k + 100 : k + 200;
}

///< A simple function to be passed into other functions as an argument
int arg_func(int k);

///< Overloading functions 1
int func_process(int k);
///< Overloading functions 2
int func_process(int k, int priority);

///< A function's argument is another function
int func_arg_is_func_0(int (*af)(int), int val);

///< A function's argument is another function (different format)
int func_arg_is_func_1(int af(int), int val);

///< A function's argument is another overloading function
int func_arg_is_func_2(int (*af)(int, int), int val);

///< A function's argument is another overloading function (different format)
int func_arg_is_func_3(int *af(int, int), int val);

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

/**
 * @brief Shows how to pass a bit field to a perfect forwarding function
 */
void test_bitfield_as_arg();

void test_all();

} // namespace item_30

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM30
