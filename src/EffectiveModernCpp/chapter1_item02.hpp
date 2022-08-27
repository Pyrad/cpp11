#ifndef EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM02
#define EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM02

#include <iostream>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_1 {

namespace item_02 {

/**
 * @brief A template function to show the T CAN NOT be deduced from an std::initializer_list instance.
 *
 * @param[in] param Pass an instance of std::initializer_list will lead to failure
 * @param[out] no
 *
 * @return void
 *
 * @note This function is just to show that the T can't be deduced from an std::initializer_list instance'
 */
template<typename T>
void func_deduce_T(T param) {
    fprintf(stdout, "                Deduction for type T: %s\n", utilities::get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", utilities::get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_deduce_T

/**
 * @brief A template function to show that the T CAN be deduced from an std::initializer_list instance.
 *
 * @param[in] param Pass an instance of std::initializer_list will lead to failure
 * @param[out] no
 *
 * @return void
 *
 * @note This function is just to show that the T CAN be deduced from an std::initializer_list instance'
 */
template<typename T>
void func_deduce_initializer_list_T(std::initializer_list<T> param) {
    fprintf(stdout, "                Deduction for type T: %s\n", utilities::get_type_name<T>().c_str());
    fprintf(stdout, "Deduction for type of argument param: %s\n", utilities::get_type_name<decltype(param)>().c_str());
    fprintf(stdout, "\n");
} // func_deduce_initializer_list_T

/**
 * @brief To check what type of the auto is deduced.
 */
void test_auto_deduction();



/**
 * @brief An initializer might be deduced from auto type
 */
void test_auto_deduction_initializer();


} // namespace item_02

} // namespace chapter_1

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER1_ITEM02

