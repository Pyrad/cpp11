#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM28
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM28

#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_28 {

class Foobar {
public:
    Foobar() = default;
    Foobar(const std::string &n) : m_name(n) { }

public:
    static Foobar make(const std::string &n) {
        Foobar obj(n);
        return obj;
    }

private:
    std::string m_name = "unkown";
};

/**
 * The encode mechanism
 * ---------------------
 * (1) If an lvalue is passed in, type "T" is deduced as lvalue reference
 * (2) If an rvalue is passed in, type "T" is deduced as non-reference type
 */
template<typename T>
void encode_mechanism_func(T &&param);

/**
 * @brief Shows the deduced types of the parameter passed into a function with
 *        a universal reference
 */
void show_deduced_types_of_universal_ref();

void test_all();

} // namespace item_28

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM28
