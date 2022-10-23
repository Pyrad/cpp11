#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM16
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM16

#include "../utilities/utilities.hpp"
#include <cmath>
#include <vector>
#include <mutex>
#include <atomic>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_16 {

/**
 * @brief A class to show that if a member data is declared as "mutabley",
 *        then it CAN be changed in a const member function
 */
class Polynomial {
public:
    using RootsType = std::vector<double>;

    /**
     * @brief Though this function is 'const', data member m_roots_are_valid is
     *        declared as "mutable", so it can be changed in this const function
     */
    RootsType roots() const {
        // In order to guarentee thread safety, use a mutex
        // to resolve data racing issues
        std::lock_guard<std::mutex> g(m_mtx);
        if (m_roots_are_valid) {
            // If cache not valid, compute roots, and
            // store them in m_root_vals
            m_roots_are_valid = true;
        }
        return m_root_vals;
    }

private:
    /**
     * Once a class has a member data of std::mutex or std::atomic,
     * then this object can't be copied, but only be moved (not copyable,
     * move-only)
     */
    mutable std::mutex m_mtx;
    mutable bool m_roots_are_valid{false};
    mutable RootsType m_root_vals{};
}; // end class Polynomial

/**
 * @brief To improve performance by using atomic instead of mutex
 */
class AtomicPoint {
public:
    AtomicPoint() = default;
    AtomicPoint(const double x, const double y) : m_x(x), m_y(y) { }

public:
    double dist_from_origin() const noexcept {
        m_call_cnt++; // atomic increment
        return std::sqrt(m_x * m_x + m_y * m_y);
    }

private:
    mutable std::atomic<unsigned> m_call_cnt{0};
    double m_x;
    double m_y;
}; // end class AtomicPoint

/**
 * @brief Shows that a data member can be changed in 'const' member function
 *        if this data member is mutable
 */
void test_set_mutable_member_in_const_member_func();

/**
 * Use atomic instead of mutex to improve performance
 */
void test_use_atomic_to_improve_performance();


void test_all();

} // namespace item_16

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM16
