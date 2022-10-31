#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM23
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM23

#include "../utilities/utilities.hpp"
#include <stdio.h>
#include <type_traits>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_23 {

class Foo {
public:
    static uint32_t id_cnt;

public:
    Foo() : m_id(id_cnt++), m_name("unknown") { }
    Foo(const std::string &n) : m_id(id_cnt++), m_name(n) { }
    Foo(const Foo &f) : m_id(id_cnt++), m_name(f.name()) {
        fprintf(stdout, "Copy constructing Foo object (id = %u)\n", m_id);
    }
    Foo(Foo &&f) : m_id(id_cnt++), m_name(f.name()) {
        fprintf(stdout, "Move constructing Foo object (id = %u)\n", m_id);
    }
    ~Foo() { }

public:
    uint32_t id() const { return m_id; }

    const std::string & name() const { return m_name; }
    std::string && name() { return std::move(m_name); }

    void echo() const { fprintf(stdout, "A foo (ID = %u)\n", m_id); }

private:
    uint32_t m_id = 0;
    std::string m_name = "unknown";
};

/**
 * @brief A class to show that std::move can't move something
 */
class Annotation {
public:
    /**
     * Since the argument here is an lvalue "const Foo" type, after std::move,
     * it becomes an rvalue "const Foo" type, but Foo's constructor doesn't
     * have a overload with "const Foo" type (just ctor with const lvalue reference,
     * and rvalue reference), so here the const rvalue will be bound to the const lvalue
     * reference, thus the Foo's copy constructor is called, not the move constructor.
     *
     * Function test_std_move_can_not_move_something() shows this.
     */
    Annotation(const Foo f) : m_foo(std::move(f)) {
        fprintf(stdout, "Constructing Annotation object\n");
    }

public:
    void echo() const {
        fprintf(stdout, "Annotation has object Foo id = %u\n", m_foo.id());
    }

private:
    Foo m_foo;
};

/**
 * std::move move a lot of things, but it still can't move something.
 * It's just a cast, and it doesn't do anything at runtime.
 * It doesn't create any code, not even a byte.
 *
 * My own version of std::move, C++11 style
 */
template<typename T>
typename std::remove_reference<T>::type &&
my_move_cxx11(T &&param) {
    using ReturnType = typename std::remove_reference<T>::type &&;
    return static_cast<ReturnType>(param);
}

/**
 * My own version of std::move, C++14 style
 */
template<typename T>
decltype(auto) my_move_cxx14(T &&param) {
    using ReturnType = typename std::remove_reference<T>::type &&;
    return static_cast<ReturnType>(param);
}

/**
 * Use my own version of std::move to move something
 */
void test_my_own_version_move();

/**
 * Since the argument of the Annotation's ctor is an lvalue "const Foo" type,
 * after std::move, it becomes an rvalue "const Foo" type, but Foo's constructor
 * doesn't have a overload with "const Foo" type (just ctor with const lvalue
 * reference, and rvalue reference), so here the const rvalue will be bound to
 * the const lvalue reference, thus the Foo's copy constructor is called,
 * not the move constructor.
 *
 * This function shows this, see Annotation's ctor for details
 *
 * This function show that, if to move thing, don't declare it as "const"
 */
void test_std_move_can_not_move_something();

void test_all();

} // namespace item_23

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM23
