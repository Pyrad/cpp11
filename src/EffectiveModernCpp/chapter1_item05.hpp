#ifndef EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM05
#define EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM05

#include <corecrt.h>
#include <functional>
#include <iostream>
#include <stdio.h>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_05 {

/**
 * @brief A simple class containing just one int
 *
 */
class foo {
public:
    static int idcnt;

public:
    foo() : m_id(idcnt++) {
        fprintf(stdout, "constructing foo (id = %d)\n", m_id);
    }
    virtual ~foo () { }
    foo(const foo &f) : m_id(idcnt++) {
        fprintf(stdout, "constructing foo (id = %d, copied from foo.id = %d)\n", m_id, f.id());
    }
    foo(foo &&f) : m_id(idcnt++) {
        fprintf(stdout, "constructing foo (id = %d, moved from foo.id = %d)\n", m_id, f.id());
    }

    bool operator==(const foo & rhs) const {
        return id() == rhs.id();
    }

    bool operator<(const foo &rhs) const {
        return id() < rhs.id();
    }

public:
    int id() const { return m_id; }

private:
    int m_id;
}; // end of class foo

struct fooHash {
    auto operator()(const foo &f) const -> size_t {
        return std::hash<int>{}(f.id());
    }
}; // fooHash


void test_unordered_map_without_auto();
void test_unordered_map_with_auto();
void test_auto_in_map_iteration();

void test_std_func_and_auto();

void test_auto_all();


} // namespace item_05

} // namespace chapter_1

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM05
