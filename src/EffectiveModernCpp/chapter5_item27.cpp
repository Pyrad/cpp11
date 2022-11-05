#include "chapter5_item27.hpp"
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <chrono>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_27 {

/**
 * Just a demo function to show index to string convertion
 */
std::string name_from_index(int idx) {
    return (idx > 0) ? "Jim" : "Smith";
}

/**
 * A global variable to hold names
 */
static std::unordered_set<std::string> name_set;

/**
 * A function that uses a tag dispatch to forward argument to
 * propriet overloading functions, including the a template function
 * with a universal reference
 *
 * Why using std::remove_reference?
 * Because "int" and "int&" are different types.
 */
template<typename T>
void log_and_add(T &&name) {
    log_and_add_impl(std::forward<T>(name),
                     std::is_integral<typename std::remove_reference<T>::type>());
}

/**
 * @brief A function with a universal reference, and it expects the second
 *        argument to be a std::false_type object, which is of no use so compiler
 *        will optimize it out during compilition.
 *
 * @note, std::false_type and std::true_type are instantiations of std::integeral_constant.
 */
template<typename T>
void log_and_add_impl(T &&name, std::false_type /* no_use_arg */) {
    auto time_now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(time_now);
    std::cout << "[log_and_add_impl] Add " << name
              << ", time " << t_c << "\n";
    name_set.emplace(std::forward<T>(name));
}

/**
 * @brief An overloading function which accepts an integer and a std::true_type as
 *        a tag for dispatch use, similar to the function above, this std::true_type
 *        object will be optimized out during compilition.
 *
 * @note, std::false_type and std::true_type are instantiations of std::integeral_constant.
 */
void log_and_add_impl(int idx, std::true_type /* no_use_arg */) {
    std::string name(std::to_string(idx) + "_index");
    auto time_now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(time_now);
    std::cout << "[log_and_add_impl idx] Add " << name
              << ", time " << t_c << "\n";
    name_set.emplace(name);
}

/**
 * There are 5 ways to workaround the issue caused by overloading a template
 * function with a universal reference,
 * (1) No overloading. For example, use different function names.
 * (2) Pass by "const T&". But this will always lead to copy, which we don't expect.
 * (3) Pass by value. See EffectiveModernCpp item 41.
 * (4) Use tag dispatch. As functiony "log_and_add" shown here.
 * (5) Constraining templates for certain types.
 */
void test_tag_dispatch_workaround_overloading_univ_ref_issue() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Following call log_and_add_impl(T &&, std::false_type) at last
    std::string petName("Darla");
    log_and_add(petName);
    log_and_add(std::string("Persephone"));
    log_and_add("Patty Dog");

    // Following call log_and_add_impl(int, std::true_type) at last
    log_and_add(22);
    short i = 9;
    log_and_add(i);
    short &j = i;
    log_and_add(j);

} // test_tag_dispatch_workaround_overloading_univ_ref_issue

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_tag_dispatch_workaround_overloading_univ_ref_issue();
}


} // namespace item_27

} // namespace chapter_5

} // namespace effective_mordern_cpp

