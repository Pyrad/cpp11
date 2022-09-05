#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM07
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM07

#include <iostream>
#include <vector>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_07 {

/**
 * @brief A simple class containing just one int
 *
 */
class foo {
public:
    static int idcnt;

public:
    foo() : m_id(idcnt++), m_value(0) {
        fprintf(stdout, "constructing foo (id = %d)\n", m_id);
    }

    foo(const int i) : m_id(idcnt++), m_value(i) {
        fprintf(stdout, "constructing foo (id = %d)\n", m_id);
    }

    virtual ~foo () { }

    foo(const foo &f) : m_id(idcnt++), m_value(f.value()) {
        fprintf(stdout, "constructing foo (id = %d, copied from foo.id = %d)\n", m_id, f.id());
    }

    foo(foo &&f) : m_id(idcnt++), m_value(f.value()) {
        fprintf(stdout, "constructing foo (id = %d, moved from foo.id = %d)\n", m_id, f.id());
    }

    foo & operator=(const foo &f) {
        fprintf(stdout, "assigning foo (id = %d, assigned from foo.id = %d)\n", m_id, f.id());
        this->set_value(f.value());
        return *this;
    }

public:
    void show_me() const {
        fprintf(stdout, "This is foo of id = %d, value = %d\n", m_id, m_value);
    }


public:
    int id() const { return m_id; }
    int value() const { return m_value; }
    void set_value(const int v) { m_value = v; }

private:
    int m_id;
    int m_value;
}; // end of class foo


void test_ctors();


void test_all();


} // namespace item_07

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM07

