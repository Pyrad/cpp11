#ifndef LVALUE_RVALUE
#define LVALUE_RVALUE

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <wchar.h>

namespace lvalue_rvalue {

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

void test_lvalue_rvalue();

void test_resource_move();

void test_all();

} // namespace lvalue_rvalue

#endif // LVALUE_RVALUE

