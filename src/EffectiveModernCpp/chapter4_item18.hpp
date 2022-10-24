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
 *        customized deleter
 *
 * @note, This is currently defined in the source file, which needs
 *        a lambda expression defined in source file to avoid mutiple
 *        definitions
 */
// static std::unique_ptr<UniInvestment, decltype(CusDelInvmt)> makeInvestment2(const double val);

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
void test_factory_method_return_unique_ptr_custom_deleter();


void test_all();

} // namespace item_18

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM18
