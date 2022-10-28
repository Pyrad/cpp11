#ifndef EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM20
#define EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM20

#include "../utilities/utilities.hpp"
#include <memory>
#include <stdint.h>
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_20 {

class Foo {
public:
    static uint32_t id_cnt;

public:
    Foo() : m_id(id_cnt++), m_name("unknown") { }
    Foo(const std::string &n) : m_id(id_cnt++), m_name(n) { }
    ~Foo() { }

public:
    uint32_t id() const { return m_id; }

    void echo() const { fprintf(stdout, "A foo (ID = %u)\n", m_id); }

private:
    uint32_t m_id = 0;
    std::string m_name = "unknown";
};

// The following 3 classes are to show how to resolve circular dependency issue
// by using weak_ptr
class CirB; // Forward declaration
class CirA {
public:
    CirA() { fprintf(stdout, "Constructing a CirA object\n"); }
    ~CirA(){ fprintf(stdout, "Destructiong a CirA object\n"); }

public:
    std::shared_ptr<CirB> m_p;
};

class CirB {
public:
    CirB() { fprintf(stdout, "Constructing a CirB object\n"); }
    ~CirB(){ fprintf(stdout, "Destructiong a CirB object\n"); }
public:
    std::weak_ptr<CirA> m_p;
};

class CirC {
public:
    CirC() { fprintf(stdout, "Constructing a CirC object\n"); }
    ~CirC(){ fprintf(stdout, "Destructiong a CirC object\n"); }
public:
    std::weak_ptr<CirB> m_p;
};

/**
 * The class MyObserver, MySubject and function test_subject_observer_pattern below
 * show how to use weak_ptr to realize the subject-observer design pattern
 */
class MyObserver {
public:
    MyObserver() : m_id(0) { }
    MyObserver(const int32_t id) : m_id(id) { }
    ~MyObserver() { }

public:
    void signal(const int32_t id) const {
        fprintf(stdout, "MyObserver object ID = %d received signal "
                "from MyObserver object ID = %d\n", m_id, id);
    }

private:
    int32_t m_id;
};

class MySubject {
public:
    MySubject() : m_id(0) { }
    MySubject(const int32_t id) : m_id(id) { }
    ~MySubject() { }

private:
    void notify_observers() {
        auto p0 = m_obs0.lock();
        if (p0) {
            p0->signal(m_id);
        } else {
            fprintf(stdout, "MySubject found observer 0 has been released\n");
        }

        auto p1 = m_obs1.lock();
        if (p1) {
            p1->signal(m_id);
        } else {
            fprintf(stdout, "MySubject found observer 1 has been released\n");
        }

        auto p2 = m_obs2.lock();
        if (p2) {
            p2->signal(m_id);
        } else {
            fprintf(stdout, "MySubject found observer 2 has been released\n");
        }
    }
public:
    void change_happens() {
        fprintf(stdout, "Making changes for MySubject...\n");
        fprintf(stdout, "Making changes done\n");
        fprintf(stdout, "Notify observers...\n");
        notify_observers();
        fprintf(stdout, "Done\n");
    }

public:
    int32_t m_id;
    std::weak_ptr<MyObserver> m_obs0;
    std::weak_ptr<MyObserver> m_obs1;
    std::weak_ptr<MyObserver> m_obs2;
};


/**
 * Why weak_ptr?
 * -------------------------------------------------------------------
 * Because sometimes a shared_ptr might point to an object which has
 * already been released, while shared_ptr doesn't know that. In this
 * situation, a dangle point appears.
 *
 * So weak_ptr deals with such cases, it doesn't increase reference count,
 * (no management for the resource).
 *
 * A weak_ptr can neither be dereferenced nor be checked for null.
 */
void test_create_weak_ptr();

/**
 * Why weak_ptr doesn't have dereference operator?
 * -------------------------------------------------------------------
 * Because data racing might happen, which is that if it has a dereference
 * operator, after dereference, it still can be released by other threads,
 * that lead to a bad situation for this thread.
 *
 * Create shared_ptr by a weak_ptr
 * -------------------------------------------------------------------
 * (1) Use member function weak_ptr::lock
 * (2) Use a weak_ptr to initialize a shared_ptr
 */
void test_create_shared_ptr_by_weak_ptr();

/**
 * This mimics a basic heavy loading operation which might cost a lot of time
 */
std::unique_ptr<const Foo> loadFoo(const uint32_t id);

/**
 * This takes advantage of std::weak_ptr to cache the Foo object loaded by
 * a heavy but basic operation which cost a lot of time
 */
std::shared_ptr<const Foo> loadFooFast(const uint32_t id);

/**
 * Use loadFooFast function to show how an object is cached by using weak_ptr
 */
void test_use_weak_ptr_for_cache();

/**
 * Subject-observer design pattern using weak_ptr
 */
void test_subject_observer_pattern();

/**
 * Suppose CirA has a pointer to CirB, CirC also has a pointer to CirB,
 * and they are both shared_ptr.
 *
 * If CirB should have a pointer to CirA, which kind of pointer should
 * it have?
 * Answer is weak_ptr
 *
 *  +--------+   shared_ptr      +--------+   shared_ptr    +--------+
 *  |  CirA  | -------------->   |  CirB  | <-------------- |  CirC  |
 *  +--------+                   +--------+                 +--------+
 *      /\                            |
 *      |         weak_ptr            |
 *      +-----------------------------+
 *
 */
void test_weak_ptr_resolve_circular_dependency();

void test_all();

} // namespace item_20

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM20
