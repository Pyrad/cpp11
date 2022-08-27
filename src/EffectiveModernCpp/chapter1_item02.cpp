#include "chapter1_item02.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_02 {

/**
 * @brief A simple function to be used as a function pointer
 * 
 * @param[in] i no use indeed
 * @param[out] d no use indeed
 *
 * @return an integer of no use indeed.
 *
 * @note This function acts as a function pointer to be used later
 */
int func_test_sample(int i, double d) {
    double x = i + d;
    fprintf(stdout, "This is an example of function\n");

    return int(x);
} //func_test_sample


/**
 * @brief An initializer might be deduced from auto type
 */
void test_auto_deduction_initializer() {
    auto a = 27;
    auto b(27);
    auto c = {27};
    auto d{27};

    fprintf(stdout, "---------------\n");
    fprintf(stdout, "auto a = 27; <type of 'a': %s>\n", utilities::get_type_name<decltype(a)>().c_str());
    fprintf(stdout, "auto b(27); <type of 'b': %s>\n", utilities::get_type_name<decltype(b)>().c_str());
    fprintf(stdout, "auto c = {27}; <type of 'c': %s>\n", utilities::get_type_name<decltype(c)>().c_str());
    fprintf(stdout, "auto d{27}; <type of 'd': %s>\n", utilities::get_type_name<decltype(d)>().c_str());

} // test_auto_deduction_initializer

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
    fprintf(stdout, "---------------\n");
    fprintf(stdout, "auto z = 10; <type of 'z': %s>\n", utilities::get_type_name<decltype(z)>().c_str());
    fprintf(stdout, "const auto cz = 20; <type of 'cz': %s>\n", utilities::get_type_name<decltype(cz)>().c_str());

    // --------------------------------------------------------------
    // Array-decay-to-pointer rule
    // --------------------------------------------------------------
    char const name[] = "R. N. Briggs"; // name's type is const char[13]
    auto arr1 = name;
    auto &arr2 = name;
    fprintf(stdout, "---------------\n");
    fprintf(stdout, "name = %s\n", name);
    fprintf(stdout, "auto arr1 = name; <type of 'arr1': %s>\n", utilities::get_type_name<decltype(arr1)>().c_str());
    fprintf(stdout, "auto &arr2 = name; <type of 'arr2': %s>\n", utilities::get_type_name<decltype(arr2)>().c_str());

    // --------------------------------------------------------------
    // Function-decay-to-pointer rule
    // --------------------------------------------------------------
    auto func1 = func_test_sample;
    auto &func2 = func_test_sample;
    fprintf(stdout, "---------------\n");
    fprintf(stdout, "auto func1 = func_test_sample; <type of 'func1': %s>\n", utilities::get_type_name<decltype(func1)>().c_str());
    fprintf(stdout, "auto &func2 = func_test_sample; <type of 'func2': %s>\n", utilities::get_type_name<decltype(func2)>().c_str());


    // --------------------------------------------------------------
    // Sometimes auto will be deduced as an initializer
    // --------------------------------------------------------------
    test_auto_deduction_initializer();

    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
} // test_auto_deduction





} // namespace item_02

} // namespace chapter_1

} // namespace effective_mordern_cpp

