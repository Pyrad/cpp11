#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM07
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM07

#include <initializer_list>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "../utilities/utilities.hpp"
#include <atomic>

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


/**
 * @brief To show different ways to set the default value for class members
 */
class ObjDefaultMemberValue {
public:
    ///< If members are not initialized in (default/copy/...) ctors, then
    ///< the default values when those members variables are defined will
    ///< be used.
    ///< Here the 0 and 1 are set to m_x and m_y separately. But if they 
    ///< are not set in this default ctor, then 4 and 5 will be set to
    ///< m_x and m_y respectively.
    ObjDefaultMemberValue() : m_x(0), m_y(1) { }
    virtual ~ObjDefaultMemberValue () { }

    ///< For this copy constructor, the default values 4 and 5 will be set to
    ///< m_x and m_y respectively.
    ObjDefaultMemberValue(const ObjDefaultMemberValue &other) { }

public:
    int x() const { return m_x; }
    int y() const { return m_y; }

public:
    void show_me() const {
        // fprintf(stdout, "m_x = %d, m_y = %d, m_z = %d\n", m_x, m_y, m_z);
        fprintf(stdout, "m_x = %d, m_y = %d\n", m_x, m_y);
    }

private:
    int m_x{4}; // Use curly braces to initialize member variable's default value
    int m_y = 5; // Use equal sign to initialize member variable's default value
    // int m_z(0); // parenthesis can't be used to initialize member variable's default value
}; // class ObjDefaultMemberValue


/**
 * @brief Difference between myItem2Ctors and myItem3Ctors, is that myItem3Ctors
 *        has a constructor which accepts std::initializer_list.
 *        So these 2 constructors are to show how curly braces initialization with
 *        an std::initializer_list ctor will result in unexpected behaviours.
 */
class myItem2Ctors {
public:
    myItem2Ctors(int i, bool b) : m_i(i), m_b(b) {
        fprintf(stdout, "[myItem2Ctors] ctor with Int and Bool\n");
    }
    myItem2Ctors(int i, double d) : m_i(i), m_d(d) {
        fprintf(stdout, "[myItem2Ctors] ctor with Int and Double\n");
    }
    virtual ~myItem2Ctors() { }

private:
    int m_i = 100;
    bool m_b = true;
    double m_d = 3.14;
}; // class myItem2Ctors

/**
 * @brief See comment in @myItem2Ctors
 */
class myItem3Ctors {
public:
    myItem3Ctors(int i, bool b) : m_i(i), m_b(b) {
        fprintf(stdout, "[myItem3Ctors] ctor with Int and Bool\n");
    }
    myItem3Ctors(int i, double d) : m_i(i), m_d(d) {
        fprintf(stdout, "[myItem3Ctors] ctor with Int and Double\n");
    }
    myItem3Ctors(std::initializer_list<long double> il) {
        fprintf(stdout, "[myItem3Ctors] ctor std::initializer_list\n");
    }
    virtual ~myItem3Ctors() { }

private:
    int m_i = 100;
    bool m_b = true;
    double m_d = 3.14;
}; // class myItem3Ctors

class myItemFloatOp {
public:
    myItemFloatOp(int i, bool b) : m_i(i), m_b(b) {
        // fprintf(stdout, "[myItemFloatOp] ctor with Int and Bool\n");
    }
    myItemFloatOp(int i, double d) : m_i(i), m_d(d) {
        // fprintf(stdout, "[myItemFloatOp] ctor with Int and Double\n");
    }
    myItemFloatOp(std::initializer_list<long double> il) {
        fprintf(stdout, "[myItemFloatOp] ctor std::initializer_list\n");
    }
    myItemFloatOp(const myItemFloatOp &other) : m_i(other.i()), m_b(other.b()), m_d(other.d()) {
        fprintf(stdout, "[myItemFloatOp] copy ctor\n");
    }
    myItemFloatOp(myItemFloatOp &&other) : m_i(other.i()), m_b(other.b()), m_d(other.d()) {
        fprintf(stdout, "[myItemFloatOp] move ctor\n");
    }
    virtual ~myItemFloatOp() { }
    operator float() const { return m_d; }

public:
    int i() const { return m_i; }
    bool b() const { return m_b; }
    double d() const { return m_d; }

private:
    int m_i = 100;
    bool m_b = true;
    double m_d = 3.14;
};

/**
 * @brief Use to show how ctor with an std::initializer_list hijack other constructors
 *        (when using curly braces) and might lead to compiler errors
 */
class myItemNarrowConversion {
public:
    myItemNarrowConversion(int i, bool b) : m_i(i), m_b(b) {
        // fprintf(stdout, "[myItemNarrowConversion] ctor with Int and Bool\n");
    }
    myItemNarrowConversion(int i, double d) : m_i(i), m_d(d) {
        // fprintf(stdout, "[myItemNarrowConversion] ctor with Int and Double\n");
    }
    myItemNarrowConversion(std::initializer_list<bool> il) {
        fprintf(stdout, "[myItemNarrowConversion] ctor std::initializer_list\n");
    }
    myItemNarrowConversion(const myItemNarrowConversion &other) : m_i(other.i()), m_b(other.b()), m_d(other.d()) {
        fprintf(stdout, "[myItemNarrowConversion] copy ctor\n");
    }
    myItemNarrowConversion(myItemNarrowConversion &&other) : m_i(other.i()), m_b(other.b()), m_d(other.d()) {
        fprintf(stdout, "[myItemNarrowConversion] move ctor\n");
    }
    virtual ~myItemNarrowConversion() { }

public:
    int i() const { return m_i; }
    bool b() const { return m_b; }
    double d() const { return m_d; }

private:
    int m_i = 100;
    bool m_b = true;
    double m_d = 3.14;
}; // class myItemNarrowConversion

class myItemCallDefaultCtor {
public:
    myItemCallDefaultCtor() {
        fprintf(stdout, "[myItemCallDefaultCtor] ctor with Int and Bool\n");
    }
    myItemCallDefaultCtor(std::initializer_list<int> il) {
        fprintf(stdout, "[myItemCallDefaultCtor] ctor std::initializer_list\n");
    }

private:
    int m_i = 100;
    bool m_b = true;
    double m_d = 3.14;
}; // class myItemCallDefaultCtor

void test_ctors();

void test_initialize_ways_compared_with_curly_braces();
void test_class_member_default_values();
void test_equal_sign_cannot_be_used_for_constructing_atomic();
void test_narrowing_conversion();
void test_curly_braces_hijack_with_std_initializer_list();
void test_curly_braces_hijack_with_std_initializer_list_lead_to_compile_errors();
void test_curly_braces_call_default_ctor();

void test_initialize_with_curly_braces();



void test_all();


} // namespace item_07

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM07

