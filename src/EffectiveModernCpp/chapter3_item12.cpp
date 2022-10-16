#include "chapter3_item12.hpp"
#include <memory>
#include <utility>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_12 {

/**
 * @brief A function to show if conditions are not met,
 *        function overloading between parent and child
 *        class won't take effect
 *        Thus it's good to have "override" keyword for
 *        reminder.
 *
 * Conditions shown here are,
 * 1. mf1 in parent & child clas have different constantness.
 * 2. mf2 in parent & child clas have different argument types.
 * 3. mf3 in parent & child clas have different reference qualifiers.
 * 4. mf4 in parent is not declared as "virtual"
 *
 * Since conditions are not met, so overloading won't take effect.
 */
void test_overloading_conditions() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    WBase w;
    w.mf1();
    w.mf2(10);
    w.mf3();
    w.mf4();
    w.mf5();

    std::unique_ptr<WDerived> d(new WDerived);
    d->mf1(); // mf1 in parent & child clas have different constantness.
    d->mf2(10); // mf2 in parent & child clas have different argument types.
    // Should use std::move(*d)mf3() instead of d->mf3(), because
    // mf3 can only be called with an rvalue WDerived object
    std::move(*d).mf3(); // mf3 in parent & child clas have different reference qualifiers.
    d->mf4(); // mf4 in parent is not declared as "virtual"
    d->mf5(); // overloading takes effect

} // test_overloading_conditions


/**
 * @brief A function to show if conditions are not met,
 *        function overloading between parent and child
 *        class won't take effect
 */
void test_overload_as_func_name() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    WBase w;
    w.override(); // func name "override" is allowd

    std::unique_ptr<WDerived> d(new WDerived);
    d->override(); // func name "override" is allowd

} // test_overload_as_func_name

/**
 * @brief Show different reference qualifiers make different,
 *        and sometimes rvalue reference qualifier maybe more
 *        efficient.
 */
void test_diff_ref_qualifiers() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    WBase w;

    // Since w is an lvalue, so "DataType & data() & { ... }"
    // is called
    auto val0 = w.data();

    // Since "WBase::makeWBase()" is an rvalue, so "DataType data() &&  { ... }"
    // is called
    auto val1 = WBase::makeWBase().data();

} // test_diff_ref_qualifiers




void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_overloading_conditions();

    test_overload_as_func_name();

    test_diff_ref_qualifiers();

} // test_all

} // namespace item_12

} // namespace chapter_3

} // namespace effective_mordern_cpp


