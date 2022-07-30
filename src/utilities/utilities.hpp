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

namespace utilities {

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
