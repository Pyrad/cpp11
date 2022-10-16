#include <iostream>

#include "effectiveModernCpp.hpp"
#include "chapter1_item01.hpp"
#include "chapter1_item02.hpp"
#include "chapter1_item03.hpp"
#include "chapter1_item04.hpp"
#include "chapter2_item05.hpp"
#include "chapter2_item06.hpp"
#include "chapter3_item07.hpp"
#include "chapter3_item08.hpp"
#include "chapter3_item09.hpp"
#include "chapter3_item10.hpp"
#include "chapter3_item11.hpp"
#include "chapter3_item12.hpp"
#include "../utilities/utilities.hpp"

namespace effective_mordern_cpp {

void test_all() {
    fprintf(stdout, "\n----- BEGIN of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

    chapter_1::item_01::test_template_type_deduction();
    chapter_1::item_02::test_auto_deduction();
    chapter_1::item_03::test_decltype();
    chapter_1::item_04::test_show_runtime_types();
    chapter_2::item_05::test_auto_all();
    chapter_2::item_06::test_auto_all();
    chapter_3::item_07::test_all();
    chapter_3::item_08::test_all();
    chapter_3::item_09::test_all();
    chapter_3::item_10::test_all();
    chapter_3::item_11::test_all();
    chapter_3::item_12::test_all();

    fprintf(stdout, "----- END of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

} // function test_all

} // namespace effective_mordern_cpp
