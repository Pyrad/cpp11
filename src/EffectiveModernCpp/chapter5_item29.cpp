#include "chapter5_item29.hpp"
#include <vector>
#include <array>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_29 {

/**
 * Sometimes the move operations are not as fast as we expect
 * Scott gave 2 examples, std::array and std::string.
 *
 * Moving a std::array object is not as fast as we think, that's
 * because the memory allocated for this std::array object is on stack,
 * not on heap. So moving a std::array is indeed copying it.
 *
 * Moving std::string sometimes has a similar situation. Because today
 * compilers will do Small String Optimization (SSO), so if the char string
 * is short enough, then it is allocated on stack instead of on heap. Thus
 * same result as the analysis above.
 */
void test_move_op_not_fast() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    std::vector<Foobar> vw0(10);
    auto vw1 = std::move(vw0); // Move works as expected

    std::array<Foobar, 10> aw0;
    auto aw1 = std::move(aw0); // Here move is indeed copy
    
    std::string s0("ocean");
    auto s1 = std::move(s0); // Since SSO, here move is indeed copy
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_move_op_not_fast();
}


} // namespace item_29

} // namespace chapter_5

} // namespace effective_mordern_cpp

