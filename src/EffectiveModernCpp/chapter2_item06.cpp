#include "chapter1_item06.hpp"
#include <vector>
#include <boost/type_index.hpp>

namespace effective_mordern_cpp {

namespace chapter_2 {

namespace item_06 {

std::vector<bool> get_bool_vec() {
    std::vector<bool> ivec{true, false, true, true};
    return ivec;
}

/**
 * Pay attention to the difference between this function and function
 * test_access_bool_vec_with_static_cast
 */
void test_access_bool_vec_with_auto() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    auto val = get_bool_vec()[0];
    namespace bti = boost::typeindex;
    std::cout << "type of val: " << bti::type_id_with_cvr<decltype(val)>().pretty_name() << "\n";
    std::cout << "val = " << val << "\n";
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
} // test_access_bool_vec_with_auto

/**
 * Pay attention to the difference between this function and function
 * test_access_bool_vec_with_auto
 */
void test_access_bool_vec_with_static_cast() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    auto val = static_cast<bool>(get_bool_vec()[0]);
    namespace bti = boost::typeindex;
    std::cout << "type of val: " << bti::type_id_with_cvr<decltype(val)>().pretty_name() << "\n";
    std::cout << "val = " << val << "\n";
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
} // test_access_bool_vec_with_static_cast

void test_auto_all() {
    fprintf(stdout, "\n");
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    test_access_bool_vec_with_auto();
    test_access_bool_vec_with_static_cast();
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
}


} // namespace item_06

} // namespace chapter_2

} // namespace effective_mordern_cpp

