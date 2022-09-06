#include "chapter2_item05.hpp"
#include <functional>
#include <memory>
#include <stdio.h>
#include <unordered_map>

namespace effective_mordern_cpp {

namespace chapter_2 {

namespace item_05 {

int foo::idcnt = 0;

/**
 * @brief To show the iterator returned by unordered_map is actually the type of
 *        std::pair<const KeyType, ValueType>, not std::pair<KeyType, ValueType>
 * 
 * Because in the iteration below, the auto is used, then the compiler deduces the
 * type as std::pair<const foo, int>, thus a temporary object creation is avoided
 */
void test_unordered_map_with_auto() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    std::unordered_map<foo, int, fooHash> fmap;
    fprintf(stdout, "----- checkPoint0 -----\n");
    foo f1, f2;
    fprintf(stdout, "----- checkPoint1 -----\n");
    fmap.insert(std::make_pair(f1, 0));
    fmap.insert(std::make_pair(f2, 0));
    fprintf(stdout, "----- checkPoint2 -----\n");
    for (const auto &v : fmap) {
        fprintf(stdout, "foo.id() = %d\n", v.first.id());
    }
    fprintf(stdout, "----- checkPoint3 -----\n");
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
}

/**
 * @brief To show the iterator returned by unordered_map is actually the type of
 *        std::pair<const KeyType, ValueType>, not std::pair<KeyType, ValueType>
 * 
 * Because in the iteration below, the std::pair<foo, int> other than
 * std::pair<const foo, int> is used, so the compiler will try to convert 
 * std::pair<const foo, int> to std::pair<foo, int>, thus a new foo object is created
 * indeed, which is a temporary object.
 */
void test_unordered_map_without_auto() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    std::unordered_map<foo, int, fooHash> fmap;
    fprintf(stdout, "----- checkPoint0 -----\n");
    foo f1, f2;
    fprintf(stdout, "----- checkPoint1 -----\n");
    fmap.insert(std::make_pair(f1, 0));
    fmap.insert(std::make_pair(f2, 0));
    fprintf(stdout, "----- checkPoint2 -----\n");
    for (const std::pair<foo, int> &v : fmap) {
        fprintf(stdout, "foo.id() = %d\n", v.first.id());
    }
    fprintf(stdout, "----- checkPoint3 -----\n");
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
}

/**
 * @brief Compare using 'auto' or not using 'auto' when iterating an
 *        unordered_map
 */
void test_auto_in_map_iteration() {
    test_unordered_map_without_auto();
    test_unordered_map_with_auto();
}

bool compare_foo_less(const std::unique_ptr<foo> &lhs,
                      const std::unique_ptr<foo> &rhs)
{ return *lhs < *rhs; }


void test_std_func_and_auto() {
    std::unique_ptr<foo> aptr(new foo());
    std::unique_ptr<foo> bptr(new foo());

    // Use std::function
    std::function<bool(const std::unique_ptr<foo> &, const std::unique_ptr<foo> &)> func = compare_foo_less;
    fprintf(stdout, "aptr < bptr = %s (using std::function)\n", func(aptr, bptr) ? "YES" : "NO");

    // Use lambda with specific return type name
    std::function<bool(const std::unique_ptr<foo> &, const std::unique_ptr<foo> &)> 
    deref_up_less = [](const std::unique_ptr<foo> &a, const std::unique_ptr<foo> &b)
    { return *a < *b; };
    fprintf(stdout, "aptr < bptr = %s (using deref_up_less)\n", deref_up_less(aptr, bptr) ? "YES" : "NO" );

    // Use lambda with auto (compiler deduces types)
    auto
    deref_up_less_auto = [](const std::unique_ptr<foo> &a, const std::unique_ptr<foo> &b)
    { return *a < *b; };
    fprintf(stdout, "aptr < bptr = %s (using deref_up_less_auto)\n", deref_up_less_auto(aptr, bptr) ? "YES" : "NO" );
} // test_std_func_and_auto

void test_auto_all() {
    test_auto_in_map_iteration();
    test_std_func_and_auto();
}

} // namespace item_05

} // namespace chapter_2

} // namespace effective_mordern_cpp

