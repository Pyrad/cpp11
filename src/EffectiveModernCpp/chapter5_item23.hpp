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

void test_my_own_version_move();

void test_all();

} // namespace item_23

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM23
