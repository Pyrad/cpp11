#include "chapter4_item20.hpp"
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <unordered_map>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_20 {

uint32_t Foo::id_cnt = 0;

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
void test_create_weak_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // after spf is constructed, the pointed-to Foo's ref count (RC) is 1
    auto spf = std::make_shared<Foo>("test");

    // wpf points to same Foo as spf. RC remains 1
    std::weak_ptr<Foo> wpf(spf);

    // RC goes to 0, and the Foo is destroyed. wpf now dangles
    spf = nullptr;

    // if wpf doesn't point, to an object
    if (wpf.expired()) {
        fprintf(stdout, "A weak_ptr now points to a dangle pointer\n");
    }
}

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
void test_create_shared_ptr_by_weak_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    auto spf = std::make_shared<Foo>("test");
    std::weak_ptr<Foo> wpf(spf);

    auto spf2 = wpf.lock();
    std::shared_ptr<Foo> spf3(wpf);

    spf2->echo();
    spf3->echo();

    // Release all shared pointers
    spf = nullptr;
    spf2 = nullptr;
    spf3 = nullptr;

    // weak_ptr::lock won't throw exception
    auto spf4 = wpf.lock();
    if (spf4) {
        spf4->echo();
    } else {
        fprintf(stdout, "Error: weak_ptr points to a dangle pointer\n");
    }

    // weak_ptr::lock throw exception std::bad_weak_ptr
    try {
        std::shared_ptr<Foo> spf5(wpf);
    } catch (const std::bad_weak_ptr &bwp) {
        fprintf(stdout, "Error: [bad_weak_ptr] %s\n", bwp.what());
    } catch (...) {
        fprintf(stdout, "Error: unknown error\n");
    }
}

/**
 * This mimics a heavy loading operation which might cost a lot of time
 */
std::unique_ptr<const Foo> loadFoo(const uint32_t id) {
    return std::make_unique<const Foo>(std::to_string(id) + "_heavy");
}

/**
 * This takes advantage of std::weak_ptr to cache the Foo object loaded by
 * a heavy but basic operation which cost a lot of time
 */
std::shared_ptr<const Foo> loadFooFast(const uint32_t id) {
    static std::unordered_map<uint32_t, std::weak_ptr<const Foo>> cache_map;

    // objPtr is a std::shared_ptr to cache an object. It also might be
    // a nullptr if this id has no cache in the map
    auto objPtr = cache_map[id].lock();
    if (!objPtr) {
        fprintf(stdout, "Object of ID=%u is not cached, cache it now\n", id);
        // loadFoo() returns a unique_ptr, here it is assigned to a shared_ptr,
        // which transfers the ownership to objPtr
        objPtr = loadFoo(id);
        // Since objPtr is a shared_ptr, so it can be used to initialize a 
        // std::weak_ptr, which is exactly what this map stores for its key
        // value
        cache_map[id] = objPtr;
    } else {
        fprintf(stdout, "Object of ID=%u is already in the cache, use it\n", id);
    }

    return objPtr;
}

/**
 * Use loadFooFast function to show how an object is cached by using weak_ptr
 */
void test_use_weak_ptr_for_cache() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Load id 0 for the first time
    uint32_t id = 0;
    fprintf(stdout, "Load ID = %u for the 1st time\n", id);
    auto sp0 = loadFooFast(id);
    sp0->echo();

    fprintf(stdout, "Load ID = %u for the 2nd time\n", id);
    auto sp1 = loadFooFast(id);
    sp1->echo();

    fprintf(stdout, "Destroied object ID = %u\n", id);
    sp0 = nullptr;
    sp1 = nullptr;

    fprintf(stdout, "Load ID = %u for the 3rd time\n", id);
    auto sp2 = loadFooFast(id);
    sp2->echo();
}

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
void test_weak_ptr_resolve_circular_dependency() {
    std::shared_ptr<CirA> ca = std::make_shared<CirA>();
    std::shared_ptr<CirB> cb = std::make_shared<CirB>();
    std::shared_ptr<CirC> cc = std::make_shared<CirC>();

    ca->m_p = cb; // Now ca has a shared_ptr to cb
    cc->m_p = cb; // Now cc has a shared_ptr to cb
    cb->m_p = std::weak_ptr<CirA>(ca); // Now cb has a weak_ptr to ca

    // Release ca(CirA)
    ca = nullptr;
    if (!cb->m_p.lock()) {
        fprintf(stdout, "cb (CirB) found ca (CirA) was released\n");
    }
}

/**
 * Subject-observer design pattern using weak_ptr
 */
void test_subject_observer_pattern() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Create a subject which might change itself later
    MySubject ms(29);
    // Create some observers which need to observe the subject ms
    auto obs0 = std::make_shared<MyObserver>(100);
    auto obs1 = std::make_shared<MyObserver>(101);
    auto obs2 = std::make_shared<MyObserver>(102);
    // Connect the observers to the subject
    ms.m_obs0 = obs0;
    ms.m_obs1 = obs1;
    ms.m_obs2 = obs2;
    // Subject changes for the 1st time
    ms.change_happens();
    // Release obs1 to mimic as if it is broken down
    obs1 = nullptr;
    // Subject changes for the 2nd time
    ms.change_happens();
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_create_weak_ptr();

    test_create_shared_ptr_by_weak_ptr();

    test_use_weak_ptr_for_cache();

    test_weak_ptr_resolve_circular_dependency();

    test_subject_observer_pattern();
}


} // namespace item_20

} // namespace chapter_4

} // namespace effective_mordern_cpp

