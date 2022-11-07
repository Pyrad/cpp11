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

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_perfect_forwarding_samples();
}


} // namespace item_30

} // namespace chapter_5

} // namespace effective_mordern_cpp

