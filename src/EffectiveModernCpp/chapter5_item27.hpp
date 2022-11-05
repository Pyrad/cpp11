#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM27
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM27

#include "../utilities/utilities.hpp"
#include <string>
#include <type_traits>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_27 {

/**
 * Just a demo function to show index to string convertion
 */
std::string name_from_index(int idx);

/**
 * A function that uses a tag dispatch to forward argument to
 * propriet overloading functions, including the a template function
 * with a universal reference
 */
template<typename T>
void log_and_add(T &&name);

/**
 * @brief A function with a universal reference, and it expects the second
 *        argument to be a std::false_type object, which is of no use so compiler
 *        will optimize it out during compilition.
 *
 * @note, std::false_type and std::true_type are instantiations of std::integeral_constant.
 */
template<typename T>
void log_and_add_impl(T &&name, std::false_type /* no_use_arg */);

/**
 * @brief An overloading function which accepts an integer and a std::true_type as
 *        a tag for dispatch use, similar to the function above, this std::true_type
 *        object will be optimized out during compilition.
 *
 * @note, std::false_type and std::true_type are instantiations of std::integeral_constant.
 */
void log_and_add_impl(int idx, std::true_type /* no_use_arg */);

/**
 * @brief A class to show overloading a ctor with a universal reference
 *        might cause more compile errros, and there's ways to workaround it.
 */
class Person {
public:
    /**
     * Following is the C++11 style, if using C++14, use the following style
     *
     * C++14 style
     * ----------------------
     * template<typename T,
     *          typename = std::enable_if_t<
     *                        !std::is_base_of<Person, std::decay_t<T>>::value
     *                        && 
     *                        !std::is_integral<std::remove_reference_t<T>>::value
     *                     >
     *          >
     * explicit Person(T &&n);
     *
     *
     * C++11 style
     * ----------------------
     * template<typename T,
     *          typename = typename std::enable_if<
     *                                 !std::is_base_of<Person, typename std::decay<T>::type>::value
     *                                 && 
     *                                 !std::is_integral<typename std::remove_reference<T>::type>::value
     *                     >::type
     *          >
     *
     * As we can see, C++14 is more simple.
     */
    template<typename T,
             typename = std::enable_if_t<
                           !std::is_base_of<Person, std::decay_t<T>>::value
                           && 
                           !std::is_integral<std::remove_reference_t<T>>::value
                        >
             >
    explicit Person(T &&n) : m_name(std::forward<T>(n)) {
        // Use a static_assert to test if the T type object can be used to construct
        // a std::string object, otherwise compiler won't know that T object can't
        // be used to construct a std::string until that T object finally got passed to the
        // final function, at this circumstances, a very very very long message will
        // be thrown to you, and you will get quite confused on what happened...
        static_assert(std::is_constructible<std::string, T>::value,
                      "Parameter n can't be used to construct a std::string");

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

}; // end class Person

/**
 * There are 5 ways to workaround the issue caused by overloading a template
 * function with a universal reference,
 * (1) No overloading. For example, use different function names.
 * (2) Pass by "const T&". But this will always lead to copy, which we don't expect.
 * (3) Pass by value. See EffectiveModernCpp item 41.
 * (4) Use tag dispatch. As functiony "log_and_add" shown here.
 * (5) Constraining templates for certain types.
 */
void test_tag_dispatch_workaround_overloading_univ_ref_issue();

/**
 * @brief Constraining template types for a template function with
 *        a universal reference can be useful sometimes, because it
 *        tells the compiler not to use the instantiations of that
 *        template function for a specified template types, thus
 *        avoiding some issues caused by unexpected function overloading
 */
void test_contraining_template_type_taking_univ_ref();

void test_all();

} // namespace item_27

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM27
