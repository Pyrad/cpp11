#ifndef UTILITIES
#define UTILITIES

#include <iostream>

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

namespace utilities {

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
void show_template_lvalue_rvalue(T&& param) {
    // fprintf(stdout, "T : %s\n", std::is_const<T>::value ? "IsConst" : "NotConst");
    // fprintf(stdout, "T : %s\n", std::is_volatile<T>::value ? "IsVolatile" : "NotVolatile");
    // fprintf(stdout, "T : %s\n", std::is_lvalue_reference<T>::value ? "IsLValueReference" : "NotLValueReference");
    // fprintf(stdout, "T : %s\n", std::is_rvalue_reference<T>::value ? "IsRValueReference": "NotRValueReference");
    fprintf(stdout, "type of param is: %s\n", type_name<decltype(param)>().c_str());

    fprintf(stdout, "typeid(param).name = %s\n", typeid(param).name());
}




} // end of namespace utilities


#endif // UTILITIES
