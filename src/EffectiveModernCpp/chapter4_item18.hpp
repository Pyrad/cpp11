#ifndef EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM18
#define EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM18

#include "../utilities/utilities.hpp"
#include <memory>
#include <stdint.h>
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_18 {

/**
 * @brief A base class to be inheritted by other 3 classes
 */
class UniInvestment {
public:
    UniInvestment() : m_value(0) { }
    UniInvestment(double val) : m_value(val) { }
    virtual ~UniInvestment() { }

public:
    virtual void echo() const { fprintf(stdout, "UniInvestment\n"); }

private:
    double m_value;
};

// Note define this lambda in the source file to avoid mutiple
// definitions issue
// /**
//  * @brief A customized deleter for a std::unique_ptr
//  */
// auto CusDelInvmt = [](UniInvestment *p) {
//     fprintf(stdout, "Customized deleting pointer to UniInvestment\n");
//     delete p;
// };

/**
 * @brief Inheritted class 1
 */
class UniStock : public UniInvestment {
public:
    UniStock() : UniInvestment(), m_sid(0) { }
    UniStock(double val) : UniInvestment(val), m_sid(0) { }
    ~UniStock() { }

public:
    void echo() const { fprintf(stdout, "UniStock\n"); }

private:
    int32_t m_sid;
};

/**
 * @brief Inheritted class 2
 */
class UniBond : public UniInvestment {
public:
    UniBond() : UniInvestment(), m_bid(0) { }
    UniBond(double val) : UniInvestment(val), m_bid(0) { }
    ~UniBond() { }

public:
    void echo() const { fprintf(stdout, "UniBond\n"); }

private:
    int32_t m_bid;
};

/**
 * @brief Inheritted class 3
 */
class UniRealEstate : public UniInvestment {
public:
    UniRealEstate() : UniInvestment(), m_rid(0) { }
    UniRealEstate(double val) : UniInvestment(val), m_rid(0) { }
    ~UniRealEstate() { }

public:
    void echo() const { fprintf(stdout, "UniRealEstate\n"); }

private:
    int32_t m_rid;
};

/**
 * @brief A factory method to return a std::unique_ptr
 *
 * @note, std::unique_ptr is available after C++14, it is not
 *        available in C++11
 */
std::unique_ptr<UniInvestment> makeInvestment(const double val);

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
// static std::unique_ptr<UniInvestment, decltype(CusDelInvmt)> makeInvestment2(const double val);


auto makeInvestment3(const double val);

/**
 * @brief A factory method makeInvestment returns a std::unique_ptr,
 *        which might point to different kind of inheritted classes
 */
void test_factory_method_return_unique_ptr();

/**
 * @brief A factory method makeInvestment2 returns a std::unique_ptr with
 *        a customized deleter, this std::unique_ptr also can point to
 *        different kind of inheritted classes
 */
void test_factory_method_return_unique_ptr_custom_deleter_cxx11();


/**
 * @brief A factory method makeInvestment3 returns a std::unique_ptr with
 *        a customized deleter, this std::unique_ptr also can point to
 *        different kind of inheritted classes, and makeInvestment3 is
 *        implemented in C++14 style, that is lambda deleter is defined
 *        inside the function object, but return type is actually a
 *        std::unique_ptr with that lambda as a customized deleter.
 *        (How this is achieved? Using "auto")
 */
void test_factory_method_return_unique_ptr_custom_deleter_cxx14();

/**
 * If the std::unique_ptr has a default deleter (which is delete operator),
 * then the size of it equals to a raw pointer.
 * But if the std::unique_ptr has a customized deleter, cases are different,
 * (1) If the customized deleter is a lambda w/o any capture, then the size
 *     of it is still equal to a raw pointer
 * (2) If the customized deleter is a lambda w/ any capture, then the size
 *     of it increases
 * (3) If the customized deleter is a function pointer, then the size of it
 *     increases from 1 word to 2 words.
 */
void test_size_of_unique_ptr();

/**
 * A function deleter for std::unique_ptr
 */
void CusDelInvmt3(UniInvestment *p);

/**
 * std::unique_ptr has 2 types, the 2nd form is not recommended, because
 * std::array, std::vector are all good ones to replace it.
 *
 * (1) std::unique_ptr<T>, which points to a single variable
 *     This form has dereference operator, but doesn't have operator[]
 * (2) std::unique_ptr<T[]>, which points to an array
 *     This form has operator[], but doesn't have dereference operator
 */
void test_2_types_of_unique_ptr();

/**
 * @brief Shows that a std::unique_ptr can be implicitly converted to
 *        a std::shared_ptr, which is good to act as a return type of
 *        a factory method
 */
void test_unique_ptr_implicitly_convert_to_shared_ptr();

void test_all();

} // namespace item_18

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM18
