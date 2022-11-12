#include "chapter6_item31.hpp"
#include <functional>
#include <memory>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_31 {

// Define a global filter whose elements are function objects
using FilterContainer = std::vector<std::function<bool(int)>>;
FilterContainer g_filters;

/**
 * A class to show that lambda doesn't capture static variables and
 * data members
 */
class WidgetCapByValueUB {
public:
    WidgetCapByValueUB() { }

    void add_filter() const {
        // Default capture by value, the following actually doesn't capture the
        // data member 'divisor' (because lambda doesn't capture static variables
        // and data members), indeed it captured 'this'
        g_filters.emplace_back([=](int value) { return value % divisor == 0; });

        // The following 2 won't compile, because lambda doesn't capture static variables
        // and data members.
        // -------------------------
        // g_filters.emplace_back([](int value) { return value % divisor == 0; });
        // g_filters.emplace_back([divisor](int value) { return value % divisor == 0; });
    }

private:
    int divisor = 0;
}; // end class WidgetCapByValueUB

/**
 * What is a lambda ?
 * --------------------------------- 
 * A lambda is just an expression, which is a part of the source code.
 * Compiler generates an unique closure type/class for each lambda expression.
 *
 * What is a closure ?
 * --------------------------------- 
 * A closure is a runtime object, it is an instance of a closure class/type.
 * A closure can be copied, so there might be a few closures which are instances
 * of a same closure class.
 *
 * What is a closure class?
 * --------------------------------- 
 * A closure class is a class, from which a closure is instantiated.
 * A closure class may have more than 1 instances, which are closures. (Because
 * a closure can be copied)
 */
void test_what_is_lambda_and_closure() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    int x = 25;

    // c0 is a copy of the closure produced by the lambda
    auto c0 = [x](int y) { return x * y > 55; };
    // c1 is a copy of c0
    auto c1 = c0;
    // c2 is a copy of c1
    auto c2 = c1;

} // test_what_is_lambda_and_closure

/**
 * Lambda has 2 types of default capture mode
 * (1) Capture by reference
 * (2) Capture by value
 * 
 * Both capture mode can lead to the dangling issue. Why?
 * Because lambda only captures the local variables or references, and these
 * variables or references are defined where the lambda exists.
 * It won't capture static variables or data members.
 * In other words, if the life time of those local variables and references
 * run out, the captured references or variables are dangling.
 */
void test_capture_by_ref_dangling() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    int var = 100;
    // DANGER!
    // A lambda that captures a reference of a local variable,
    // and then it is stored in a global container.
    // When it goes out of this function scope, that local variable
    // no longer exists, thus dangling issue
    g_filters.emplace_back([&](int value){ return value % var; });

    // DANGER!
    // Though captured explicitly, it still becomes invalid when using
    // outside this function scope
    g_filters.emplace_back([&var](int value){ return value % var; });

} // test_capture_by_ref_dangling

/**
 * @brief A function to show capture by value still might lead to dangling issue
 */
void test_capture_by_value_dangling() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    WidgetCapByValueUB val;
    // See comments of function WidgetCapByValueUB::add_filter, that the lambda it
    // stores actually captures 'this' pointer
    val.add_filter();

    // Since the lambda in WidgetCapByValueUB::add_filter actually captures 'this'
    // pointer, thus after 'pw' is destroyed, the lambdas hold by the g_filters
    // now dangles.
    auto pw = std::make_unique<WidgetCapByValueUB>();
    pw->add_filter();

    // After reset 'pw', now dangling issue appears
    pw = nullptr;
    
} // test_capture_by_value_dangling

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_what_is_lambda_and_closure();

    test_capture_by_ref_dangling();
}


} // namespace item_31

} // namespace chapter_6

} // namespace effective_mordern_cpp

