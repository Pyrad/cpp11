#include "chapter5_item26.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_set>
#include <iomanip>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_26 {

/**
 * A global variable to hold names
 */
static std::unordered_set<std::string> name_set;

/**
 * A normal function to accept a name
 */
void log_and_add(const std::string &name) {
    auto time_now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(time_now);
    std::cout << "[log_and_add] Add " << name
              << ", time " << t_c << "\n";
    name_set.emplace(name);
}

/**
 * A function with a universal reference to accept a name
 * This is actually to replace the normal function
 * "void log_and_add(const std::string &name);"
 */
template<typename T>
void log_and_add_new(T &&name) {
    auto time_now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(time_now);
    std::cout << "[log_and_add_new] Add " << name
              << ", time " << t_c << "\n";
    name_set.emplace(std::forward<T>(name));
}

/**
 * An overloading function to accept just an integer
 */
void log_and_add_new(int32_t idx) {
    std::string name(std::to_string(idx) + "_index");
    auto time_now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(time_now);
    std::cout << "[log_and_add_new idx] Add " << name
              << ", time " << t_c << "\n";
    name_set.emplace(name);
}

/**
 * Shows an overloading function with an universal reference sometimes
 * results in unexpected consequences, compared with a normal function
 */
void test_use_univ_ref_to_optimize_3_calls() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    std::string petName("Darla");
    // Pass an lvalue "petName" to log_and_add, finally "petName" is passed
    // to "name_set.emplace(name)", and then it is copied to "name_set"
    log_and_add(petName);
    // Pass a temporary variable to log_and_add, similarly this temp variable
    // is passed to "name_set.emplace(name)", and then it is copied to "name_set"
    // But because it is an rvalue, it can be moved to "name_set" instead.
    log_and_add(std::string("Persephone"));
    // Pass a "char*" to log_and_add, it first creates a temp variable (type is
    // std::string), and then similarly it is passed to "name_set.emplace(name)",
    // finally it is copied to "name_set". But because std::string has a constructor
    // which accepts "char*", so actually it can be optimized here.
    log_and_add("Patty Dog");

    // As analysis above, it's easy to think about using a function with a universal
    // reference to optimize, thus we defined a function with a universal reference,
    log_and_add_new(petName);
    log_and_add_new(std::string("Persephone"));
    log_and_add_new("Patty Dog");

    // Beside a function with a universal reference, we also have an overloading function
    // which accepts an integer. The following calls "log_and_add_new(int32_t)", not
    // "log_and_add_new(T &&)", why?
    // Because "22" is an rvalue, the template function after instantiation is
    // "log_and_add_new(int &&)", which is as good as normal function
    // "void log_and_add_new(int32_t idx);", and C++ standard points that in this circumstances,
    // (template after instantiation is as good as an overloading normal function), the normal
    // function wins.
    log_and_add_new(22);
} // end function test_use_univ_ref_to_optimize_3_calls

/**
 * Overloading a function with an universal reference might cause unexpected
 * error
 */
void test_overloading_univ_ref_causes_error() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // The following lines cause a compile error, but why?
    // ----------------------------------------------------------------
    // Because "log_and_add_new" is a overloading function, and one of those
    // overloadings is a template function with a universal reference.
    // For the following function call, this template function will be instantiated
    // as "log_and_add_new(short &&)", while the other overloading function which
    // is a normal function, "log_and_add_new(int)".
    // If to invoke "log_and_add_new(int)", then this "name_idx" should be promoted
    // from "short" type to "int" type, but this kind of promotion is not needed for
    // the instantiated function "log_and_add_new(short &&)", thus this function
    // instantiation is called, and it finally tries to insert an integer into a
    // std::unordered_set<std::string>, while std::string has no constructor function
    // accepting an integer, so it fails at last.
    // ----------------------------------------------------------------
#if 0
    short name_idx = 0;
    log_and_add_new(name_idx);
#endif // 0

    // Why does the following work?
    // ----------------------------------------------------------------
    // See the analysis above, pay attention to "name_idx", which now is an integer, not
    // a short integer
    int name_idx = 0;
    log_and_add_new(name_idx);

} // test_overloading_univ_ref_causes_error

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_use_univ_ref_to_optimize_3_calls();

    test_overloading_univ_ref_causes_error();
}


} // namespace item_26

} // namespace chapter_5

} // namespace effective_mordern_cpp

