#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM26
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM26

#include "../utilities/utilities.hpp"
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <utility>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_26 {

/**
 * Just a demo function to show index to string convertion
 */
std::string name_from_index(int idx);

class Person {
public:
    template<typename T>
    explicit Person(T &&n) : m_name(std::forward<T>(n)) {
        fprintf(stdout, "[Class Person] ctor with universal ref is called\n");
    }

    explicit Person(int idx) : m_name(name_from_index(idx)) {
        fprintf(stdout, "[Class Person] ctor with integer is called\n");
    }

    void echo() const {
        fprintf(stdout, "[Class Person] name = %s\n", m_name.c_str());
    }

private:
    std::string m_name;
};

/**
 * A normal function to accept a name
 */
void log_and_add(const std::string &name);

/**
 * A function with a universal reference to accept a name
 * This is actually to replace the normal function
 * "void log_and_add(const std::string &name);"
 */
template<typename T>
void log_and_add_new(T &&name);

/**
 * An overloading function to accept just an integer
 */
void log_and_add_new(int32_t idx);

/**
 * Shows an overloading function with an universal reference sometimes
 * results in unexpected consequences, compared with a normal function
 */
void test_use_univ_ref_to_optimize_3_calls();

/**
 * Overloading a function with an universal reference might cause unexpected
 * error
 */
void test_overloading_univ_ref_causes_error();

/**
 * Overloading a ctor with an universal reference might cause unexpected
 * error too
 */
void test_overloading_univ_ref_ctor_causes_error();

void test_all();

} // namespace item_26

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM26
