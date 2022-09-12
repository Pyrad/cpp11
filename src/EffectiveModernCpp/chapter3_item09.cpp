#include "chapter3_item09.hpp"

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_09 {

/**
 * Alias template can't be defined inside a function, while it
 * can be declared inside a namespace or a class
 */
template<typename T>
using foo_double = foo<T, double>;

/**
 * @brief To show use of a template alias defined by typedef
 */
void test_typedef_template_alias() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    foo_bool<int>::type fobj;
    fprintf(stdout, "fobj: value0 = %d, value1 = %s\n", fobj.value0(), fobj.value1() ? "True" : "False");

    fobj.set_value0(100);
    fobj.set_value1(false);
    fprintf(stdout, "fobj: value0 = %d, value1 = %s\n", fobj.value0(), fobj.value1() ? "True" : "False");

} // test_typedef_template_alias

/**
 * @brief To show how to define alias template, i.e.,
 *        template<typename T>
 *        using name = type;
 */
void test_define_alias_template() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // template<typename T>
    // using foo_double = foo<T, double>;

    foo_double<int> fobj;
    fprintf(stdout, "fobj: value0 = %d, value1 = %.3f\n", fobj.value0(), fobj.value1());

    fobj.set_value0(100);
    fobj.set_value1(200);
    fprintf(stdout, "fobj: value0 = %d, value1 = %.3f\n", fobj.value0(), fobj.value1());

} // test_define_alias_template

void test_all() {
    // Compare function test_typedef_template_alias and test_define_alias_template
    // to see the difference between alias template and template alias defined by
    // typedef keyword
    test_typedef_template_alias();
    test_define_alias_template();
} // test_all

} // namespace item_09

} // namespace chapter_3

} // namespace effective_mordern_cpp

