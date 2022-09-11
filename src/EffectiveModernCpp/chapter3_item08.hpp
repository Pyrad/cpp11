#ifndef EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM08
#define EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM08

#include <iostream>
#include <memory>
#include <stdio.h>
#include <mutex>
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_08 {

class foo {
public:
    foo() { }
    virtual ~foo () { }

private:
    int m_value = 0;
}; // class foo

void myfoo(int i);
void myfoo(bool b);
void myfoo(void*);

int f1(std::shared_ptr<foo> spw);
double f2(std::unique_ptr<foo> upw);
bool f3(foo* pw);

/**
 * Pay attention to this template function, that it can return to
 * different types, according to the function pointer passed in.
 */
template<typename FuncType, typename MuxType, typename PtrType>
auto lockAndCall(FuncType func, MuxType &mtx, PtrType ptr) -> decltype(func(ptr)) {
    using MuxGuard = std::lock_guard<std::mutex>;
    MuxGuard g(mtx);
    return func(ptr);
} // lockAndCall



void test_zero_and_null_overloaded_functions();

void test_nullptr_with_no_template();
void test_nullptr_with_template();
void test_nullptr_play_with_template();



void test_all();


} // namespace item_08

} // namespace chapter_3

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER3_ITEM08

