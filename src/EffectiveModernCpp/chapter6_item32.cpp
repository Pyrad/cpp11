#include "chapter6_item32.hpp"
#include <functional>
#include <memory>
#include <stdio.h>
#include <utility>
#include <vector>

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

/**
 * The essence of a lambda is that have compiler create a class and create
 * an object of that class by using an easy way.
 *
 * There's nothing you can do with a lambda that you can't do by hand.
 */
void test_cxx11_achieve_move_capture_by_class() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    auto pw = std::make_unique<Foobar>(10, "ocean");
    auto func = IsValAndArch(std::move(pw));
    fprintf(stdout, "Foobar is validated and archived? %s\n", func() ? "True" : "False");

} // test_cxx11_achieve_move_capture_by_class

/**
 * Using std::bind in C++11 to achieve move capture
 */
void test_cxx11_achieve_move_capture_by_std_bind() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    std::vector<int> ivec{0, 1, 2, 3, 4};
    fprintf(stdout, "[Before move capture] Size of ivec = %lu\n", ivec.size());

    // C++11 emulation of init capture
    // (1) A so-called "bind" object will be created by std::bind
    // (2) The first argument of std::bind is a callable object, and the following arguments
    //     are the arguments for that callable object
    // (3) The arguments copied to the bind object created by std::bind are either copy
    //     constructed if lvalue, or move constructed if rvalue.
    // (4) When the bind object is called, the callable object stored in this bind object
    //     will be called, and the arguments will be passed to it in sequence.
    // (5) The member function "operator()()" created by lambda expression is "const", i.e.,
    //     "operator()() const", so the closure created by the lambda expression has constant
    //     data members (const).
    // (6) If declaring the lambda expression as "mutable", then the member function "operator()()"
    //     created by it won't be "const" anymore, i.e., it is "operator()()", not "operator()() const".
    //     For example,
    auto func = std::bind([](const std::vector<int> &data) {
                            for (auto i : data) { fprintf(stdout, "i = %d\n", i); }
                          },
                          std::move(ivec));
    fprintf(stdout, "[After move capture] Size of ivec = %lu\n", ivec.size());

    // Use func to access the data it moved 'data' into it
    func();


    std::vector<int> ivec2{0, 1, 2, 3, 4};
    fprintf(stdout, "[Before move capture] Size of ivec2 = %lu\n", ivec2.size());
    // (6) If declaring the lambda expression as "mutable", then the member function "operator()()"
    //     created by it won't be "const" anymore, i.e., it is "operator()()", not "operator()() const".
    //     For example,
    auto func2 = std::bind([](std::vector<int> &data) mutable {
                            for (auto i : data) { fprintf(stdout, "i = %d\n", i); }
                          },
                          std::move(ivec2));
    fprintf(stdout, "[After move capture] Size of ivec2 = %lu\n", ivec2.size());


} // test_cxx11_achieve_move_capture_by_std_bind

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_cxx14_supports_move_capture();

    test_cxx11_achieve_move_capture_by_class();

    test_cxx11_achieve_move_capture_by_std_bind();
}


} // namespace item_32

} // namespace chapter_6

} // namespace effective_mordern_cpp

