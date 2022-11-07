#include "chapter5_item30.hpp"

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_30 {

/**
 * Perfect forwarding for a single argument and any number of arguments
 */
void test_perfect_forwarding_samples() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    const std::string s("sky");
    const int value = 10;

    // Perfect forwarding (only 1 argument)
    fwd_show_type(s);
    fwd_show_type(std::string("ocean"));

    // Perfect forwarding (any arguments)
    fwd_show_types(s, value);
    fwd_show_types(s, value, 20);

} // test_perfect_forwarding_samples

/**
 * @brief Perfect forwarding fails on braced initializers
 *
 * (1) Perfect forwarding fails on braced initializers
 * (2) Perfect forwarding fails on 0 or NULL for pointer types
 */
void test_perfect_forwarding_fail_cases() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // If directly use "fwd_show_type({0, 1, 2, 3})", compile fails. Why?
    // ------------------------
    // Because compiler doesn't check the real parameter's type it passed in, but
    // the type it deduces. At this point, C++ standard points out, passing a braced
    // initializer to a function which argument is not std::initializer_list, will
    // result in a non-deduced context, thus compiler was forbidden to deduce, thus
    // compile error.
    // So the workaround is to use auto variable to define a braced initializer. In
    // this circumstances, the compiler is allowed to deduce it as std::initializer_list
    auto ilist = {0, 1, 2, 3};
    fwd_show_type(ilist);

    // For null pointer type, 0 or NULl will be deduced as integral types, because they
    // indeed are integral types.
    // So use nullptr instead;
    fwd_show_type(0);
    fwd_show_type(NULL);
    fwd_show_type(nullptr);
} // test_perfect_forwarding_fail_cases

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_perfect_forwarding_samples();

    test_perfect_forwarding_fail_cases();
}


} // namespace item_30

} // namespace chapter_5

} // namespace effective_mordern_cpp

