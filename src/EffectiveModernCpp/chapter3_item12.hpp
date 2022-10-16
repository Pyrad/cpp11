#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM12
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM12

#include "../utilities/utilities.hpp"
#include <stdio.h>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_12 {

/**
 * @brief This class show if conditions are not met, then the
 * functions are not overloaded
 *
 * These conditions are,
 * 1. Func in parent class should be "virtual".
 * 2. Func in parent and child class should have a same name.
 * 3. Func in parent and child class should same argument types.
 * 4. Func in parent and child class should have some constant-ness.
 * 5. Func in parent and child class should have compatible return type and exceptions.
 * 6. Func in parent and child class should have same reference qualifiers.
 */
class WBase {
public:
    using DataType = std::vector<double>;

public:
    /**
     * Return an rvalue object to show different reference qualifiers
     * will make differences.
     */
    static WBase makeWBase() { WBase w; return w; }

public:
    ///< Only an lvalue object can call this
    DataType & data() & { 
        fprintf(stdout, "Called: DataType & data() & { ... }\n");
        return m_d;
    }
    ///< Only an rvalue object can call this
    DataType data() && {
        fprintf(stdout, "Called: DataType data() && { ... }\n");
        return std::move(m_d);
    }

public:
	virtual void mf1() const { fprintf(stdout, "WBase: mf1\n"); }
	virtual void mf2(int x) { fprintf(stdout, "WBase: mf2\n"); }
	virtual void mf3() & { fprintf(stdout, "WBase: mf3\n"); }
	void mf4() const { fprintf(stdout, "WBase: mf4\n"); }

    virtual void mf5() const { fprintf(stdout, "WBase: mf5\n"); }

    virtual void override() const { fprintf(stdout, "WBase: override function called\n"); }

    DataType m_d;

}; // class WBase

class WDerived: public WBase {
public:
	virtual void mf1() { fprintf(stdout, "WDerived: mf1\n"); }
	virtual void mf2(unsigned int x) { fprintf(stdout, "WDerived: mf2\n"); }
	virtual void mf3() && { fprintf(stdout, "WDerived: mf3\n"); }
	void mf4() const { fprintf(stdout, "WDerived: mf4\n"); }

    void mf5() const override { fprintf(stdout, "WDerived: mf5\n"); }

    void override() const { fprintf(stdout, "WDerived: override function called\n"); }
}; // class WDerived

/**
 * @brief A function to show if conditions are not met,
 *        function overloading between parent and child
 *        class won't take effect
 */
void test_overloading_conditions();

/**
 * @brief To show keyword "override" can actually be used as
 *        a function name
 */
void test_overload_as_func_name();

/**
 * @brief Show different reference qualifiers make different,
 *        and sometimes rvalue reference qualifier maybe more
 *        efficient.
 */
void test_diff_ref_qualifiers();

void test_all();


} // namespace item_12

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM12

