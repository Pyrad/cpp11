#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM15
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM15

#include "../utilities/utilities.hpp"
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_15 {


/**
 * @brief constexpr on functions
 *
 * A function declared as "constexpr", can mean 2 things,
 *
 * (1) If it is used in a context where a compile-time value is needed,
 *     then the argument(s) of it must be compile-time value(s) too,
 *     otherwise compile fails
 * (2) If it is used in a context where a run time value is needed,
 *     then the argument(s) of it can be either runtime value or
 *     compile-time value(s)
 *
 * @note, in C++11, only one return statement is allowed, in C++14,
 *        this limitation is loosed, which means you can write more than
 *        one statements in the constexpr function, and then return
 */
constexpr int test_func_get_compile_time_value(const int x, const int y);

/**
 * @brief A function to show a constexpr function can be used
 *        as the literal template argument
 */
template<int K>
int test_func_use_int_template(const int z) {
    if (K == 0) {
        return K + z;
    } else {
        return K - z;
    }
}

/**
 * @brief Show a compile-time value is needed for a constexpr function when
 *        it is in a context where a compile-time return value is needed.
 *        If it's not in a compile-time context, the arguments can be either
 *        compile-time values or runtime values.
 */
void test_use_constexpr_func();

/**
 * @brief To show that a customized class can be declared as constexpr,
 *        because its constructor and member functions can be declared
 *        as constexpr
 *
 * @note, in C++11, for constexpr member functions, it will be implicitly
 *        declared as const member function too at the same time.
 *        That means the getter functions can be declared as constexpr,
 *        but setter functions can't.
 *        In C++14, this restriction is loosed. That means if you use a
 *        compile time constant object to call setter, and the arguments
 *        are all compile-time constant, then the constexpr version takes
 *        effect. But if a compile time constant object calls a setter
 *        which recieves runtime arguments, compilition fails.
 *        If a runtime object calls setter, then the non-constexpr version
 *        takes effect.
 */
class CxprPoint {
public:
    constexpr CxprPoint(const double x = 0, const double y = 0) noexcept : m_x(x), m_y(y) { }

    // Actually a constexpr member function is declared as
    // const member function too
    constexpr double x() const noexcept { return m_x; }
    constexpr double y() const noexcept { return m_y; }

    // Used constexpr getter of CxprPoint
    static constexpr CxprPoint mid_point(const CxprPoint &a, const CxprPoint &b) noexcept {
        return { (a.x() + b.x()) / 2, (a.y() + b.y()) / 2 };
    }

    // In C++11, the constexpr members are implicitly declared as
    // "const" member functions, but this restriction is loosed in
    // C++14, in which you can define a setter which changes the
    // data member but still can be declared as "constexpr"
    constexpr void set_x(const double x) noexcept { m_x = x; }
    constexpr void set_y(const double y) noexcept { m_y = y; }

    static constexpr CxprPoint reflection(const CxprPoint &p) noexcept {
        CxprPoint rp; // creat a non-const point
        rp.set_x(-p.x()); // Use of constexpr setter
        rp.set_y(-p.y()); // Use of constexpr setter
        return rp;
    }

    /**
     * Show content
     */
    void echo() const {
        fprintf(stdout, "CxprPoint(%.3f, %.3f)\n", m_x, m_y);
    }

private:
    double m_x = 0;
    double m_y = 0;
};

/**
 * To show how to use constexpr objects
 *
 * (1) All built-in types in C++11 are literal types (except void), and they can
 *     be declared as constexpr.
 * (2) A customized class can also be declared as constexpr, because the constructor
 *     and the member functions can be declared as constexpr too.
 */
void test_use_constexpr_objects();

void test_all();

} // namespace item_15

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM15
