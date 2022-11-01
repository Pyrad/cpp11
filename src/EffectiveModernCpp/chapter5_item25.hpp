#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM25
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM25

#include "../utilities/utilities.hpp"
#include <utility>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_25 {

class Foobar {
public:
    static uint32_t id_cnt;

public:
    Foobar() : m_id(id_cnt++), m_name("unknown") { }
    Foobar(const std::string &n) : m_id(id_cnt++), m_name(n) { }

    // Foobar(const Foobar &f) : m_id(id_cnt++), m_name(f.m_name) {
    //     fprintf(stdout, "Copy constructing Foobar object (id = %u)\n", m_id);
    // }

    /**
     * Since f is an rvalue reference, we'd like to use it's rvalue-ness
     * f.m_name is always an lvalue, so we need to "move" it by std::move
     */
    Foobar(Foobar &&f) : m_id(id_cnt++), m_name(std::move(f.m_name)) {
        fprintf(stdout, "Move constructing Foobar object (id = %u)\n", m_id);
    }
    ~Foobar() { }

public:
    void echo() const { fprintf(stdout, "A foo (ID = %u)\n", m_id); }

    /**
     * Since name is an universal reference, so we'd like to use its lvalue-ness
     * if it is bound to an lvalue, or to use its rvalue-nes if it is bound to an
     * rvalue, hence std::forward
     */
    template<typename T>
    void set_name(T &&name) { name = std::forward<T>(name); }

public:
    uint32_t m_id = 0;
    std::string m_name = "unknown";
};

/**
 * In order to keep its lvalue-ness or rvalue-ness, we use std::move
 * on rvalue reference, and we use std::forward on universal reference
 */
void test_std_move_on_rvalue_ref_std_forward_on_universal_ref();

void test_all();

} // namespace item_25

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM25
