#include "chapter1_item05.hpp"
#include <unordered_map>

namespace effective_mordern_cpp {

namespace chapter_1 {

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

void test_auto_all() {
    test_unordered_map_without_auto();
    test_unordered_map_with_auto();
}

} // namespace item_05

} // namespace chapter_1

} // namespace effective_mordern_cpp

