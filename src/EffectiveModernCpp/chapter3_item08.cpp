#include "chapter3_item08.hpp"
#include <stdio.h>
#include <mutex>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_08 {


/**
 * When 0 or NULL is passed in, it's ok, since they are implicitly
 * converted to void* by the compiler grugdingly.
 */
int f1(std::shared_ptr<foo> spw) {
    fprintf(stdout, "This is f1(std::shared_ptr<foo> spw)\n");
    return 0;
}
/**
 * Same as above, when 0 or NULL is passed in, it's ok, since they are
 * implicitly converted to void* by the compiler grugdingly.
 */
double f2(std::unique_ptr<foo> upw) {
    fprintf(stdout, "This is f2(std::unique_ptr<foo> upw)\n");
    return 0;
}

bool f3(foo* pw) { fprintf(stdout, "This is f3(foo* pw)\n"); return true; }

/**
 * Following 3 overloaded functions show that if 0 or NULL is passed
 * in, then only the 1st and the 2nd will be called, the 3rd will
 * never be called.
 * But it makes a difference if nullptr is used.
 */
void myfoo(int i) { fprintf(stdout, "This is myfoo(int i)\n"); }
void myfoo(bool b) { fprintf(stdout, "This is myfoo(bool b)\n"); }
void myfoo(void*) { fprintf(stdout, "This is myfoo(void*)\n"); }


/**
 * Shows how 0 and NULL play tricks on overloaded functions if the parameter
 * is a pointer, which also shows nullptr can avoid such issues.
 */
void test_zero_and_null_overloaded_functions() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);
    myfoo(0); // calls myfoo(int i)
    /* myfoo(NULL); */ // Comment this as it doesn't compile
    myfoo(nullptr); // calls myfoo(void*)
} // test_zero_and_null_overloaded_functions


void test_nullptr_with_no_template() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);
    std::mutex f1m, f2m, f3m;
    using MuxGuard = std::lock_guard<std::mutex>;
    {
        MuxGuard g(f1m);
        // Though f1 accepts std::shared_ptr, while it works
        // because 0 will be converted to void* by the compiler
        // grugdingly.
        auto result = f1(0);
    }
    {
        MuxGuard g(f2m);
        // Same as above, though f2 accepts std::shared_ptr, while
        // it works because NULL will be converted to void* by the
        // compiler grugdingly.
        auto result = f2(NULL);
    }
    {
        MuxGuard g(f3m);
        // nullptr is std::nullptr_t, which can be implicitly converted
        // to raw pointers.
        auto result = f3(nullptr);
    }

} // test_nullptr_with_no_template

void test_nullptr_with_template() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);
    std::mutex f1m, f2m, f3m;
    // Compile error! Because 0 is int, and compiler deduces the PtrType to int,
    // while std::shared_ptr can't be initialized with an int.
    /* auto result1 = lockAndCall(f1, f1m, 0); */
    auto result1 = lockAndCall(f1, f1m, nullptr); // Instead, use nullptr

    // Compile error! Because NULL is int, and compiler deduces the PtrType to int/long/...,
    // while std::shared_ptr can't be initialized with an int/long/...
    /* auto result2 = lockAndCall(f2, f2m, NULL); */
    auto result2 = lockAndCall(f2, f2m, nullptr); // Instead, use nullptr

    auto result3 = lockAndCall(f3, f3m, nullptr);

} // test_nullptr_with_template

void test_nullptr_play_with_template() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);
    test_nullptr_with_no_template();
    test_nullptr_with_template();
}

void test_all() {

    utilities::ShowStartEndMsg smsg(__FUNCTION__);
    test_zero_and_null_overloaded_functions();
    test_nullptr_play_with_template();

} // test_all

} // namespace item_08

} // namespace chapter_3

} // namespace effective_mordern_cpp

