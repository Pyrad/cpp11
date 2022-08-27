#ifndef EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM01
#define EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM01

#include <stdio.h>
#include <string>
#include <type_traits>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif // _MSC_VER

#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_01 {

/**
 * This is an example of a normal function to test for functions as
 * arguments
 */
int func_test_sample(int i, double d);

/**
 * @brief Print the deduced type of the parameter passed to this
 *        template function.
 * @note Template parameter is a (non-const) reference
 *
 * @param[in] A parameter to type deduce use
 *
 * @return void
 */
template<typename T>
void func_by_ref(T &param) {
    fprintf(stdout, "                Deduction for type T: %s\n", utilities::get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", utilities::get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_by_ref

/**
 * @brief Print the deduced type of the parameter passed to this
 *        template function.
 * @note Template parameter is a (non-const) reference
 *
 * @param[in] A parameter to type deduce use
 *
 * @return void
 */
template<typename T>
void func_by_ref_const(const T &param) {
    fprintf(stdout, "                Deduction for type T: %s\n", utilities::get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", utilities::get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_by_ref_const

/**
 * @brief Print the deduced type of the parameter passed to this
 *        template function.
 * @note Template parameter is a (non-const) pointer
 *
 * @param[in] A parameter to type deduce use
 *
 * @return void
 */
template<typename T>
void func_by_ptr(T *param) {
    fprintf(stdout, "                Deduction for type T: %s\n", utilities::get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", utilities::get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_by_ref_const

/**
 * @brief Print the deduced type of the parameter passed to this
 *        template function.
 * @note Template parameter is a (const) pointer
 *
 * @param[in] A parameter to type deduce use
 *
 * @return void
 */
template<typename T>
void func_by_ptr_const(const T *param) {
    fprintf(stdout, "                Deduction for type T: %s\n", utilities::get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", utilities::get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_by_ref_const

/**
 * @brief Print the deduced type of the parameter passed to this
 *        template function.
 * @note Template parameter is a universal reference
 *
 * @param[in] A parameter to type deduce use
 *
 * @return void
 */
template<typename T>
void func_by_ref_universal(T &&param) {
    fprintf(stdout, "                Deduction for type T: %s\n", utilities::get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", utilities::get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_by_ref_const

/**
 * @brief Print the deduced type of the parameter passed to this
 *        template function.
 * @note Template parameter is a value (pass by value)
 *
 * @param[in] A parameter to type deduce use
 *
 * @return void
 */
template<typename T>
void func_by_value(T param) {
    fprintf(stdout, "                Deduction for type T: %s\n", utilities::get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", utilities::get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_by_value


/**
 * @brief Get an array's size at compile time
 * 
 * @param[in] (No argument name) An array of C type
 *
 * @return The size of the array
 *
 * @note Calculate the size of the array at compile time
 *       The name of the parameter is omitted because of no use
 */
template<typename T, std::size_t N>
constexpr std::size_t getArraySize(T (&)[N]) noexcept {
    return N;
}

void test_template_type_deduction();

} // namespace item_01

} // namespace chapter_1

} // namespace effective_mordern_cpp

#endif // EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM01
