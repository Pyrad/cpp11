#include "chapter1_item02.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_02 {


/**
 * @brief To check what type of the auto is deduced.
 */
void test_auto_deduction() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);

    // --------------------------------------------------------------
    // Case 1: A reference or pointer, but not a universal reference
    // --------------------------------------------------------------
    int x = 27;             // define an integer
    auto &rx = x;           // A reference (not universal reference)
    const auto &crx = x;    // A const reference (not universal reference)
    // auto &&rrx = 23;     // No '&&' here, as it is an universal reference
    auto *px = &x;          // A pointer
    const auto *cpx = &x;   // A const pointer
    fprintf(stdout, "---------------\n");
    fprintf(stdout, "auto x = 27; <type of 'x': %s>\n", utilities::get_type_name<decltype(x)>().c_str());
    fprintf(stdout, "auto &rx = x; <type of 'rx': %s>\n", utilities::get_type_name<decltype(rx)>().c_str());
    fprintf(stdout, "const auto &crx = x; <type of 'crx': %s>\n", utilities::get_type_name<decltype(crx)>().c_str());
    fprintf(stdout, "auto *px = &x; <type of 'px': %s>\n", utilities::get_type_name<decltype(px)>().c_str());
    fprintf(stdout, "const auto *cpx = &x; <type of 'cpx': %s>\n", utilities::get_type_name<decltype(cpx)>().c_str());

    // --------------------------------------------------------------
    // Case 2: An universal reference
    // --------------------------------------------------------------
    int y = 23;
    auto &&urfy0 = y;
    auto &&urfy1 = 23;
    auto &&urfy2 = std::move(y);
    auto &&urfy3 = rx;
    auto &&urfy4 = urfy0;
    auto &&urfy5 = crx;
    fprintf(stdout, "---------------\n");
    fprintf(stdout, "auto &&urfy0 = y; <type of 'urfy0': %s>\n", utilities::get_type_name<decltype(urfy0)>().c_str());
    fprintf(stdout, "auto &&urfy1 = 23; <type of 'urfy1': %s>\n", utilities::get_type_name<decltype(urfy1)>().c_str());
    fprintf(stdout, "auto &&urfy2 = std::move(y); <type of 'urfy2': %s>\n", utilities::get_type_name<decltype(urfy2)>().c_str());
    fprintf(stdout, "auto &&urfy3 = rx; <type of 'urfy3': %s>\n", utilities::get_type_name<decltype(urfy3)>().c_str());
    fprintf(stdout, "auto &&urfy4 = urfy0; <type of 'urfy4': %s>\n", utilities::get_type_name<decltype(urfy4)>().c_str());
    fprintf(stdout, "auto &&urfy5 = crx; <type of 'urfy5': %s>\n", utilities::get_type_name<decltype(urfy5)>().c_str());

    // --------------------------------------------------------------
    // Case 3: Neither a pointer/reference nor an universal reference
    // --------------------------------------------------------------
    auto z = 10;
    const auto cz = 20;
    fprintf(stdout, "auto z = 10; <type of 'z': %s>\n", utilities::get_type_name<decltype(z)>().c_str());
    fprintf(stdout, "const auto cz = 20; <type of 'cz': %s>\n", utilities::get_type_name<decltype(cz)>().c_str());


    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
} // test_auto_deduction





} // namespace item_02

} // namespace chapter_1

} // namespace effective_mordern_cpp

