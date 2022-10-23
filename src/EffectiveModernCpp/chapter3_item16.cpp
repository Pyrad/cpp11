#include "chapter3_item16.hpp"
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_16 {


/**
 * This function contains an issue(A) when multiple threads use it
 *
 * The issue in this function is,
 * If a thread computes val0 and val1 and assigns the sum to m_cached_value,
 * but has not set m_cache_valid to true, at this point another thread
 * checks the m_cache_valid and finds it is false, then that thread starts
 * computation again, thus duplicated computation happens.
 */
double AtomicPoint::magic_value_versionA() const {
    if (m_cache_valid) {
        return m_cached_value;
    } else {
        auto val0 = expensive_computation_0();
        auto val1 = expensive_computation_1();
        m_cached_value = val0 + val1;
        m_cache_valid = true;
        return m_cached_value;
    }
} // magic_value_versionA

/**
 * This function contains an issue(B) when multiple threads use it
 *
 * The issue in this function is,
 * If one thread gets val0 and val1 after computation, and set m_cache_valid
 * to true, but has not changed the value of m_cached_value, at this point another
 * thread jumps in and finds m_cache_valid is true, then takes the old m_cached_value
 * and return, thus a outdated value is got, which is not correct.
 */
double AtomicPoint::magic_value_versionB() const {
    if (m_cache_valid) {
        return m_cached_value;
    } else {
        auto val0 = expensive_computation_0();
        auto val1 = expensive_computation_1();
        m_cache_valid = true;
        return m_cached_value = val0 + val1;
    }
}

/**
 * To fix the issues which might happen in magic_value_versionA and
 * magic_value_versionB, use std::mutex
 */
double AtomicPoint::magic_value() const {
    std::lock_guard<std::mutex> g(m_mtx);

    if (m_cache_valid) {
        return m_cached_value;
    } else {
        auto val0 = expensive_computation_0();
        auto val1 = expensive_computation_1();
        m_cached_value = val0 + val1;
        m_cache_valid = true;
        return m_cached_value;
    }
}

/**
 * @brief Shows that a data member can be changed in 'const' member function
 *        if this data member is mutable
 */
void test_set_mutable_member_in_const_member_func() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    Polynomial p;
    fprintf(stdout, "Polynomial object root size : %lu\n", p.roots().size());
}

/**
 * Use atomic instead of mutex to improve performance
 */
void test_use_atomic_to_improve_performance() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    AtomicPoint p(3, 4);
    fprintf(stdout, "Distance from p to origin: %.3f\n", p.dist_from_origin());
}

/**
 * Sometimes using mutex can avoid issues which can't be
 * resolved by atomic values
 */
void test_use_mutex_to_avoid_issues_by_atomic() {
    AtomicPoint p(3, 4);
    fprintf(stdout, "A magic value of AtomicPoint: %.3f\n", p.magic_value());
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_set_mutable_member_in_const_member_func();

    test_use_atomic_to_improve_performance();

    test_use_mutex_to_avoid_issues_by_atomic();
}


} // namespace item_16

} // namespace chapter_3

} // namespace effective_mordern_cpp

