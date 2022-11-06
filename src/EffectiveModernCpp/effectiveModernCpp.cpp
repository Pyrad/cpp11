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
#include "chapter3_item13.hpp"
#include "chapter3_item14.hpp"
#include "chapter3_item15.hpp"
#include "chapter3_item16.hpp"
#include "chapter3_item17.hpp"
#include "chapter4_item18.hpp"
#include "chapter4_item19.hpp"
#include "chapter4_item20.hpp"
#include "chapter4_item21.hpp"
#include "chapter4_item22.hpp"
#include "chapter5_item23.hpp"
#include "chapter5_item24.hpp"
#include "chapter5_item25.hpp"
#include "chapter5_item26.hpp"
#include "chapter5_item27.hpp"
#include "chapter5_item28.hpp"
#include "chapter5_item29.hpp"
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
    chapter_3::item_13::test_all();
    chapter_3::item_14::test_all();
    chapter_3::item_15::test_all();
    chapter_3::item_16::test_all();
    chapter_3::item_17::test_all();
    chapter_4::item_18::test_all();
    chapter_4::item_19::test_all();
    chapter_4::item_20::test_all();
    chapter_4::item_21::test_all();
    chapter_4::item_22::test_all();
    chapter_5::item_23::test_all();
    chapter_5::item_24::test_all();
    chapter_5::item_25::test_all();
    chapter_5::item_26::test_all();
    chapter_5::item_27::test_all();
    chapter_5::item_28::test_all();
    chapter_5::item_29::test_all();

    fprintf(stdout, "----- END of function effective_mordern_cpp::%s -----\n\n", __FUNCTION__);

} // function test_all

} // namespace effective_mordern_cpp
