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
 * @brief A customized deleter for a std::unique_ptr
 */
auto CusDelInvmt = [](UniInvestment *p) {
    fprintf(stdout, "Customized deleting pointer to UniInvestment\n");
    delete p;
};

/**
 * @brief A factory method to return a std::unique_ptr with a
 *        customized deleter.
 *        This is C++11 style, because a lambda is defined outside
 *        of the function, and the return type is explicitly written.
 *
 * @note, This is currently defined in the source file, which needs
 *        a lambda expression defined in source file to avoid mutiple
 *        definitions
 */
std::unique_ptr<UniInvestment, decltype(CusDelInvmt)>
makeInvestment2(const double val) {
    std::unique_ptr<UniInvestment, decltype(CusDelInvmt)> p(nullptr, CusDelInvmt);

    if (val < 0) {
        p.reset(new UniInvestment(val));
    } else if (val > 100) {
        p.reset(new UniStock(val));
    } else if (val > 50) {
        p.reset(new UniBond(val));
    } else {
        p.reset(new UniRealEstate(val));
    }

    return p;
}

/**
 * @brief C++14 style to return a std::unique_ptr with a customized deleter
 */
auto makeInvestment3(const double val) {
    auto CusDelInvmt2 = [](UniInvestment *p) {
        fprintf(stdout, "Customized (2) deleting pointer to UniInvestment\n");
        delete p;
    };

    std::unique_ptr<UniInvestment, decltype(CusDelInvmt2)> p(nullptr, CusDelInvmt2);

    if (val < 0) {
        p.reset(new UniInvestment(val));
    } else if (val > 100) {
        p.reset(new UniStock(val));
    } else if (val > 50) {
        p.reset(new UniBond(val));
    } else {
        p.reset(new UniRealEstate(val));
    }

    return p;
}


/**
 * @brief A factory method makeInvestment returns a std::unique_ptr,
 *        which might point to different kind of inheritted classes
 */
void test_factory_method_return_unique_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    auto p0 = makeInvestment(-1);
    p0->echo();

    auto p1 = makeInvestment(102);
    p1->echo();

    auto p2 = makeInvestment(63);
    p2->echo();

    auto p3 = makeInvestment(15);
    p3->echo();
}

/**
 * @brief A factory method makeInvestment2 returns a std::unique_ptr with
 *        a customized deleter, this std::unique_ptr also can point to
 *        different kind of inheritted classes
 */
void test_factory_method_return_unique_ptr_custom_deleter_cxx11() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    auto p0 = makeInvestment2(-1);
    p0->echo();

    auto p1 = makeInvestment2(102);
    p1->echo();

    auto p2 = makeInvestment2(63);
    p2->echo();

    auto p3 = makeInvestment2(15);
    p3->echo();
}

/**
 * @brief A factory method makeInvestment3 returns a std::unique_ptr with
 *        a customized deleter, this std::unique_ptr also can point to
 *        different kind of inheritted classes, and makeInvestment3 is
 *        implemented in C++14 style, that is lambda deleter is defined
 *        inside the function object, but return type is actually a
 *        std::unique_ptr with that lambda as a customized deleter.
 *        (How this is achieved? Using "auto")
 */
void test_factory_method_return_unique_ptr_custom_deleter_cxx14() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    auto p0 = makeInvestment3(-1);
    p0->echo();

    auto p1 = makeInvestment3(102);
    p1->echo();

    auto p2 = makeInvestment3(63);
    p2->echo();

    auto p3 = makeInvestment3(15);
    p3->echo();
}

/**
 * If the std::unique_ptr has a default deleter (which is delete operator),
 * then the size of it equals to a raw pointer.
 * But if the std::unique_ptr has a customized deleter, then the size of it
 * increases from 1 word to 2 words.
 */
void test_size_of_unique_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // A pointer with a default deleter
    auto p0 = makeInvestment(10);

    // A pointer with a customized deleter
    auto p1 = makeInvestment2(10);

    // A pointer with a customized deleter
    auto p2 = makeInvestment3(10);

    fprintf(stdout, "size of p0 = %d\n", sizeof p0);
    fprintf(stdout, "size of p1 = %d\n", sizeof p1);
    fprintf(stdout, "size of p2 = %d\n", sizeof p2);

}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_factory_method_return_unique_ptr();

    test_factory_method_return_unique_ptr_custom_deleter_cxx11();

    test_factory_method_return_unique_ptr_custom_deleter_cxx14();


    test_size_of_unique_ptr();
}


} // namespace item_18

} // namespace chapter_4

} // namespace effective_mordern_cpp

