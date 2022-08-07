#ifndef LVALUE_RVALUE
#define LVALUE_RVALUE

#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <wchar.h>


namespace lvalue_rvalue {

/**
 * @brief This is just a class to return an rvalue by one of its static function
 */
class foo {
public:
    static int global_cnt;
    static foo get_foo() { return foo(); }

public:
    foo () :m_id(global_cnt++) { }
    virtual ~foo () { }
    foo& operator=(const foo &v) {
        if (v.id() % 2 == 0) {
            global_cnt++;
        } else {
            global_cnt += 2;
        }
        m_id = global_cnt;

        return *this;
    }

public:
    int id() const { return m_id; }

private:
    int m_id;
}; // class foo
   //

/**
 * @brief This is a function to test/show reference collapsing
 */
template<typename T>
class foo2 {
public:
    typedef T& LvalueRefType;
    typedef T&& RvalueRefType;
public:
    void judge_0() {
        static_assert(std::is_lvalue_reference<LvalueRefType>::value,
                        "LvalueRefType & is lvalue reference");
        static_assert(std::is_lvalue_reference<RvalueRefType>::value,
                        "RvalueRefType & is lvalue reference");
        fprintf(stdout, "LvalueRefType and RvalueRefType is lvalue reference\n");
    }
    void judge_1() {
        static_assert(std::is_lvalue_reference<LvalueRefType>::value,
                        "LvalueRefType & is lvalue reference");
        static_assert(std::is_rvalue_reference<RvalueRefType>::value,
                        "RvalueRefType & is rvalue reference");
        fprintf(stdout, "LvalueRefType is lvalue reference and RvalueRefType is rvalue reference\n");
    }
}; // class foo2

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



/**
 * Assume only one parameter for this template function
 * Assume the parameter is a universal reference
 * Based on above, check if the type deduced is lvalue reference or rvalue reference
 * 
 * @param[in] param A parameter to check for its type
 *
 * TODO Currently it only shows the type of the parameter, can't show the name.
 * Perhaps there's is a way to identify the name of the input parameters.
 * 2022-08-07 19:24
 *
 */
template<typename T>
void show_universal_reference(T &&param) {
    /**
     * Use std::is_lvalue_reference<decltype(param)>::value to check the
     * which type of input parameter 'param', to see which type of
     * !!reference!! is deduced by the compiler
     */
    constexpr const bool is_lr = std::is_lvalue_reference<decltype(param)>::value;
    constexpr const bool is_rr = std::is_rvalue_reference<decltype(param)>::value;
    constexpr const bool is_intgl = std::is_integral<decltype(param)>::value;

    /**
     * Use std::is_lvalue_reference<T>::value to check the type 'T' is
     * deduced by compiler to which type (maybe reference, plain data type)
     */
    constexpr const bool is_T_lr = std::is_lvalue_reference<T>::value;
    constexpr const bool is_T_rr = std::is_rvalue_reference<T>::value;
    constexpr const bool is_T_intgl = std::is_integral<T>::value;

    /**
     * Show type information
     */
    fprintf(stdout, "Parameter type info\n");
    fprintf(stdout, "  param is: lvalue ref(%s)", is_lr ? "1" : "0");
    fprintf(stdout, " rvalue ref(%s)", is_rr ? "1" : "0");
    fprintf(stdout, " integral(%s)\n", is_intgl ? "1" : "0");
    fprintf(stdout, "      T is: lvalue ref(%s)", is_T_lr ? "1" : "0");
    fprintf(stdout, " rvalue ref(%s)", is_T_rr ? "1" : "0");
    fprintf(stdout, " integral(%s)\n", is_T_intgl ? "1" : "0");
} // show_universal_reference


/**
 * @brief Similar to show_universal_reference. See @function
 *        show_universal_reference for more details
 * 
 * @param[in] param An input parameter
 * @param[in] s A char string to indicate the name of parameter 'param'
 *
 * @param[out] N/A 
 *
 * @return void 
 *
 * @note N/A
 */
template<typename T>
void show_universal_reference_with_str(T &&param, const char *s) {
    /**
     * Use std::is_lvalue_reference<decltype(param)>::value to check the
     * which type of input parameter 'param', to see which type of
     * !!reference!! is deduced by the compiler
     */
    constexpr const bool is_lr = std::is_lvalue_reference<decltype(param)>::value;
    constexpr const bool is_rr = std::is_rvalue_reference<decltype(param)>::value;
    constexpr const bool is_intgl = std::is_integral<decltype(param)>::value;

    /**
     * Use std::is_lvalue_reference<T>::value to check the type 'T' is
     * deduced by compiler to which type (maybe reference, plain data type)
     */
    constexpr const bool is_T_lr = std::is_lvalue_reference<T>::value;
    constexpr const bool is_T_rr = std::is_rvalue_reference<T>::value;
    constexpr const bool is_T_intgl = std::is_integral<T>::value;

    /**
     * Show type information
     */
    fprintf(stdout, "Parameter type info (param = %s)\n", s);
    fprintf(stdout, "  param is: lvalue ref(%s)", is_lr ? "1" : "0");
    fprintf(stdout, " rvalue ref(%s)", is_rr ? "1" : "0");
    fprintf(stdout, " integral(%s)\n", is_intgl ? "1" : "0");
    fprintf(stdout, "      T is: lvalue ref(%s)", is_T_lr ? "1" : "0");
    fprintf(stdout, " rvalue ref(%s)", is_T_rr ? "1" : "0");
    fprintf(stdout, " integral(%s)\n", is_T_intgl ? "1" : "0");
} // show_universal_reference_with_str

/**
 * Expand to pass in together with the paramter's name 
 * 'v' is the paramter
 * '#v' is the paramter's name
 */
#define SHOW_UNI_REF(v) show_universal_reference_with_str(v, #v)


void test_all();

void test_lvalue_rvalue();

void test_resource_move();

void test_temporary_lifetime();

void test_extend_lifetime();

void test_distinguish_lvalue_rvalue_reference();

void test_show_lr_ref();

void test_ref_collapse();

} // namespace lvalue_rvalue

#endif // LVALUE_RVALUE

