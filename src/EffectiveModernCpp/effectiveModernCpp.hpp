#ifndef EFFECTIVE_MODERN_CPP
#define EFFECTIVE_MODERN_CPP

#include <stdio.h>
#include <string>
#include <type_traits>
#include <cassert>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif // _MSC_VER
namespace effective_mordern_cpp {

namespace chapter_1 {

/**
 * @brief Get the name of the type of this template's argument
 *
 * @return A string to show the name of the typename
 */
template<typename T>
std::string get_type_name() {
    typedef typename std::remove_reference<T>::type T_NO_REF;

    // Check if the type T is a reference
    constexpr const bool sflag(std::is_reference<T>::value);
    std::string refstr;
    if (sflag) {
        // fprintf(stdout, "Type T is a %s reference\n", (sflag ? "" : "NOT"));
        if (std::is_lvalue_reference<T>::value) {
            // fprintf(stdout, "Type T is a lvalue reference\n");
            refstr = "&";
        } else {
            assert(std::is_rvalue_reference<T>::value);
            // fprintf(stdout, "Type T is a rvalue reference\n");
            refstr = "&&";
        }
    } else {
        // fprintf(stdout, "Type T is not a reference\n");
    }

    // The of T (without reference sign)
    char *tname_str =
#ifndef _MSC_VER
        abi::__cxa_demangle(typeid(T_NO_REF).name(), nullptr, nullptr, nullptr);
#else
        nullptr;
#endif
    std::unique_ptr<char, void(*)(void*)> own(tname_str, std::free);

    // If T (without reference sign) is constant or not
    const std::string const_str(std::is_const<T_NO_REF>::value ? "const" : "");

    const std::string ttypename(const_str + " " + tname_str + " " + refstr);

    return ttypename;

} // getype_name

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
    fprintf(stdout, "                Deduction for type T: %s\n", get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", get_type_name<decltype(param)>().c_str());
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
    fprintf(stdout, "                Deduction for type T: %s\n", get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_by_ref_const


void test_template_type_deduction();

} // namespace chapter_1

void test_all();


} // effective_mordern_cpp

#endif // EFFECTIVE_MODERN_CPP
