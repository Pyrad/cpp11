#ifndef _TEMPLATE_HPP
#define _TEMPLATE_HPP

#include <cstring>
#include <stdio.h>
#include <vector>
#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <stdexcept>
#include <type_traits>
#include <iostream>
#include <sstream>


namespace T1 {

// 2 overloaded functions
// The first one accepts a pointer to a function, which accepts 2 strings as arguments
// The second one accepts a pointer to a function, which accepts 2 int as arguments
void func(int(*)(const std::string&, const std::string&));
void func(int(*)(const int&, const int&));

void func(int(*)(const std::string &s1, const std::string &s2)) {
    std::cout << "T1::func(int(*)(const std::string &s1, const std::string &s2)) is called\n";
}

void func(int(*)(const int&, const int&)) {
    std::cout << "T1::func(int(*)(const int&, const int&)) is called\n";
}

} // namespace T1


namespace T2 {

// Arguments forwarding
// Template parameters: a callable object and 2 arguments
template <typename F, typename TYPE1, typename TYPE2>
void flip(F f, TYPE1 &&t1, TYPE2 &&t2) {
    f(std::forward<TYPE2>(t2), std::forward<TYPE1>(t1));
}

void f1(int v1, int &v2) {
    std::cout << "Function name: " << __FUNCTION__ << ", "<< v1 << " " << ++v2 << "\n";
}

void f2(int &&v1, int &v2) {
    std::cout << "Function name: " << __FUNCTION__ << ", "<< v1 << " " << v2 << "\n";
}

} // namespace T2

namespace T3 {

// Overloading in templates

// First version
template <typename T>
std::string debug_rep(const T &t) {
    std::cout << "Function name: (FirstVersion)" << __FUNCTION__ << "\n";
    std::ostringstream ret;
    ret << t;
    return ret.str();
}

// Second version
template <typename T>
std::string debug_rep(T *p) {
    std::cout << "Function name: (SecondVersion)" << __FUNCTION__ << "\n";
    std::ostringstream ret;
    ret << "pointer: " << p;
    if (p) {
        ret << " " << debug_rep(*p);
    } else {
        ret << " null pointer";
    }
    return ret.str();
}

void test() {
    std::string s("GoodDay");

    // Compiler can't deduct from a non-pointer type to a pointer type
    // So the first version is chosen, and the second version is not available
    std::cout << debug_rep(s) << "\n";

    // the result of "&s" is a non-constant pointer
    // So for the first version compiler deduct as the following,
    //   debug_rep(const std::string* &), now T is std::string*
    // the second version compiler deduct as the following,
    //   debug_rep(std::string*), now T is std::string
    // Since the argument is a non-constant pointer
    // it has to be converted to a constant pointer in first version
    // while for the second one, it need not
    // So the second version is chosen
    std::cout << debug_rep(&s) << "\n";

    // sp is a constant pointer
    // here the two versions are all available
    // First is:
    //   debug_rep(const std::string* &), now T is std::string*
    // Second is:
    //   debug_rep(const std::string*), now T is const std::string
    // But compiler prefers most specialized version
    // Here the first version is general, and the second is specialized
    // so compiler choose the second
    const std::string *sp = &s;
    std::cout << debug_rep(sp) << "\n";

}

} // namespace T3

namespace T4 {
// Variadic template

// Args -> template parameter packet
// rest -> function parameter packet
template <typename T, typename ... Args>
void foo(const T&, const Args& ... rest) {
    std::cout << "Number of Args: " << sizeof...(Args) << "\n";
    std::cout << "Number of rest: " << sizeof...(rest) << "\n";
}


// the exit for recursive template function -> print
template <typename T>
std::ostream &print(std::ostream &os, const T &t) {
    return os << t;
}

// recursive template function -> print
template <typename T, typename ... Args>
std::ostream &print(std::ostream &os, const T &t, const Args& ... rest) {
    os << t << ", ";
    return print(os, rest ...);
}

// Packet expansion
template <typename... Args>
std::ostream &errorMsg(std::ostream &os, const Args&... rest) {
    // After expansion, it looks just like this,
    // print(os, T3::debug_rep(a1), T3::debug_rep(a2), T3::debug_rep(a3), ... , T3::debug_rep(aN))
    return print(os, T3::debug_rep(rest)...);
}

// Variadic template together with argument forwarding
template <typename F, typename... Args>
void func(F f, Args&&... args) {
    f(std::forward<Args>(args)...);
}

void test() {
    int i = 0;
    double d = 3.14;
    std::string s("how now brown cow");

    // Calculate template/function paramter packet size
    foo(i, s, 42, d);
    foo(s, 42, "hi");
    foo(d, s);
    foo("hi");

    //
    printf("\n");
    print(std::cout, i, s, 42);

    //
    printf("\n\n");
    errorMsg(std::cout, i, s, 42);

    printf("\n");
}

} // namespace T4


