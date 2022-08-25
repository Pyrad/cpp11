#ifndef UTILITIES
#define UTILITIES

#include <iostream>

#include <stdio.h>
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <cassert>

namespace utilities {

/**
 * @brief Get the name of the type of this template's argument
 *
 * @return A string to show the name of the typename
 *
 * This implementation was referred to the documentation below in a certain degree
 * https://newbedev.com/is-it-possible-to-print-a-variable-s-type-in-standard-c/#
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


/**
 * @brief Check a parameter's value type(not value category)
 * Copied from the following website
 *     https://newbedev.com/is-it-possible-to-print-a-variable-s-type-in-standard-c/#
 * 
 * @return A char string to say which type it is. For example, "int&", "const foo &&", etc
 */
template <class T>
std::string type_name() {
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

template<typename T>
void show_template_lvalue_rvalue(T&& param, const char *pname = nullptr) {
    fprintf(stdout, "template<typename T>\nvoid show_template_lvalue_rvalue(T&& param)\n");
    fprintf(stdout, "==> type of param (%s) is: %s\n\n", pname, type_name<decltype(param)>().c_str());
}





} // end of namespace utilities


#endif // UTILITIES
