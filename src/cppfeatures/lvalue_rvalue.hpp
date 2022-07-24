#ifndef LVALUE_RVALUE
#define LVALUE_RVALUE

#include <string>
#include <stdio.h>
#include <wchar.h>

namespace lvalue_rvalue {

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

void test_all();

} // namespace lvalue_rvalue

#endif // LVALUE_RVALUE

