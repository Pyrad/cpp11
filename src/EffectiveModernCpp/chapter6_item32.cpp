#include "chapter6_item32.hpp"
#include <memory>
#include <stdio.h>

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_32 {

/**
 * As we know, C++11 doesn't support move capture, C++14 enhanced this
 * and C++14 now support move capture.
 *
 * What are the 2 cases which C++14 support while C++11 doesn't?
 * --------------------------------------------------------------
 * (1) Move a move-only object into a closure (e.g., std::unique_ptr, std::future)
 * (2) Move an object whose move operation is more effective than its copy operation
 *     to a closure, for example, containers in STL.
 *
 * C++14 introduced a mechanism called "init capture", which is also called
 * "generalized lambda capture".
 *
 * What can a generalized lambda capture explicitly specify?
 * --------------------------------------------------------------
 * (1) Names for the data members in the closure generated by the lambda
 * (2) An expression initializing the data members
 */
void test_cxx14_supports_move_capture() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // Create a unique_ptr
    auto pw = std::make_unique<Foobar>(10, "ocean");
    // C++14 style lambda, init data member in closure w/ std::move(pw)
    // 'pw' on the left side of "=" is the data member in closure created from lambda, 
    // while the 'pw' on the left side of "=" is the expression to initialize it (left
    // side 'pw').
    // They are 2 different variables and have different definition scope, and this
    // moves a local variable into a closure
    auto func = [pw = std::move(pw)] { return pw->is_validated() && pw->is_archived(); };
    fprintf(stdout, "Foobar is validated and archived? %s\n", func() ? "True" : "False");

} // test_cxx14_supports_move_capture

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_cxx14_supports_move_capture();
}


} // namespace item_32

} // namespace chapter_6

} // namespace effective_mordern_cpp