namespace template_test {

// Function template
template <typename T>
int compare(const T &v1, const T &v2) {
    if (v1 < v2) {
        return -1;
    } else if (v1 > v2) {
        return 1;
    } else {
        return 0;
    }
}

// nontype template parameter
template<unsigned N, unsigned M>
int compare(const char (&p1)[N], const char (&p2)[M]) {
    return strcmp(p1, p2);
}


// inline or constexpr must appear after template parameter list
// and before return type
template <typename T>
inline T min(const T &v1, const T &v2) {
    return v1 < v2 ? v1 : v2;
}

// Return a reference of a sequence(represented by begin and end iterators)
// Here simply return the first one
// No need to use 'typename' before decltype since no member of template parameter is used
template <typename ITR>
auto get_a_element_ref(ITR start, ITR end) -> decltype(*start) {
    return *start;
}

// Return an element copy of a sequence(represented by begin and end iterators)
// Here simply return the first one
// NEED to ADD 'typename' before 'std::remove_reference<decltype(*start)>'
//  since the member 'type' of it is used
template <typename ITR>
auto get_a_element_copy(ITR start, ITR end) -> typename std::remove_reference<decltype(*start)>::type {
    return *start;
}








template <typename T>
class Blob {

public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    // ctors
    Blob();
    Blob(std::initializer_list<T> il);

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    void push_back(const T &t) { data->push_back(t); }
    void push_back(T &&t) { data->push_back(std::move(t)); }

    void pop_back();

    T& back();
    T& operator[](size_type i);

private:
    void check(size_type i, const std::string &msg) const;

private:
    std::shared_ptr<std::vector<T>> data;

}; /* class Blob*/

// Outside of class template, should add typename T after class name
template <typename T>
void Blob<T>::
check(Blob<T>::size_type i, const std::string &msg) const {
    if (i >= data->size()) {
        throw std::out_of_range(msg);
    }
}

template <typename T>
T& Blob<T>::
back() {
    check(0, "back on empty Blob");
    return data->back();
}

template <typename T>
T& Blob<T>::
operator[](size_type i) {
    check(i, "subscript out of range");
    return (*data)[i];
}

template <typename T>
void Blob<T>::
pop_back() {
    check(0, "pop_back on empty Blob");
    return data->pop_back();
}

template <typename T>
Blob<T>::Blob() : data(std::make_shared(std::vector<T>())) {
}

template <typename T>
Blob<T>::Blob(std::initializer_list<T> il) : data(std::make_shared(std::vector<T>(il))) {
}









void tempalte_test_all() {

    printf("\n");
    int res = compare("apple", "banana");
    printf("Compare between 'apple' and 'banana' is: %d\n", res);

    std::vector<int> ivec = {9, 3, 5, 7};
    std::vector<std::string> svec = {"day", "good", "today"};

    auto elem1 = get_a_element_copy(ivec.begin(), ivec.end());
    auto elem2 = get_a_element_ref(svec.begin(), svec.end());
    std::cout << "First element of ivec is: " << elem1 << "\n";
    std::cout << "First element of svec is: " << elem2 << "\n";
    printf("\n");

    T1::func(compare<std::string>);
    T1::func(compare<int>);
    printf("\n");

    T3::test();
    printf("\n");

    T4::test();
    printf("\n");

    printf("\n");
    
}




} // namespace template_test
#endif // _TEMPLATE_HPP
