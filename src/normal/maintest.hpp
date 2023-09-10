#ifndef _MAINTEST_HPP
#define _MAINTEST_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <memory>
#include <fstream>
#include <forward_list>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <functional>
#include <set>
#include <map>
#include <unordered_map>
#include <tuple>
#include <random>
#include <chrono>
#include <ctime>
#include <array>
// #include <unistd.h>

// #include <boost/bimap.hpp>
// #include <boost/circular_buffer.hpp>
// #include <boost/functional/hash.hpp>

namespace mainspace {

void tester_alias_declaration();

void tester_auto();

void tester_decltype();

constexpr int getLiteralVal();

// Validation class
class valClass {
public:
    valClass() = default; // If 'default' is specified, compiler will generate the body of this ctor automatically
    valClass(int a): m_a(a) {}
    valClass(double b): m_b(b) {}
    valClass(int a, double b): m_a(a), m_b(b) {}

    // Below are 2 delegating constructors
    // Notice that m_str shouldn't be in the initialization list because of delegation
    valClass(const std::string& s) : valClass() { m_str = s; }
    valClass(const std::string& s, int a) : valClass(a) { m_str = s; }

    // IMPORTATN NOTICE:
    //   Classes that define a move constructor or move-assignment operator must
    //   also define their own copy operations. Otherwise, those members are deleted
    //   by default.
    //
    // Use rvalue-reference to do copy initialization
    // 'noexcept' here to declare no exception garanteed
    valClass(valClass &&vc) noexcept; // a move constructor
    valClass(const valClass &vc); // a copy constructor

    //---------------------------------------------
    //
    // Important Notice for reference qualifier
    //
    //  Since 'sorted' is a same name
    //  so all of them have to be added reference qualifier
    //  or none of them should be added a reference qualifier
    //  Which is to say, all have or all have none
    //
    //---------------------------------------------
    // Reference qualifier, can be used as rvalue
    // No matter inside or outside class, '&&' must be kept
    valClass sorted() &&;
    // Can be used as lvalue, '&' must be kept inside or outside class
    valClass sorted() const &;

public:
    valClass& operator=(const valClass &d); // it will be declared as '=default' below outside class definition

    // this can also be declared as 'delete' to prevent assignment
    // But then it should be declared inside the class body, not outside
    // valClass& valClass::operator=(const valClass &d) = delete

public:
    enum ErrCode {TA = 0, TB, TC, TD};

public:
    virtual void print() const {
        std::cout << "m_a: " << m_a << ", m_b: " << m_b << std::endl;
    }

    virtual void print_id() const {
        std::cout << "ID is: " << m_a << "\n";
    }

    inline const int get_id() const { return m_a; }
    inline int get_id() { int r = m_a; return r; }

#if 0
public:
    static std::size_t hasher(const valClass &v) {
        return std::hash<int>()(v.get_id());
    }
    static bool equalOperator(const valClass &lhs, const valClass &rhs) {
        return lhs.get_id() == rhs.get_id();
    }

    bool operator==(const valClass &rhs) const {
        return get_id() == rhs.get_id();
    }
#endif // 0

public:
    void error_msg(std::initializer_list<std::string> il);
    void error_msg(ErrCode e, std::initializer_list<std::string> il);

    const std::vector<std::string> process_list_return_1();
    const std::vector<int> process_list_return_2();
    const std::vector<int> process_list_return_empty();

