#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM29
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM29

#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_29 {

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
}; // end class Foobar

/**
 * Sometimes the move operations are not as fast as we expect
 * Scott gave 2 examples, std::array and std::string.
 *
 * Moving a std::array object is not as fast as we think, that's
 * because the memory allocated for this std::array object is on stack,
 * not on heap. So moving a std::array is indeed copying it.
 *
 * Moving std::string sometimes has a similar situation. Because today
 * compilers will do Small String Optimization (SSO), so if the char string
 * is short enough, then it is allocated on stack instead of on heap. Thus
 * same result as the analysis above.
 */
void test_move_op_not_fast();

void test_all();

} // namespace item_29

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM29
