#include "chapter4_item18.hpp"
#include <memory>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_18 {


/**
 * @brief A factory method to return a std::unique_ptr
 *
 * @note, std::unique_ptr is available after C++14, it is not
 *        available in C++11
 */
std::unique_ptr<UniInvestment> makeInvestment(const double val) {
    if (val < 0) {
        return std::make_unique<UniInvestment>(val);
    } else if (val > 100) {
        return std::make_unique<UniStock>(val);
    } else if (val > 50) {
        return std::make_unique<UniBond>(val);
    } else {
        return std::make_unique<UniRealEstate>(val);
    }
}

/**
 * @brief A factory method makeInvestment returns a std::unique_ptr,
 *        which might point to different kind of inheritted classes
 */
void test_factory_method_return_unique_ptr() {
    auto p0 = makeInvestment(-1);
    p0->echo();

    auto p1 = makeInvestment(102);
    p1->echo();

    auto p2 = makeInvestment(63);
    p2->echo();

    auto p3 = makeInvestment(15);
    p3->echo();
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_factory_method_return_unique_ptr();

}


} // namespace item_18

} // namespace chapter_4

} // namespace effective_mordern_cpp

