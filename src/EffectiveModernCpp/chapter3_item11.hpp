#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM11
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM11

#include "../utilities/utilities.hpp"
#include <stdint.h>
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_11 {

/**
 * This class shows in C++98, how to forbid using a function.
 * And this is achieved by delcaring it in private but don't
 * define it.
 */
class Foo {
public:
    Foo() : m_id(0) { }
    Foo(const int32_t i) : m_id(i) { }

public:
    int32_t id() const { return m_id; }
    void set_id(const int32_t i) { m_id = i; }

private:
    /**
     * C++98 style
     * Declare the copy-ctor, but don't define it, thus it is
     * forbidden to use.
     */
    Foo(const Foo &);

private:
    int32_t m_id;

}; // class Foo

class Foobar {
public:
    Foobar() { }
    Foobar(const int32_t i) : m_id(i) { }
    virtual ~Foobar () { }

public:
    int32_t id() const { return m_id; }
    void set_id(const int32_t i) { m_id = i; }

    /**
     * C++11 style
     * Declare the copy-ctor as deleted
     */
    Foobar(const Foobar &) = delete;

private:
    int32_t m_id;
}; // class Foobar

/**
 * To show how a function is deleted in C++11 & C++98 style
 */
void test_cxx98_forbid_func();


/**
 * Define a function which only accepts an integer, but doesn't
 * accepts an argument that can be converted to an integer
 */
bool is_lucky(int number); // defined in cpp
bool is_lucky(char) = delete;
bool is_lucky(bool) = delete;
bool is_lucky(double) = delete;


/**
 * "delete" keyword in C++11 can forbid a template instantiation
 * for a certain type, while no such way to do it in C++98
 */
template<typename T> void process_pointer(T *p) {
    fprintf(stdout, "size of p = %d\n", sizeof(p));
} // process_pointer

template<> void process_pointer<void>(void *p) = delete;
template<> void process_pointer<char>(char *p) = delete;
template<> void process_pointer<const void>(const void *p) = delete;
template<> void process_pointer<const char>(const char *p) = delete;



/**
 * To show how to forbid a template instantiation for a certain inside
 * a class, only "delete" keyword in C++11 can do it
 */
class PtrProcess {
public:
    PtrProcess() : m_id(0) {}

    /**
     * Need to forbid template instantiation for a few types, see below
     */
    template<typename T>
    void process_pointer(T *p) {
        fprintf(stdout, "size of p = %d\n", sizeof(p));
    } // process_pointer

private:
    int32_t m_id;

}; // class PtrProcess

/**
 * declare them as delete to forbid template instantiation for these types
 */
template<> void PtrProcess::process_pointer<void>(void *p) = delete;
template<> void PtrProcess::process_pointer<char>(char *p) = delete;
template<> void PtrProcess::process_pointer<const void>(const void *p) = delete;
template<> void PtrProcess::process_pointer<const char>(const char *p) = delete;

void test_all();


} // namespace item_11

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM11

