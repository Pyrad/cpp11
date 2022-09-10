#include "chapter3_item07.hpp"
#include <vector>
#include <boost/type_index.hpp>

namespace effective_mordern_cpp {

namespace chapter_3 {

namespace item_07 {

int foo::idcnt = 0;

void test_initialize_ways_compared_with_curly_braces() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    int a(0); // initialize with parenthesis
    int b = 0; // initialize with a equal sign
    int c{0}; // initialize with a pair of curly braces
    int d = {0}; // equals-sign-plus-curly-braces, C++ treat it the same as braces-only version

    foo myfoo(10);
    foo myfoo2 = myfoo; // initialization with the copy cotr


    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);


} // test_initialize_ways_compared_with_curly_braces

void test_class_member_default_values() {
    utilities::ShowStartEndMsg msgshow(__FUNCTION__);

    // Default values (4 and 5) are overridden by the values in default ctor.
    ObjDefaultMemberValue myDefaultMemberValue;
    myDefaultMemberValue.show_me(); // 0 and 1

    // Default values (4 and 5) are used because m_x and m_y are not set in
    // the copy constructor.
    ObjDefaultMemberValue myDefaultMemberValue2 = myDefaultMemberValue;
    myDefaultMemberValue2.show_me(); // 4 and 5


} // test_class_member_default_values

void test_ctors() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    foo myfoo(10);
    foo myfoo2 = myfoo;

    myfoo.show_me();
    myfoo2.show_me();
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
}

void test_equal_sign_cannot_be_used_for_constructing_atomic() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    std::atomic<int> ai1{0};
    std::atomic<int> ai2(0);
    // For std::atomic, Can't use copy constructor, because it is marked as "delete"
    /* std::atomic<int> ai3 = 0; */

    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);

} // test_equal_sign_cannot_be_used_for_constructing_atomic

void test_narrowing_conversion() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    double x = 0, y = 1, z = 2;

    // Can't use curly braces, because narrowing convertion is not allowed
    // by initialization in curly braces.
    // Here narrowing convertion happens when trying to convert from
    // 'double' to 'int'.
    /* int sum1{x + y + z}; */

    int sum2( x + y + z ); // Ok
    int sum3 = x + y + z ; // Ok
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
} // test_narrawing_conversion

void test_curly_braces_call_default_ctor() {
    utilities::ShowStartEndMsg MSGSHOW(__FUNCTION__);

    myItemCallDefaultCtor a; // calls default constructor
    myItemCallDefaultCtor b{}; // calls default constructor
    myItemCallDefaultCtor c({}); // calls constructor with std::initializer_list
    myItemCallDefaultCtor d{{}}; // calls constructor with std::initializer_list
    
} // test_curly_braces_call_default_ctor

/**
 * @brief Show how curly braces with a std::initializer_list as ctor's parameter
 *        results in bad match when contructing objects.
 *
 * If there is a ctor which accepts a std::initializer_list, then perhaps it
 * results in bad match when contructing objects. Even sometimes the narrowing
 * convertion happens, compiler still tries to match it with the ctor with a
 * parameter of std::initializer_list, thus leads to compiler error!
 */
void test_curly_braces_hijack_with_std_initializer_list() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    myItem2Ctors a(0, false); // ctor with int & bool
    myItem2Ctors b(0, 0.1); // ctor with int & double
    myItem2Ctors c{0, false}; // ctor with int & bool
    myItem2Ctors d{0, 0.1}; // ctor with int & double

    myItem3Ctors e(0, false); // ctor with int & bool
    myItem3Ctors f(0, 0.1); // ctor with int & double
    myItem3Ctors g{0, false}; // Hijack!! ctor with std::initializer_list was called
    myItem3Ctors h{0, 0.1}; // Hijack!! ctor with std::initializer_list was called

    myItemFloatOp x0(0, false); // Define 2 objects
    myItemFloatOp x1(0, false); // Define 2 objects

    myItemFloatOp a1(x0); // copy constructor called
    myItemFloatOp a2{x0}; // Hijack!!! copy constructor not called

    myItemFloatOp a3(std::move(x0)); // copy constructor called
    myItemFloatOp a4{std::move(x1)}; // Hijack!!! copy constructor not called

    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
} // test_curly_braces_hijack_with_std_initializer_list


void test_curly_braces_hijack_with_std_initializer_list_lead_to_compile_errors() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);

    myItemNarrowConversion w0(0, 5.0); // Ok

    // Compile error, because curly braces with std initializer_list ctor hijack other
    // ctors, but at the same time, narrowing convertion is not allowed, thus error.
    /* myItemNarrowConversion w1{0, 5.0}; */

    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);

} // test_curly_braces_hijack_with_std_initializer_list_lead_to_compile_errors


/**
 * @brief Show usage of curly braces
 */
void test_initialize_with_curly_braces() {
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);

    test_initialize_ways_compared_with_curly_braces();

    test_class_member_default_values();

    test_equal_sign_cannot_be_used_for_constructing_atomic();

    test_curly_braces_hijack_with_std_initializer_list();

    test_curly_braces_hijack_with_std_initializer_list_lead_to_compile_errors();

    test_curly_braces_call_default_ctor();

    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
} // test_initialize_with_curly_braces



void test_all() {
    fprintf(stdout, "\n");
    fprintf(stdout, "----- BEGIN of function %s -----\n", __FUNCTION__);
    test_ctors();
    test_initialize_with_curly_braces();
    fprintf(stdout, "----- END of function %s -----\n\n", __FUNCTION__);
}

} // namespace item_07

} // namespace chapter_3

} // namespace effective_mordern_cpp

