#ifndef EFFECTIVE_MODERN_CPP
#define EFFECTIVE_MODERN_CPP

#include <stdio.h>
#include <string>
#include <type_traits>
// #include <cassert>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif // _MSC_VER

#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {


/**
 * This is an example of a normal function to test for functions as
 * arguments
 */
int func_test_sample(int i, double d);

#if 0
/**
 * @brief Get the name of the type of this template's argument
 *
 * @return A string to show the name of the typename
 */
template<typename T>
std::string get_type_name() {
    typedef typename std::remove_reference<T>::type T_NO_REF;
    typedef typename std::remove_pointer<T>::type T_NO_PTR;

    // Check if the type T is a reference
    constexpr const bool rflag(std::is_reference<T>::value);
    // Check if the type T is a pointer to
    constexpr const bool pflag(std::is_pointer<T>::value);
    // Check if the type T is an array
    constexpr const bool aflag(std::is_array<T>::value);
    // A string contains either a pointer or a reference
    std::string flagstr;

    if (rflag) {
        if (std::is_lvalue_reference<T>::value) {
            flagstr = "&"; // Type T is an lvalue reference
        } else {
            assert(std::is_rvalue_reference<T>::value);
            flagstr = "&&"; // Type T is an rvalue reference
        }
    } else if (pflag) {
        flagstr = "*"; // Type T is a pointer
    } else if (aflag) {
    } else {
        // fprintf(stdout, "Type T is neither a pointer nor a reference\n");
    }

    // The of T (without reference/pointer sign)
    char *tname_str =
#ifndef _MSC_VER
    #if rflag
        abi::__cxa_demangle(typeid(T_NO_REF).name(), nullptr, nullptr, nullptr);
    #else
        abi::__cxa_demangle(typeid(T_NO_PTR).name(), nullptr, nullptr, nullptr);
    #endif
#else
        nullptr;
#endif
    std::unique_ptr<char, void(*)(void*)> own(tname_str, std::free);

    // const bool bool_value_a = (rflag && std::is_const<T_NO_REF>::value);
    // const bool bool_value_b = (pflag && std::is_const<T_NO_PTR>::value);
    // const bool bool_value_c = (aflag && std::is_const<T>::value);
    // fprintf(stdout, "a, b, c = %d, %d, %d\n", bool_value_a, bool_value_b, bool_value_c);

    // If T (without reference/pointer sign) is constant or not
    std::string const_str;
    if (rflag) {
        const_str = std::is_const<T_NO_REF>::value ? "const" : const_str;
    } else if (pflag) {
        const_str = std::is_const<T_NO_PTR>::value ? "const" : const_str;
    } else if (aflag) {
        const_str = std::is_const<T>::value ? "const" : const_str;
    } else {
        // do nothing
    }

    // It can't be a reference and a pointer at the same time.
    assert(!(rflag && pflag));

    const std::string ttypename(const_str + " " + tname_str + " " + flagstr);

    return ttypename;

} // get_type_name
#endif // 0

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
} // func_by_ref_const


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

} // namespace chapter_1

void test_all();


} // effective_mordern_cpp

#endif // EFFECTIVE_MODERN_CPP
