#ifndef LVALUE_RVALUE
#define LVALUE_RVALUE

#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <wchar.h>

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>



namespace lvalue_rvalue {

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




class foo {
public:
    static int global_cnt;
    static foo get_foo() { return foo(); }

public:
    foo () :m_id(global_cnt++) { }
    virtual ~foo () { }

public:
    int id() const { return m_id; }

private:
    int m_id;
}; // class foo

/**
 * A class for rvalue test
 */
class char_string {
public:
    char_string(const char *s, const int len) {
        m_len = len;
        m_ptr = (char*)malloc(m_len);
        memcpy(m_ptr, s, m_len);
        fprintf(stdout, "Constructor of char_string(%s)\n", m_ptr);
    }

    char_string(const char_string &cs) {
        m_len = cs.len();
        m_ptr = (char*)malloc(m_len);
        memcpy(m_ptr, cs.ptr(), m_len);
        fprintf(stdout, "Copy constructor of char_string(%s)\n", m_ptr);
    }

    ~char_string() { if (m_ptr) { free(m_ptr); } }

public:
    char * ptr() const { return m_ptr; }
    int len() const { return m_len; }

protected:
    char *m_ptr;
    int m_len;
}; // class char_string

class char_string2 {
public:
    char_string2(const char *s, const int len) {
        m_len = len;
        m_ptr = (char*)malloc(m_len);
        memcpy(m_ptr, s, m_len);
        fprintf(stdout, "Constructor of char_string2(%s)\n", m_ptr);
    }

    char_string2(const char_string2 &cs) {
        m_len = cs.len();
        m_ptr = (char*)malloc(m_len);
        memcpy(m_ptr, cs.ptr(), m_len);
        fprintf(stdout, "Copy constructor of char_string2(%s)\n", m_ptr);
    }

    char_string2(char_string2 &&cs) {
        m_len = cs.len();
        m_ptr = cs.ptr();
        cs.reset_ptr();
        fprintf(stdout, "Move constructor of char_string2(%s)\n", m_ptr);
    }

    ~char_string2() { if (m_ptr) { free(m_ptr); } }

public:
    char * ptr() const { return m_ptr; }
    int len() const { return m_len; }
    void reset_ptr() { m_ptr = nullptr; }

protected:
    char *m_ptr;
    int m_len;
}; // class char_string2

class function_msg_printer {
public:
    function_msg_printer(const char *fn) : m_funcname(fn) {
        fprintf(stdout, "----- BEGIN OF FUNCTION: %s -----\n", m_funcname.c_str());
    }

    ~function_msg_printer() {
        fprintf(stdout, "----- END OF FUNCTION: %s -----\n", m_funcname.c_str());
    }

private:
    std::string m_funcname;

}; // function_msg_printer

struct T {
    double a[4];
};
inline struct T f(void) { return (struct T){3.15}; }
inline double g1(double* x) { return *x; }
inline void g2(double* x) { *x = 1.0; }



/**
 *
 * shape
 *   |
 *   +-----> circle
 *   |
 *   +-----> triangle
 *   |
 *   +-----> rectangle
 */
class shape {
public:
    shape() { std::cout << "shape" << std::endl; }
    virtual ~shape() { std::cout << "~shape" << std::endl; }
};
class circle : public shape {
public:
    circle() { std::cout << "circle" << std::endl; }
    ~circle() { std::cout << "~circle" << std::endl; }
};
class triangle : public shape {
public:
    triangle() { std::cout << "triangle" << std::endl; }
    ~triangle() { std::cout << "~triangle" << std::endl; }
};
class rectangle : public shape {
public:
    rectangle() { std::cout << "rectangle" << std::endl; }
    ~rectangle() { std::cout << "~rectangle" << std::endl; }
};
class result {
public:
    result() { std::cout << "result()" << std::endl; }
    ~result() { std::cout << "~result()" << std::endl; }
};

result process_shape(const shape &shape1, const shape &shape2);


void test_all();

void test_lvalue_rvalue();

void test_resource_move();

void test_temporary_lifetime();

void test_extend_lifetime();

void test_distinguish_lvalue_rvalue_reference();

} // namespace lvalue_rvalue

#endif // LVALUE_RVALUE

