#ifndef EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM25
#define EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM25

#include "../utilities/utilities.hpp"
#include <stdio.h>
#include <string>
#include <utility>

namespace effective_mordern_cpp {

namespace chapter_5 {

namespace item_25 {

class Foobar {
public:
    static uint32_t id_cnt;

public:
    Foobar() : m_id(id_cnt++), m_name("unknown"), m_value(0) { }
    Foobar(const std::string &n) : m_id(id_cnt++), m_name(n), m_value(0) { }
    Foobar(const std::string &n, const double val) : m_id(id_cnt++), m_name(n), m_value(val) { }

    /**
     * Since f is an rvalue reference, we'd like to use it's rvalue-ness
     * f.m_name is always an lvalue, so we need to "move" it by std::move
     */
    Foobar(Foobar &&f) : m_id(id_cnt++), m_name(std::move(f.m_name)), m_value(f.m_value) {
        fprintf(stdout, "Move constructing Foobar object (id = %u)\n", m_id);
    }
    ~Foobar() { }

public:
    /**
     * When to use std::move on function's return value?
     * -------------------------------------------------
     * (1) The form is return-by-value
     * (2) An rvalue reference is returned, or the universal
     *     reference passed in is returned.
     */
    static Foobar add_name(Foobar &&lhs, const Foobar &rhs) {
        fprintf(stdout, "[Foobar::add_name] Start adding name of 2 Foobar objects\n");
        lhs.check_set_name(lhs.name() + "_" + rhs.name());
        fprintf(stdout, "[Foobar::add_name] Added name to LeftHandSide Foobar object\n");
        return std::move(lhs);
    }

public:
    void echo() const {
        fprintf(stdout, "A foo (ID = %u, name = %s)\n", m_id, m_name.c_str());
    }
    
    const std::string &name() const { return m_name; }

    bool check_name(const std::string &name) {
        if (name.empty()) {
            fprintf(stdout, "[Foobar::check_set_name] Error, empty name given\n");
            return false;
        }
        if (name == "cold") {
            fprintf(stdout, "[Foobar::check_set_name] Invalid name given: cold\n");
            return false;
        }

        return true;
    }

    /**
     * Since name is an universal reference, so we'd like to use its lvalue-ness
     * if it is bound to an lvalue, or to use its rvalue-nes if it is bound to an
     * rvalue, hence std::forward
     */
    template<typename T>
    void set_name(T &&name) { m_name = std::forward<T>(name); }

    /**
     * Shows how to use an universal reference mutiple times in a single function
     */
    template<typename T>
    void check_set_name(T &&name) {
        if (!check_name(name)) { // Function check_name's argument is "const std::string&"
            fprintf(stdout, "[Foobar::check_set_name] Skip setting name as check failed\n");
            return ;
        }
        fprintf(stdout, "[Foobar::check_set_name] Prepare to set name %s\n", name.c_str());
        // Function set_name's argument is "T&&", which is an universal reference
        set_name(std::forward<T>(name));
        fprintf(stdout, "[Foobar::check_set_name] Successfully checked & set name\n");
    }

public:
    uint32_t m_id = 0;
    std::string m_name = "unknown";
    double m_value = 0;
};

/**
 * In order to keep its lvalue-ness or rvalue-ness, we use std::move
 * on rvalue reference, and we use std::forward on universal reference
 */
void test_std_move_on_rvalue_ref_std_forward_on_universal_ref();

/**
 * A function to show how to use a universal reference mutiple times
 */
void test_use_universal_ref_mutiple_times();

/**
 * std::move, std::forward and universal reference
 *
 * 1. Don't use std::forward on an rvalue
 *    Indeed it can be use, but it is redundant and not necessary...
 * 2. Don't use std::move on an universal reference
 *    Because you might move an lvalue! That sometimes is not expected.
 * 3. Don't use overloading functions to replace universal references
 *    Reason (1), more code, not a convention
 *    Reason (2), potential performance degradation
 *    Reason (3), it's impossible to accomadate variadic argument, but universal reference
 *    can be used in this circumstances.
 */
void test_some_rules_of_move_forward_univ_ref();

/**
 * See function Foobar::add_name
 *
 * When to use std::move on function's return value?
 * -------------------------------------------------
 * (1) The form is return-by-value
 * (2) An rvalue reference is returned, or the universal
 *     reference passed in is returned.
 */
void test_use_move_forward_on_return_values();

void test_all();

} // namespace item_25

} // namespace chapter_5

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER5_ITEM25
