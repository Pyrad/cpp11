#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM16
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM16

#include "../utilities/utilities.hpp"
#include <vector>

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
        if (m_roots_are_valid) {
            m_roots_are_valid = true;
        }
        return m_root_vals;
    }

private:
    mutable bool m_roots_are_valid{false};
    mutable RootsType m_root_vals{};
}; // end class Polynomial

/**
 * @brief Shows that a data member can be changed in 'const' member function
 *        if this data member is mutable
 */
void test_set_mutable_member_in_const_member_func();


void test_all();

} // namespace item_16

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM16
