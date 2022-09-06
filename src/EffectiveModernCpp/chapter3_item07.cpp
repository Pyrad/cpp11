#include "chapter3_item07.hpp"
#include <vector>
#include <boost/type_index.hpp>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_07 {

int foo::idcnt = 0;

void test_ctors() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    foo myfoo(10);
    foo myfoo2 = myfoo;

    myfoo.show_me();
    myfoo2.show_me();
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
}



void test_all() {
    fprintf(stdout, "\n");
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    test_ctors();
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
}

} // namespace item_07

} // namespace chapter_3

} // namespace effective_mordern_cpp