    // constexpr function in a class must be static
    // It can be non-static outside of a class
    static constexpr int getLiteralVal() { return 23; }

public:
    std::vector<int> m_vec;

public:
    int m_global = 3;

private:
    // Initialization inside class must use '=' or '{...}']
    int m_a = 1;
    double m_b = 23.1;
    std::string m_str{"yy"};
}; /* class valClass */

struct hashfunc_valClass {
    std::size_t operator() (const valClass &k) const {
        return std::hash<int>()(k.get_id());
    }
};

struct equal_valClass {
    bool operator() (const valClass &lhs, const valClass &rhs) const {
        return lhs.get_id() == rhs.get_id();
    }
};

// Reference qualifier, can be used as rvalue
// No matter inside or outside class, '&&' must be kept
valClass valClass::
sorted() && {
    std::sort(m_vec.begin(), m_vec.end());
    return *this;
}

// Can be used as lvalue, '&' must be kept inside or outside class
valClass valClass::
sorted() const & {
    valClass ret(*this);
    std::sort(ret.m_vec.begin(), ret.m_vec.end());

    return ret;
}

// this can also be declared as 'delete' to prevent assignment
// But then it should be declared inside the class body, not outside
valClass& valClass::operator=(const valClass &d) = default;

// If you define a move constructor, you must define a
// copy constructor at the same time
valClass::valClass(valClass &&vc) noexcept {
    m_global = vc.m_global;
    m_a = vc.m_a;
    m_b = vc.m_b;
    m_str = vc.m_str;
}

valClass::valClass(const valClass &vc) {
    m_global = std::move(vc.m_global);
    m_a = std::move(vc.m_a);
    m_b = std::move(vc.m_b);
    m_str = std::move(vc.m_str);
}

void valClass::
error_msg(std::initializer_list<std::string> il) {
    printf("Error Message length is: %zu\n", il.size());
    for (const auto &str : il) {
        printf("->%s\n", str.c_str());
    }
}

void valClass::
error_msg(ErrCode e, std::initializer_list<std::string> il) {
    printf("Error Code is: %d, Error message length is: %zu\n", e, il.size());
    for (const auto &str : il) {
        printf("->%s\n", str.c_str());
    }
}

const std::vector<std::string> valClass::
process_list_return_1() {
    std::string str1("Nice");
    std::string str2("to");
    std::string str3("meet");
    std::string str4("you");
    return {str1, str2, str3, str4};
    // return {"Nice", "to", "meet", "you"};
}

const std::vector<int> valClass::
process_list_return_2() {
    int a = 23;
    int b = 24;
    int c = 25;
    return {a, b, c};
}

const std::vector<int> valClass::
process_list_return_empty() {
    return {};
}

// 'final' means this class can't be inheritted anymore
class newValClass final : public valClass {
public:
    // Inherit constructors(non-default and others) of base class
    // This will tell the compiler to generate codes like this,
    // newValClass(<params list>) : valClass(<param arg list>) { }
    // for example
    // newValClass(int a) : valClass(a) { }
    // newValClass(double b) : valClass(b) { }
    // newValClass(int a, double b) : valClass(a, b) { }
    // Be careful, this won't change the access level of the ctors
    // i.e., if a ctor is private in base, then after generated in child class
    // it is still a private class, even 'using' is in public domain in child class
    // Notice, this still can not inherit default-ctor, copy ctor & move ctor
    using valClass::valClass;

public:
    // 'override' means it overrides the print() in parent
    void print() const override {
        print_id();
        printf("------This is class newValClass calling print() in base class------START\n");
        valClass::print();
        printf("------This is class newValClass calling print() in base class------END\n");
    }

    // 'final' here means 'print_id' can't be overridden in a inheritted class
    void print_id() const final {
        printf("The ID of this newValClass is: %d\n", m_cid);
    }

public:
    int m_cid = 1;
    int m_child_value = 3;

};

class Debug {
    // constant constructors
    // it must initialize all its data members
public:
    constexpr Debug(bool b = true): hw(b), io(b), other(b) { }
    constexpr Debug(bool b, bool i, bool o): hw(b), io(i), other(o) { }

public:
    constexpr bool any() { return hw || io || other; }

private:
    bool hw;
    bool io;
    bool other;
}; /* class Debug*/

class divide {
public:
    int operator() (int denominator, int divisor) {
        return denominator / divisor;
    }
}; /* class divide */

int add(int i, int j);

void test_std_regex();

void check_size(const std::string &s, std::string::size_type sz);

void fargstest(int a, int b, int c, int d, int e);

std::shared_ptr<std::string> getFruit(int i);

void test_all();

} // namespace mainspace

#endif // _MAINTEST_HPP

