#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM17
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM17

#include "../utilities/utilities.hpp"
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_17 {

/**
 * A class without any user-defined ctor or dtor.
 * In this circumstance, compiler helps us create 6 special member
 * functions, which are,
 *   (1) Default constructor
 *   (2) Destructor
 *   (3) Copy constructor
 *   (4) Copy assignment operator
 *   (5) Move constructor (since C++11)
 *   (6) Move assignment operator (since C++11)
 *
 * Since here we don't define any of these special member functions, then
 * the compiler will create them for us
 */
class ValObj {
public:
    double x() const { return m_x; }
    double y() const { return m_y; }

    void set_x(double x) { m_x = x; }
    void set_y(double y) { m_y = y; }
    void set_xy(double x, double y) { m_x = x; m_y = y; }

    void echo() const { fprintf(stdout, "ValObj{x=%.3f, y=%.3f}\n", m_x, m_y); }

private:
    double m_x = 0;
    double m_y = 0;
}; // end class ValObj

/**
 * A class without any user-defined ctor or dtor, compiler
 * helps us create 6 special member functions (see above)
 */
void test_compiler_generated_special_member_funcs();

void test_all();

} // namespace item_17

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM17
