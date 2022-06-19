#include "normal.hpp"

#include<stdlib.h>
#include<string.h>
#include<list>

#include<climits>

#include <type_traits>

#include <boost/optional.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <regex>
#include <bitset>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/math/special_functions/relative_difference.hpp>
#include <boost/math/special_functions/next.hpp>

#include <limits> // for std::numeric_limits<T>::epsilon().

namespace NORMAL {

bool validate_order(const std::vector<int> &arr) {
   if (arr.size() <= 1) {
      return true;
   }

   auto pre_m = arr[0];
   std::set<int32_t> mset;

   for (const auto &v : arr) {
      if (pre_m != v) {
         if (mset.find(pre_m) == mset.end()) {
            mset.insert(pre_m);
         } else {
            return false;
         }
         pre_m = v;
      }
   }

   return true;
}

static bool edge_sequence_comp_less(const path_query_result_type &t1,
                                const path_query_result_type &t2)
{
   #define POSITION 3
   return std::get<POSITION>(t1) < std::get<POSITION>(t2);
   #undef POSITION
}

static bool edge_sequence_comp_greater(const path_query_result_type &t1,
                                const path_query_result_type &t2)
{
   #define POSITION 3
   return std::get<POSITION>(t1) > std::get<POSITION>(t2);
   #undef POSITION
}

static bool edge_close_enough(int a, int b, double gap) {
    // return std::abs(std::get<POSITION>(t1) - std::get<POSITION>(t2)) < gap;
   return std::abs(a - b) < gap;
}


#if 0

41 28 10 25 48 13  8 27 45 47 11  9 26 42 44 29 46 40 43 12
 5  6  7  8  9 10 11 12 13 30 31 32 33 34 35 36 50 51 52 53


8 9 10 11 12 13 *25 26 27  28 29 *40 41 42 43 44  45 46 47 48
5 6  7  8  9 10  11 12 13 *30 31  32 33 34 35 36 *50 51 52 53

#endif

static void print_tuple_vector(const std::vector<path_query_result_type> &arr) {
    for (const auto &t : arr) {
        int a = std::get<0>(t);
        int b = std::get<1>(t);
        int c = std::get<2>(t);
        int d = std::get<3>(t);
        printf("%d %d %d %d\n", a, b, c, d);
    }
}


template<int IINDEX>
bool is_ordered_sequence(const std::vector<path_query_result_type> &arr,
                   const std::pair<size_type, size_type> &range)
{
   if (arr.size() <= 1) {
      return true;
   }

   size_type i = range.first;
   if (std::get<IINDEX>(arr[i]) < std::get<IINDEX>(arr[range.second])) {
      for (; i < range.second - 1; ++i) {
         if (std::get<IINDEX>(arr[i]) > std::get<IINDEX>(arr[i+1])) {
            return false;
         }
      }
      return true;
   } else if (std::get<IINDEX>(arr[i]) > std::get<IINDEX>(arr[range.second])) {
      for (; i < range.second - 1; ++i) {
         if (std::get<IINDEX>(arr[i]) < std::get<IINDEX>(arr[i+1])) {
            return false;
         }
      }
      return true;
   } else { // (std::get<IINDEX>(arr[i]) == std::get<IINDEX>(arr[range.second])) {
      for (; i < range.second; ++i) {
         if (std::get<IINDEX>(arr[i]) != std::get<IINDEX>(arr[range.second])) {
            return false;
         }
      }
      return true;
   }
}

class active_part {
public:
   std::list<int> m_path_a;
   std::list<int> m_path_b;
};


void separate_segments(const std::vector<path_query_result_type> &arr,
                        std::pair<size_type, size_type> range,
                        double gap)
{
    #define POSITION_A 1
    #define POSITION_B 3

    size_type i = range.first;
    size_type pre = std::get<POSITION_A>(arr[0]);
    size_type pre_idx = i;
    
    assert(range.first <= range.second);
    std::list<std::pair<size_type, size_type>> alist;
    std::map<size_type, size_type> amap;
    for (; i < range.second; ++i) {
        assert(i + 1 <= range.second);
        size_type next = std::get<POSITION_A>(arr[i + 1]);
        // if (!edge_close_enough(pa, pre, next, gap)) {
        if (!edge_close_enough(pre, next, gap)) {

            // Get this longest consecutive one
            assert(pre_idx <= i);
            alist.emplace_back(std::make_pair(pre_idx, i));

            size_type end1 = std::get<POSITION_A>(arr[i]);
            size_type end2 = std::get<POSITION_B>(arr[i]);
            amap.emplace(std::make_pair(end2, end1));

            pre_idx = i + 1;
        }
        // If these 2 edge is close enough
        pre = next;
    }
    assert(pre_idx <= i);
    alist.emplace_back(std::make_pair(pre_idx, i));

    printf("Segments in Path A\n");
    for (const auto &ele : alist) {
        printf("%lu -> %lu\n", ele.first, ele.second);
    }

    size_type end1 = std::get<POSITION_A>(arr[i]);
    size_type end2 = std::get<POSITION_B>(arr[i]);
    amap.emplace(std::make_pair(end2, end1));
    printf("End-End pairs\n");
    for (const auto &v : amap) {
       printf("%lu - %lu\n", v.first, v.second);
    }

    /*
    std::vector<path_query_result_type> rvec(arr.begin() + range.first, arr.begin() + range.second + 1);
    std::sort(rvec.begin(), rvec.end(), edge_sequence_comp);
    printf("Tuple vec after sort\n");
    print_tuple_vector(rvec);
    */

#if 0
    std::list<active_part> APlist;
    for (const auto &v : alist) {
       // TODO should be assert(is_ordered_sequence<POSITION_B>(arr, v));
       if (is_ordered_sequence<POSITION_B>(arr, v)) {
          printf("Path B is ordered in range: %lu -> %lu\n", v.first, v.second);
       } else {
          printf("Path B is NOT ordered in range: %lu -> %lu\n", v.first, v.second);
       }

       // Sort the elements in PathB corresponding to this range 
       std::vector<path_query_result_type> rvec(arr.begin() + v.first, arr.begin() + v.second + 1);
       if (std::get<POSITION_B>(arr[v.first]) < std::get<POSITION_B>(arr[v.secod])) {
          std::sort(rvec.begin(), rvec.end(), edge_sequence_comp_less);
       } else {
          std::sort(rvec.begin(), rvec.end(), edge_sequence_comp_greater);
       }

       active_part single_ap;
       assert(v.first <= v.second);
       assert(v.second - v.first + 1 == rvec.size());
       for (size_type idx = 0; idx < rvec.size(); ++idx) {
          single_ap.m_path_a.push_back(std::get<POSITION_A>(arr[idx + v.first]));
          single_ap.m_path_b.push_back(rvec[idx]);
       }
       APlist.push_back(single_ap);
    }
#endif // 0

    #undef POSITION_A
    #undef POSITION_B

}



void print_matrix( const char* desc, int m, int n, double* a, int lda ) {
   int i, j;
   printf( "\n %s\n", desc );
   for( i = 0; i < m; i++ ) {
      for( j = 0; j < n; j++ ) printf( " %6.2f", a[i+j*lda] );
      printf( "\n" );
   }
}

int sparse2dense(double* sparse, int row_sparse, int col_sparse, double* dense, int row_dense, int col_dense) {
   int rCode = 0;
   int sparse_start, dense_start, last, inx;
   for (int i = 0; i < row_dense; i++) {
      sparse_start = i*col_sparse;
      dense_start = i* col_dense;
      last = sparse[sparse_start+col_sparse-1];
      for (int j = 1; j < col_sparse-2; j+=2) {
         inx = sparse_start + j;
         if (sparse[inx] == -1) break;
         if (sparse[inx] >= col_dense) {
            rCode = -1;
            goto quit;
         }
         dense[ dense_start + (int)sparse[inx]] = sparse[inx+1];
      }
      while(last>0) {

         for (int j = 1; j < col_sparse-2; j+=2) {
            inx = last*col_sparse + j;
            if (sparse[inx] == -1) break;
            if (sparse[inx] >= col_dense) {
               rCode = -1;
               goto quit;
            }
            dense[ dense_start + (int)sparse[inx] ] = sparse[inx+1];
         }
         last = sparse[last*col_sparse+col_sparse-1];
      }
   }
quit:
   print_matrix("Matrix dense (internal):", row_dense, col_dense, dense, row_dense);
   return rCode;
}

bool test_temporary_param(std::string &str) {
    str.clear();
    str = "test";

    return true;
}


bool custom_compare(int x, bool (*func)(int i)) {
    if (!func) {
        return x > 9;
    } else {
        return func(x);
    }
}

template <typename T>
void temptest(T arr) {
    using vtype = typename std::decay<decltype(arr[0])>::type;
    if (typeid(vtype) == typeid(int)) {
        printf("[Type 1] Core type is int\n");
    } else if (typeid(vtype) == typeid(float)) {
        printf("[Type 2] Core type is float\n");
    } else if (typeid(vtype) == typeid(double)) {
        printf("[Type 3] Core type is double\n");
    } else {
        printf("[Type UNKOWN] Core type is UNKOWN\n");
    }
}

bool boost_optional_reference_test(boost::optional<const valClass&> vrf) {
    if (vrf.is_initialized()) {
        printf("valueClass reference is initialized\n");
        vrf.get().print();
        return true;
    } else {
        printf("valueClass reference is NOT initialized\n");
        return false;
    }
}

// I should get the function resolved by namespace issue, thus it can work by
// then. 2022-06-18
//
#if 0
void test_float_compare() {
  std::cout << "Compare floats using Boost.Math functions/classes" << std::endl;


//[compare_floats_using
/*`Some using statements will ensure that the functions we need are accessible.
*/

  using namespace boost::math;

//`or

  using boost::math::relative_difference;
  using boost::math::epsilon_difference;
  using boost::math::float_next;
  using boost::math::float_prior;

//] [/compare_floats_using]


//[compare_floats_example_1
/*`The following examples display values with all possibly significant digits.
Newer compilers should provide `std::numeric_limitsFPT>::max_digits10`
for this purpose, and here we use `float` precision where `max_digits10` = 9
to avoid displaying a distracting number of decimal digits.

[note Older compilers can use this formula to calculate `max_digits10`
from `std::numeric_limits<FPT>::digits10`:[br]
__spaces `int max_digits10 = 2 + std::numeric_limits<FPT>::digits10 * 3010/10000;`
] [/note]

One can set the display including all trailing zeros
(helpful for this example to show all potentially significant digits),
and also to display `bool` values as words rather than integers:
*/
  std::cout.precision(std::numeric_limits<float>::max_digits10);
  std::cout << std::boolalpha << std::showpoint << std::endl;

//] [/compare_floats_example_1]

//[compare_floats_example_2]
/*`
When comparing values that are ['quite close] or ['approximately equal],
we could use either `float_distance` or `relative_difference`/`epsilon_difference`, for example
with type `float`, these two values are adjacent to each other:
*/

  float a = 1;
  float b = 1 + std::numeric_limits<float>::epsilon();
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
  std::cout << "float_distance = " << float_distance(a, b) << std::endl;
  std::cout << "relative_difference = " << relative_difference(a, b) << std::endl;
  std::cout << "epsilon_difference = " << epsilon_difference(a, b) << std::endl;

/*`
Which produces the output:

[pre
a = 1.00000000
b = 1.00000012
float_distance = 1.00000000
relative_difference = 1.19209290e-007
epsilon_difference = 1.00000000
]
*/
  //] [/compare_floats_example_2]

//[compare_floats_example_3]
/*`
In the example above, it just so happens that the edit distance as measured by `float_distance`, and the
difference measured in units of epsilon were equal.  However, due to the way floating point
values are represented, that is not always the case:*/

  a = 2.0f / 3.0f;   // 2/3 inexactly represented as a float
  b = float_next(float_next(float_next(a))); // 3 floating point values above a
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
  std::cout << "float_distance = " << float_distance(a, b) << std::endl;
  std::cout << "relative_difference = " << relative_difference(a, b) << std::endl;
  std::cout << "epsilon_difference = " << epsilon_difference(a, b) << std::endl;

/*`
Which produces the output:

[pre
a = 0.666666687
b = 0.666666865
float_distance = 3.00000000
relative_difference = 2.68220901e-007
epsilon_difference = 2.25000000
]

There is another important difference between `float_distance` and the
`relative_difference/epsilon_difference` functions in that `float_distance`
returns a signed result that reflects which argument is larger in magnitude,
where as `relative_difference/epsilon_difference` simply return an unsigned
value that represents how far apart the values are.  For example if we swap
the order of the arguments:
*/

  std::cout << "float_distance = " << float_distance(b, a) << std::endl;
  std::cout << "relative_difference = " << relative_difference(b, a) << std::endl;
  std::cout << "epsilon_difference = " << epsilon_difference(b, a) << std::endl;

  /*`
  The output is now:

  [pre
  float_distance = -3.00000000
  relative_difference = 2.68220901e-007
  epsilon_difference = 2.25000000
  ]
*/
  //] [/compare_floats_example_3]

//[compare_floats_example_4]
/*`
Zeros are always treated as equal, as are infinities as long as they have the same sign:*/

  a = 0;
  b = -0;  // signed zero
  std::cout << "relative_difference = " << relative_difference(a, b) << std::endl;
  a = b = std::numeric_limits<float>::infinity();
  std::cout << "relative_difference = " << relative_difference(a, b) << std::endl;
  std::cout << "relative_difference = " << relative_difference(a, -b) << std::endl;

/*`
Which produces the output:

[pre
relative_difference = 0.000000000
relative_difference = 0.000000000
relative_difference = 3.40282347e+038
]
*/
//] [/compare_floats_example_4]

//[compare_floats_example_5]
/*`
Note that finite values are always infinitely far away from infinities even if those finite values are very large:*/

  a = (std::numeric_limits<float>::max)();
  b = std::numeric_limits<float>::infinity();
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
  std::cout << "relative_difference = " << relative_difference(a, b) << std::endl;
  std::cout << "epsilon_difference = " << epsilon_difference(a, b) << std::endl;

/*`
Which produces the output:

[pre
a = 3.40282347e+038
b = 1.#INF0000
relative_difference = 3.40282347e+038
epsilon_difference = 3.40282347e+038
]
*/
//] [/compare_floats_example_5]

//[compare_floats_example_6]
/*`
Finally, all denormalized values and zeros are treated as being effectively equal:*/

  a = std::numeric_limits<float>::denorm_min();
  b = a * 2;
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
  std::cout << "float_distance = " << float_distance(a, b) << std::endl;
  std::cout << "relative_difference = " << relative_difference(a, b) << std::endl;
  std::cout << "epsilon_difference = " << epsilon_difference(a, b) << std::endl;
  a = 0;
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
  std::cout << "float_distance = " << float_distance(a, b) << std::endl;
  std::cout << "relative_difference = " << relative_difference(a, b) << std::endl;
  std::cout << "epsilon_difference = " << epsilon_difference(a, b) << std::endl;

/*`
Which produces the output:

[pre
a = 1.40129846e-045
b = 2.80259693e-045
float_distance = 1.00000000
relative_difference = 0.000000000
epsilon_difference = 0.000000000
a = 0.000000000
b = 2.80259693e-045
float_distance = 2.00000000
relative_difference = 0.000000000
epsilon_difference = 0.000000000]

Notice how, in the above example, two denormalized values that are a factor of 2 apart are
none the less only one representation apart!

*/
//] [/compare_floats_example_6]


#if 0
//[old_compare_floats_example_3
//`The simplest use is to compare two values with a tolerance thus:

  bool is_close = is_close_to(1.F, 1.F + epsilon, epsilon); // One epsilon apart is close enough.
  std::cout << "is_close_to(1.F, 1.F + epsilon, epsilon); is " << is_close << std::endl; // true

  is_close = is_close_to(1.F, 1.F + 2 * epsilon, epsilon); // Two epsilon apart isn't close enough.
  std::cout << "is_close_to(1.F, 1.F + epsilon, epsilon); is " << is_close << std::endl; // false

/*`
[note The type FPT of the tolerance and the type of the values [*must match].

So `is_close(0.1F, 1., 1.)` will fail to compile because "template parameter 'FPT' is ambiguous".
Always provide the same type, using `static_cast<FPT>` if necessary.]
*/


/*`An instance of class `close_at_tolerance` is more convenient
when multiple tests with the same conditions are planned.
A class that stores a tolerance of three epsilon (and the default ['strong] test) is:
*/

  close_at_tolerance<float> three_rounds(3 * epsilon); // 'strong' by default.

//`and we can confirm these settings:

  std::cout << "fraction_tolerance = "
    << three_rounds.fraction_tolerance()
    << std::endl; // +3.57627869e-007
  std::cout << "strength = "
    << (three_rounds.strength() == FPC_STRONG ? "strong" : "weak")
    << std::endl; // strong

//`To start, let us use two values that are truly equal (having identical bit patterns)

  float a = 1.23456789F;
  float b = 1.23456789F;

//`and make a comparison using our 3*epsilon `three_rounds` functor:

  bool close = three_rounds(a, b);
  std::cout << "three_rounds(a, b) = " << close << std::endl; // true

//`Unsurprisingly, the result is true, and the failed fraction is zero.

  std::cout << "failed_fraction = " << three_rounds.failed_fraction() << std::endl;

/*`To get some nearby values, it is convenient to use the Boost.Math __next_float functions,
for which we need an include

  #include <boost/math/special_functions/next.hpp>

and some using declarations:
*/

  using boost::math::float_next;
  using boost::math::float_prior;
  using boost::math::nextafter;
  using boost::math::float_distance;

//`To add a few __ulp to one value:
  b = float_next(a); // Add just one ULP to a.
  b = float_next(b); // Add another one ULP.
  b = float_next(b); // Add another one ULP.
  // 3 epsilon would pass.
  b = float_next(b); // Add another one ULP.

//`and repeat our comparison:

  close = three_rounds(a, b);
  std::cout << "three_rounds(a, b) = " << close << std::endl; // false
  std::cout << "failed_fraction = " << three_rounds.failed_fraction()
    << std::endl;  // abs(u-v) / abs(v) = 3.86237957e-007

//`We can also 'measure' the number of bits different using the `float_distance` function:

  std::cout << "float_distance = " << float_distance(a, b) << std::endl; // 4

/*`Now consider two values that are much further apart
than one might expect from ['computational noise],
perhaps the result of two measurements of some physical property like length
where an uncertainty of a percent or so might be expected.
*/
  float fp1 = 0.01000F;
  float fp2 = 0.01001F; // Slightly different.

  float tolerance = 0.0001F;

  close_at_tolerance<float> strong(epsilon); // Default is strong.
  bool rs = strong(fp1, fp2);
  std::cout << "strong(fp1, fp2) is " << rs << std::endl;

//`Or we could contrast using the ['weak] criterion:
  close_at_tolerance<float> weak(epsilon, FPC_WEAK); // Explicitly weak.
  bool rw = weak(fp1, fp2); //
  std::cout << "weak(fp1, fp2) is " << rw << std::endl;

//`We can also construct, setting tolerance and strength, and compare in one statement:

  std::cout << a << " #= " << b << " is "
    << close_at_tolerance<float>(epsilon, FPC_STRONG)(a, b) << std::endl;
  std::cout << a << " ~= " << b << " is "
    << close_at_tolerance<float>(epsilon, FPC_WEAK)(a, b) << std::endl;

//`but this has little advantage over using function `is_close_to` directly.

//] [/old_compare_floats_example_3]


/*When the floating-point values become very small and near zero, using
//a relative test becomes unhelpful because one is dividing by zero or tiny,

//Instead, an absolute test is needed, comparing one (or usually both) values with zero,
//using a tolerance.
//This is provided by the `small_with_tolerance` class and `is_small` function.

  namespace boost {
  namespace math {
  namespace fpc {


  template<typename FPT>
  class small_with_tolerance
  {
  public:
  // Public typedefs.
  typedef bool result_type;

  // Constructor.
  explicit small_with_tolerance(FPT tolerance); // tolerance >= 0

  // Functor
  bool operator()(FPT value) const; // return true if <= absolute tolerance (near zero).
  };

  template<typename FPT>
  bool
  is_small(FPT value, FPT tolerance); // return true if value <= absolute tolerance (near zero).

  }}} // namespaces.

/*`
[note The type FPT of the tolerance and the type of the value [*must match].

So `is_small(0.1F, 0.000001)` will fail to compile because "template parameter 'FPT' is ambiguous".
Always provide the same type, using `static_cast<FPT>` if necessary.]

A few values near zero are tested with varying tolerance below.
*/
//[compare_floats_small_1

  float c = 0;
  std::cout << "0 is_small " << is_small(c, epsilon) << std::endl; // true

  c = std::numeric_limits<float>::denorm_min(); // 1.40129846e-045
  std::cout << "denorm_ min =" << c << ", is_small is " << is_small(c, epsilon) << std::endl; // true

  c = (std::numeric_limits<float>::min)(); // 1.17549435e-038
  std::cout << "min = " << c << ", is_small is " << is_small(c, epsilon) << std::endl; // true

  c = 1 * epsilon; // 1.19209290e-007
  std::cout << "epsilon = " << c << ", is_small is " << is_small(c, epsilon) << std::endl; // false

  c = 1 * epsilon; // 1.19209290e-007
  std::cout << "2 epsilon = " << c << ", is_small is " << is_small(c, 2 * epsilon) << std::endl; // true

  c = 2 * epsilon; //2.38418579e-007
  std::cout << "4 epsilon = " << c << ", is_small is " << is_small(c, 2 * epsilon) << std::endl; // false

  c = 0.00001F;
  std::cout << "0.00001 = " << c << ", is_small is " << is_small(c, 0.0001F) << std::endl; // true

  c = -0.00001F;
  std::cout << "0.00001 = " << c << ", is_small is " << is_small(c, 0.0001F) << std::endl; // true

/*`Using the class `small_with_tolerance` allows storage of the tolerance,
convenient if you make repeated tests with the same tolerance.
*/

  small_with_tolerance<float>my_test(0.01F);

  std::cout << "my_test(0.001F) is " << my_test(0.001F) << std::endl; // true
  std::cout << "my_test(0.001F) is " << my_test(0.01F) << std::endl; // false

  //] [/compare_floats_small_1]
#endif
  return 0;
}  // int main()
#endif // 0



void normal_test_all() {
    std::cout << "---------- START OF FUNCTION: " << __FUNCTION__ << std::endl;


    // for (ENUM_KEYS e = ENUM_KEYS::DEV; e < ENUM_KEYS::DEL; e = (ENUM_KEYS)(e + 1)) {
    for (GKEY e = GKEY::DEV; e < GKEY::DEL; e = (GKEY)(e + 1)) {
        std::cout << e << std::endl;
    }

    valClass obj;
    obj.m_emap.emplace(valClass::ENUM_KEYS::FIL, 3);
    obj.m_emap.emplace(valClass::ENUM_KEYS::URX, 9);
    obj.m_emap[valClass::ENUM_KEYS::FIL] = 18;
    for (const auto &v : obj.m_emap) {
        std::cout << "First: " << v.first << ", Second " << v.second << std::endl;
    }

    std::string ustr("(mm)");
    std::string unit(ustr.substr(1, ustr.size() - 2));
    printf("unit is: %s\n", unit.c_str());


    zoo z1(3, 4);
    z1.print();

    zoo z2(1, 8);
    z2.print();

    z1 = zoo(7, 9);
    z1.print();

    zoo z3(zoo(18, 27));
    z3.print();

    printf("This is %d a %d"
            " sperated line %d for"
            " testing intention\n",
            2, 3, 7);


    // Test if numbers in set are in order
    std::set<int> sset;
    sset.insert(9);
    sset.insert(12);
    sset.insert(10);
    sset.insert(8);
    sset.insert(11);

    std::cout << "Sequence" << "\n";
    for (auto i : sset) {
        std::cout << i << "\n";
    }
    std::cout << "\n";

    int vsize = 10;
#if 1
    std::vector<std::shared_ptr<valClass>> vvec;
    vvec.resize(vsize);
    for (std::size_t i = 0; i < vsize; i++) {
        // vvec[i] = std::make_shared<valClass>();
        vvec[i] = nullptr;
    }

    for (auto &p : vvec) {
        if (p) {
            p->print();
        } else {
            printf("Pointer is null\n");
        }
    }
#endif

    
#if 0
    std::vector<std::shared_ptr<valClass>> vvec2(vsize, nullptr);

    for (std::size_t i = 5; i < vsize; i++) {
        vvec2[i] = std::make_shared<valClass>(24);
    }

    for (auto &p : vvec2) {
        if (p) {
            p->print();
        } else {
            printf("Pointer is null\n");
        }
    }


    std::list<int> ilist;
    ilist.push_back(21);
    ilist.push_back(20);
    ilist.push_back(19);
    ilist.push_back(23);
    ilist.push_back(22);

    // std::vector<int> myvec(ilist.begin(), ilist.end());
    for (auto i : ilist) {
        printf("Value is: %d\n", i);
    }
#endif


    // std::regex float_int_pattern("([-+]?[0-9]+)|([-+]?[0-9]*\\.[0-9]+)");
    std::regex float_int_pattern("[-+]?((\\d+)|([0-9]\\d*\\.\\d*)|(\\d*\\.[0-9]\\d*))");
    // std::regex float_int_pattern("\\d*\\.[0-9]\\d*");
    // std::string str("+2.6");
    std::string str("002");

    try {
        printf("Conversion succeed: %d\n",std::stoi(str));
    } catch(...) {
        printf("Conversion fails\n");
    }

    if (std::regex_match(str, float_int_pattern)) {
        printf("Regex match: YES\n");
    } else {
        printf("Regex match: NO\n");
    }


    auto xxx = [](valClass &v) { return static_cast<double>(v.m_value2 + 1); };
    valClass myObj;
    printf("output is: %f\n", xxx(myObj));



    std::string dimstr1("RETICLE_DIM(mm)");
    std::string dimstr2("RETICLE_DIM");

    std::size_t pos1 = dimstr1.find_first_of("(");
    std::size_t pos2 = dimstr2.find_first_of("(");

    std::string mstr1(dimstr1, 0, pos1);
    std::string mstr2(dimstr2, 0, pos2);

    std::cout << "First string: " << mstr1 << std::endl;
    std::cout << "Second string: " << mstr2 << std::endl;


    std::string s1("MINGAP");
    std::string s2("INDEX");

    if (s1.find(s2) != std::string::npos) {
        printf("MINGAP contains INDEX\n");
    } else {
        printf("MINGAP does NOT contain INDEX\n");
    }

    double rotation = -180;
    printf("fmod(rotation, 90) is %f\n", fmod(rotation, 90));


    // std::regex intIdx("[1-9]([0-9]+)?", std::regex::icase);
    std::regex intIdx("[1-9][0-9]*", std::regex::icase);
    std::string istr[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "123", "5601", "0892", "0", "101"};
    for (std::string s : istr) {
        if (std::regex_match(s, intIdx)) {
            printf("%s matches\n", s.c_str());
        } else {
            printf("%s DOES NOT match\n", s.c_str());
        }
    }

    // TEST: trim test
    {
        std::string mstr("10002");

        auto lbd = [](const char &c) { return c != '0'; };
        std::string::iterator itr = std::find_if (mstr.begin(), mstr.end(), lbd);
        mstr.erase(mstr.begin(), itr);

        std::cout << "After trim is: " << mstr << std::endl;
    }

#if 0
    // TEST: get cpu info
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        printf("CPU info: %lu\n", info.dwNumberOfProcessors);
    }
#endif // 0

    // TEST: data reference test
    {
        // See the definition of class dataRef
        printf("\n");
        dataRef darr;
        printf("Size of darr is %d\n", darr.size());
        printf("First element of darr is %d\n", darr[0]);
    }

    // TEST: add a new key-value to a map
    {
        std::map<std::string, std::vector<int> > m;
        std::string key("apple");
        m[key].push_back(1);
        m[key].push_back(3);
        m[key].push_back(4);
        key = "grape";
        m[key].push_back(2);
        m[key].push_back(5);
        m[key].push_back(6);
        for (const auto& p : m) {
            printf("[LONGC_TEST] key '%s' vector has size '%llu'.\n", p.first.c_str(), p.second.size());
        }
    }

    // Test bitset
    {
        std::bitset<9> bset("000011111");
        for (int i = 0; i < 9; i++) {
            printf("[LONGC_TEST] Test %d: %s\n", i, bset.test(i) ? "True" : "False");
        }

        int varibleWidth = 20;
        varibleWidth += 3;

        const int bitwidth = 23;
        // If use const int bitwidth = varibleWidth, then it will be a compile error!
        // as variableWidth is not a constant
        std::bitset<bitwidth> bx;
        for (int i = 0; i < bitwidth; i++) {
            printf("[LONGC_TEST] Test %d: %s\n", i, bx.test(i) ? "True" : "False");
        }
        printf("bitset with width of %d is: %s\n", bitwidth, bx.to_string<char,std::string::traits_type,std::string::allocator_type>().c_str());
    }

    // Test std::find
    {
        std::list<std::string> flist={"put1", "put2", "put3"};
        if (std::find(flist.begin(), flist.end(), "put") != flist.end()) {
            printf("[LONGC_TEST] Found 'put'\n");
        } else {
            printf("[LONGC_TEST] Didn't find 'put'\n");
        }
        if (std::find(flist.begin(), flist.end(), "put2") != flist.end()) {
            printf("[LONGC_TEST] Found 'put2'\n");
        } else {
            printf("[LONGC_TEST] Didn't find 'put2'\n");
        }
    }

    {
        fooValue v1("Apple", 1.4, 9);
        fooValue v2("Banana", -0.23, 87);
        fooValue v3("Pear", 1354.99, -101);
        foo f("Company");
        f.add(std::move(v1));
        f.add(std::move(v2));
        f.add(std::move(v3));

        f.print();
    }

    {
        newZoo z1(2, 3);
        newZoo z2(7, 7);
        newZoo z3(12, 0);
        newZoo z4(200, 29);
        std::unordered_map<std::string, newZoo> m;
        std::string sk("z1");
        m.insert(std::make_pair(std::move(sk), std::move(z1)));
        sk = "z2";
        m.insert(std::make_pair(std::move(sk), std::move(z2)));
        sk = "z3";
        m.insert(std::make_pair(std::move(sk), std::move(z3)));
        sk = "z4";
        m.insert(std::make_pair(std::move(sk), std::move(z4)));
        for (const auto &z : m) {
            z.second.print();
        }

        newZoo z5(12, 400);
        std::unordered_map<std::string, newZoo>::iterator itr = m.find("z2");
        if (itr != m.end()) {
            printf("Found z2!\n");
        } else {
            printf("Didn't find z2!\n");
        }
    }

    {
        std::list<std::string> l = {"apple", "pear", "grape", "watermelon", "pear"};
        std::unordered_set<std::string> uset(l.begin(), l.end());
        for (const auto &s : uset) {
            printf("Fruit: %s\n", s.c_str());
        }
        printf("Fruit size is: %llu\n", uset.size());

        std::unordered_set<std::string> uset2;
        uset2 = std::move(std::unordered_set<std::string>(l.begin(), l.end()));
        for (const auto &s : uset2) {
            printf("Fruit: %s\n", s.c_str());
        }
        printf("Fruit size is: %llu\n", uset2.size());

        uset.clear();
        uset.insert("apple");
        uset.insert("pear");
        uset.insert("grape");
        uset.insert("watermelon");
        uset.insert("pear");

        for (const auto &s : uset) {
            printf("Fruit: %s\n", s.c_str());
        }
        printf("Fruit size is: %llu\n", uset.size());
    }


    {
        std::vector<std::pair<std::string, std::string>> svec;
        std::list<std::pair<std::string, std::string>> slist;
        slist.push_back(std::make_pair("apple", "3"));
        slist.push_back(std::make_pair("pear", "5"));
        slist.push_back(std::make_pair("grape", "11"));
        slist.push_back(std::make_pair("watermelon", "9"));

        svec.assign(slist.begin(), slist.end());
        for (const auto &v : svec) {
            printf("Pair %s : %s\n", v.first.c_str(), v.second.c_str());
        }
    }

    {
        std::string s("\\");
        std::regex pattern("\\\\");
        if (std::regex_match(s, pattern)) {
            printf("Regex match: YES\n");
        } else {
            printf("Regex match: NO\n");
        }
    }

    {
        zoo_data z1("a", 3, 9);
        zoo_data z2("b", 1, 0);
        zoo_data z3("c", 8, 7);
        zoo_data z4("d", 88, 12);
        zoo_data z5("e", 90, 120);
        std::list<zoo_data> zlist;
        zlist.push_back(z1);
        zlist.push_back(z2);
        zlist.push_back(z3);
        zlist.push_back(z4);
        zlist.push_back(z5);

        zoo_data z6("c", 8, 7);
        auto itr = std::find(zlist.begin(), zlist.end(), z6);
        if (itr != zlist.end()) {
            printf("Found z5\n");
        } else {
            printf("Didn't find z5\n");
        }

        const std::string name("d");
        auto itr2 = std::find(zlist.begin(), zlist.end(), name);
        if (itr2 != zlist.end()) {
            printf("Found name\n");
        } else {
            printf("Didn't find name\n");
        }


    }

    // Test bitset 2
    {
        std::bitset<9> bset("000011111");
        std::string s(bset.to_string<char, std::string::traits_type, std::string::allocator_type>());
        std::cout << "'s' after converted is " << s << std::endl;

        std::bitset<9> bset2("001101111");
        std::string s1(bset2.to_string<char, std::string::traits_type, std::string::allocator_type>());
        std::cout << "'s' after converted is " << s1 << std::endl;
    }

    // back_inserter
    {
        std::vector<zoo> zvec;
        zoo z1(2 ,3);
        zoo z2(4, 5);
        zoo z3(7, 8);
        zoo z4(100, 0);

        std::list<newZoo> zlist;

        // std::copy(zvec.begin(), zvec.end(), std::back_inserter(zlist));
        zlist.resize(zvec.size());
        std::transform(zvec.begin(), zvec.end(), zlist.begin(), newZoo::convertFrom);
        printf("Size of zlist is: %llu\n", zlist.size());
        for (const auto &z : zlist) {
            z.print();
        }
    }

    {
        foo f("Company");
        boost::tuple<int, int> t(f.gettuple<int>(1, 2));
        printf("tuple: %d, %d\n", t.get<0>(), t.get<1>());
    }

    {
        int parray[2] = {23, 22};
        if (typeid(int) == typeid(decltype(parray[0]))) {
            printf("Current type is int\n");
        } else {
            printf("Current type is NOT int\n");
        }
        // typedef decltype(parray[0]) value_type;
        using value_type = std::decay<decltype(parray[0])>::type;
        value_type x = 9;
        printf("My self-defined type value is %d\n", x);
    }

    {
        int a[2] = {0, 0};
        double b[2] = {0, 0};
        float c[2] = {0, 0};

        temptest<int[2]>(a);
        temptest<double[2]>(b);
        temptest<float[2]>(c);
    }

    {
        zoo z1(3, 4);
        zoo_data z2("a", 101, 582);
        int a = getAniValue<zoo, true>(z1);
        int b = getAniValue<zoo, false>(z1);
        int c = getAniValue<zoo_data, true>(z2);
        int d = getAniValue<zoo_data, false>(z2);
        printf("a, b = %d, %d\n", a, b);
        printf("c, d = %d, %d\n", c, d);
    }

    {
        valClass vc;
        vc.msg_print("%d %s has %d apples\n", 1, "He", 3);
        vc.msg_print("%d %d planes are flying in the air on their way to %s", 2, 30, "Washington");
    }

    {
        std::string s1("Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec");
        char *s2 = (char*)malloc(s1.size() + 1);
        char *s2_original = s2;
        strcpy(s2, s1.c_str());
        char *token = NULL;
        int cnt = 0;
        token = strtok(s2, ",");
        while (token != NULL) {
            printf("%d: %s\n", ++cnt, token);
            token = strtok(NULL, ",");
        }
        free(s2_original);
    }

    {
        const std::string s("Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec");
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep(",");
        tokenizer toks(s, sep);
        for (BOOST_AUTO(itr, toks.begin()); itr != toks.end(); itr++) {
            std::cout << "[" << *itr << "]" << "\n";
            std::string tmp(*itr);
            std::cout << "[" << tmp << "]" << "\n";
        }
        std::cout << std::endl;
    }

    {
        const std::string s("Jan\nFeb\nMar\nApr\nMay\nJun\nJul\nAug\nSep\nOct\nNov\nDec");
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep("\n");
        tokenizer toks(s, sep);
        for (BOOST_AUTO(itr, toks.begin()); itr != toks.end(); itr++) {
            std::cout << "[" << *itr << "]" << "\n";
            std::string tmp(*itr);
            std::cout << "[" << tmp << "]" << "\n";
        }
        std::cout << std::endl;
    }

    {
        A a;
        B<A> b;
        // b.func = static_cast<void(A::*)()>(&A::print);
        b.func = &A::print;
        b.func(a);

        void (A::*f)() const = &A::print;
        // a->f();
    }

    {
        int lower = 20;
        int upper = 200000000;
        const int delta = (upper - lower + 1);
        for (int i = 0; i < 10; i++) {
            int r = rand();
            r = r % delta + lower;
            printf("Random %d: %d\n", i, r);
        }
    }

    {
        int i = 10;
        int x = 20;
        int y = 5;
        i += x - 2;
        x -= y + 1;
        printf("i= %d, x= %d\n", i, x);
    }

    {
        unsigned long int ulmax_value = ULONG_MAX;
        char s[256];
        snprintf(s, sizeof(s), "%lu", ulmax_value);
        char buff[20];
        snprintf(buff, sizeof(buff), "%7s", s);
        printf("%llu %lu %s\n", sizeof(unsigned long int), ulmax_value, buff);
    }

    {
        printf("=== Size of various kinds of types ===\n");
        printf("Size of                    int: %d\n", sizeof(int));
        printf("Size of           unsigned int: %d\n", sizeof(unsigned int));
        printf("Size of                   long: %d\n", sizeof(long));
        printf("Size of      unsigned long int: %d\n", sizeof(unsigned long int));
        printf("Size of          long long int: %d\n", sizeof(long long int));
        printf("Size of unsigned long long int: %d\n", sizeof(unsigned long long int));
        printf("Size of                  float: %d\n", sizeof(float));
        printf("Size of                 double: %d\n", sizeof(double));
        printf("Size of            long double: %d\n", sizeof(long double));
    }

    {
        // Test for std::copy
        std::vector<int> vec1(30, 7);
        std::vector<int> vec2(40, 5);
        int i = 0;
        printf("Before copy\n");
        for (const auto & v : vec1) {
            printf("value %d: %d\n", i++, v);
        }

        int start = 4;
        int range_half = 3;
        int end = start + 2 * range_half;
        std::copy(vec2.begin() + start, vec2.begin() + end, vec1.begin() + start);

        i = 0;
        printf("After copy\n");
        for (const auto & v : vec1) {
            printf("value %d: %d\n", i++, v);
        }
    }

    {
        int a = 9;
        printf("Address of a is: %p\n", &a);
        int &aa = a;
        printf("Address of aa is: %p\n", &aa);
    }

    {
        /*
        if (test_temporary_param(std::string())) {
            printf("test_temporary_param returns true\n");
        } else {
            printf("test_temporary_param returns false\n");
        }
        */
    }

    {
        int x = 10;
        std::vector<std::unordered_set<int>> wz_list_from(x);
        wz_list_from[3].insert(7);
        wz_list_from[3].insert(10);
        wz_list_from[5].insert(1);
        wz_list_from[9].insert(23);

        int cnt = 0;
        for (const auto &v : wz_list_from) {
            for (const auto &value: v) {
                printf("[%d] Value is: %d\n", cnt, value);
            }
            cnt++;
        }
    }

    // shared_ptr test
    {
#if 0
        std::vector<std::shared_ptr<valClass>> vvec;
        vvec.resize(vsize);
        for (std::size_t i = 0; i < vsize; i++) {
            vvec[i] = std::make_shared<valClass>(i);
        }

        std::vector<std::shared_ptr<valClass>> vvec2;
        vvec2 = vvec;

        printf("vvec:\n");
        for (auto &p : vvec) {
            if (p) {
                p->print();
            } else {
                printf("Pointer is null\n");
            }
        }
        vvec.clear();

        printf("vvec2:\n");
        for (auto &p : vvec2) {
            if (p) {
                p->print();
            } else {
                printf("Pointer is null\n");
            }
        }
        vvec2.clear();

        printf("End of shared_ptr test\n");
#endif // 0

        printf("Start of shared_ptr test\n");

        const size_t vec_size = 3;
        std::vector<std::vector<std::shared_ptr<valClass>>> VALC_VEC;
        for (size_t i = 0; i < vec_size; ++i) {
            std::vector<std::shared_ptr<valClass>> v;
            for (size_t j = 0; j < vsize; ++j) {
                v.push_back(std::make_shared<valClass>(i*10 + j, 2 * (i*10 + j)));
            }
            VALC_VEC.push_back(v);
        }

        int vcnt = 0;
        const int alen = VALC_VEC.size();
        printf("size of VALC_VEC is %d\n", alen);
        for (const auto &vec : VALC_VEC) {
            printf("vec %d:\n", ++vcnt);
            for (const auto &p : vec) {
                p->print();
            }
        }

        printf("End of shared_ptr test\n");

    }

    {
        std::unordered_map<int, std::unordered_map<int, int>> pvec;
        // std::vector<std::map<int, int>> pvec;
        std::unordered_map<int, int> m;
        m.insert(std::make_pair(3, 2));
        pvec[0] = m;

        std::vector<std::unordered_set<int>> svec;
        std::unordered_set<int> iset;
        iset.insert(3);
        svec.push_back(iset);

        printf("vector-set values:\n");
        for (const auto &s : svec) {
            for (const auto &iv : s) {
                printf("%d\n", iv);
            }
        }

    }


    {
        valClass vc;
        boost::optional<const valClass&> bop(vc);
        // boost::optional<const valClass&> bop;
        boost_optional_reference_test(bop);
    }

    {
        std::vector<int> ivec(10, -1);
        printf("Size of ivec is: %llu\n", ivec.size());
        for (const auto v : ivec) {
            printf("%d ", v);
        }
        printf("\n");

        ivec.assign(ivec.size(), 9);
        printf("Size of ivec is: %llu\n", ivec.size());
        for (const auto v : ivec) {
            printf("%d ", v);
        }
        printf("\n");
    }

    {
        std::vector<std::unordered_map<int, int>> imapvec(10);
        for (const auto &m : imapvec) {
            printf("Size of map is: %llu\n", m.size());
        }
    }

    // Core dump will happen if this block is enabled.
    // Disable it for now. (2022-06-19)
#if 0
    {
        printf("data.print():\n");
        data_storage data;
        data.print();
        printf("\n");

        printf("data.print() after reset:\n");
        data.m_value.reset(new valClass(23));
        data.print();
        printf("\n");

        printf("data2.print():\n");
        data_storage data2;
        data2.print();
        printf("\n");

        printf("data2.print() after reset:\n");
        data2.m_value.reset(new valClass(99));
        data2.print();
        printf("\n");

        printf("data.print() & data2.print() after assignment:\n");
        data = data2;
        data.print();
        data2.print();
        printf("\n");

        printf("data3.print()\n");
        data_storage *data3 = new data_storage;
        data3->print();
        printf("\n");

        printf("data3.print() & data2.print() after memcpy:\n");
        memcpy(data3, &data2, sizeof(data_storage));
        data3->print();
        data2.print();
        printf("\n");

        printf("data2.print() after deleting data3:\n");
        delete data3;
        data3 = NULL;
        data2.print();
        printf("\n");
    }
#endif // 0

    {
        std::shared_ptr<int> p(new int(3));
        p.reset();
        p.reset();

        std::shared_ptr<valClass> vp;
        if (vp.get()) {
            printf("vp is NOT NULL\n");
        } else {
            printf("vp is NULL\n");
        }

    }

    {
        std::bitset<64> bset(0x01);
        std::cout << "bset: " << bset.to_string() << "\n";

        unsigned long i = bset.to_ulong();

        std::bitset<64> bset2(i);
        std::cout << "bset2: " << bset2.to_string() << "\n";
    }

    {
        std::vector<valClass> vecval;
        vecval.push_back(valClass(1));
        vecval.push_back(valClass(2));
        vecval.push_back(valClass(3));
        vecval.push_back(valClass(4));

        std::vector<valClass> vecval2;
        vecval2.push_back(valClass(5));
        vecval2.push_back(valClass(6));
        vecval2.push_back(valClass(7));
        vecval2.push_back(valClass(8));

        int size = vecval.size() + vecval2.size();
        valClass *pval = new valClass[size];
        std::copy(vecval.begin(), vecval.end(), pval);
        std::copy(vecval2.begin(), vecval2.end(), pval + vecval.size());
        for (int i = 0; i < size; ++i) {
            pval[i].print();
        }

        printf("===\n");
        std::copy(pval + 4, pval + 8, vecval.begin());
        for (int i = 0; i < vecval.size(); ++i) {
            vecval[i].print();
        }

    }

    {
         const std::vector<int> vec_poly_src_1{2,6,2,5,3,5,4,5,5,5,6,5,6,4,7,4,8,4,9,4,10,4,10,5,11,5,13,5,15,5,15,6,14,6,13,6,11,6,9,6,7,6,6,6,5,6,4,6,3,6};
         const std::vector<int> vec_poly_src_2{2,9,2,8,3,8,3,7,5,7,5,8,5,10,3,10,3,9};
         const std::vector<int> vec_poly_src_3{2,15,2,13,2,11,3,11,5,11,5,13,4,13,4,15};
         const std::vector<int> vec_poly_src_4{6,11,6,10,7,10,7,11};
         const std::vector<int> vec_poly_src_5{10,14,10,13,10,11,10,10,11,10,13,10,13,11,11,11,11,12,11,14};
         const std::vector<int> vec_poly_src_6{7,9,7,8,8,8,8,7,10,7,10,9,9,9,8,9};

         const int PLEN = 6;
         const std::vector<int>* pvec[6] = {&vec_poly_src_1,
                                            &vec_poly_src_2,
                                            &vec_poly_src_3,
                                            &vec_poly_src_4,
                                            &vec_poly_src_5,
                                            &vec_poly_src_6 };
         for (int i = 0; i < PLEN; ++i) {
             const int len = pvec[i]->size();
             printf("The size of pvec[%d] is %d\n", i, len);
         }

         std::array<const char*, 3> sarr{"abut", "corwt", "seg"};
         const int len = sarr.size();
         for (int i = 0; i < len; ++i) {
             printf("%d: %s\n", i, sarr[i]);
         }
    }


    // Sparse matrix to dense matrix
    {
#if 0
#define M 6
#define N 7
#define DM 4
#define DN 4
        int m = M;
        int n = N;
        double A[M * N] = {
            1, 0, 2, 0, 0, 0, 0,
            0, 3, 0, 0, 0, 0, 0,
            0, 0, 5, 0, 0, 0, 0,
            0, 0, 0, 7, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0
        };

        print_matrix("Matrix A:", m, n, A, m);

        int dm = DM;
        int dn = DN;
        double Ad[DM * DN];

        int res = sparse2dense(A, m, n, Ad, dm, dn);
        if (res < 0) {
            printf("sparse2dense fails\n");
        } else {
            print_matrix("Matrix Ad:", dm, dn, Ad, dm);
        }

#undef M
#undef N
#undef DM
#undef DN
#endif // 0
    }

    {
        std::tuple<short, int, long, long long, unsigned short, unsigned, unsigned long, float, double, int, int, int, int, int, int>
            t(1, 2, 3, 4, 5, 6, 7, 8.0, 9.0, 9, 9, 9, 9, 9, 9);
        std::remove_reference<decltype(std::get<0>(t))>::type x = 2;
        printf("x is %d\n", x);
    }

    {
        std::vector<int> vec1;
        std::vector<int> vec2;
        printf("vec1 capacity is %d\n", vec1.capacity());
        printf("vec2 capacity is %d\n", vec2.capacity());

        vec1.resize(1);
        vec2.reserve(1);
        vec2.resize(1);
        printf("vec1 capacity is %d\n", vec1.capacity());
        printf("vec2 capacity is %d\n", vec2.capacity());
    }

    {
        boost::shared_ptr<zoo_data> zptr(new zoo_data("Green", 3, 10));
        // zoo_data zd("Green", 3, 10);
        boost::any anyval = zptr;
        const std::type_info &ti = anyval.type();
        std::cout << "anyval.type() = " << ti.name() << "\n";

    }

    {
        // auto greater_than = [](int x, int y) { return x > y; };
        // auto greater_than_one = std::bind(greater_than, std::placeholders::_1, 1);
        auto greater_than_one = [](int x) { return x > 1; };
        bool val0 = custom_compare(5);
        bool val1 = custom_compare(5, nullptr);
        bool val2 = custom_compare(5, greater_than_one);
        printf("val0 = %s\n", val0 ? "True" : "False");
        printf("val1 = %s\n", val1 ? "True" : "False");
        printf("val2 = %s\n", val2 ? "True" : "False");
    }

    {
        const std::string str1("protosolveCorbasicDebug.");
        char str2[30] = {'\0'};
        size_t i = 0;
        for (; i < str1.size(); ++i) {
            str2[i] = str1[i];
        }
        str2[i] = '0';
        const int len = str1.size();
        int res = str1.compare(0, len, str2, len);
        printf("str1 = %s\n", str1.c_str());
        printf("str2 = %s\n", str2);
        printf("Compare result is %d\n", res);
        const std::string s(str2);
        const std::string ss(s.substr(len));
        printf("ss = %s\n", ss.c_str());
    }


#if 0
    {
        // int *iarr = new int[10];
        // SharedArray<int> arr(iarr, iarr + 10);
        int *iarr = new int[10];
        SharedArray<int> arr(iarr, iarr + 10);
        SharedArray<int> arr2(arr.begin(), arr.end());

        std::vector<bool> bvec(2);
        bvec[0] = true;
        bvec[1] = false;
        printf("%p\n", &(bvec[0]));
        printf("%p\n", &(bvec[1]));
    }
#endif

    {
        double pvalue = 1.99999;
        int i = round(pvalue);
        std::cout << i << std::endl;
    }

    {
        int ivec[9] = {-1, -1, -1, -1,-1, -1,  -1, -1, -1};
        ivec[0] = ISK_TYPE::TYPE_0;
        ivec[1] = ISK_TYPE::TYPE_1;
        ivec[2] = ISK_TYPE::TYPE_2;
        ivec[3] = ISK_TYPE::TYPE_3;
        ivec[4] = ISK_TYPE::TYPE_UNKOWN;
        ivec[5] = ISK_TYPE::TYPE_TRIPLE;
        ivec[6] = ISK_TYPE::TYPE_CONCAT;
        ivec[7] = ISK_TYPE::TYPE_MERGE;
        ivec[8] = ISK_TYPE::TYPE_TBONE;
        std::cout << "Enum types:" << std::endl;
        for (int i = 0; i < 9; ++i) {
            std::cout << ivec[i] << std::endl;
        }
    }

    {
       std::vector<zoo> zvec;
       for (int i = 0; i < 10; ++i) {
          zvec.push_back(zoo(20 - i, 3));
       }

       std::cout << "Before sort:" << std::endl;
       for (const auto &v : zvec) { v.print(); }

       std::sort(zvec.begin(), zvec.end(), zoo::less_than);

       std::cout << "After sort:" << std::endl;
       for (const auto &v : zvec) { v.print(); }
    }


    {
       std::vector<int> v1{5,6,7,8,9,10,11,12,13,/*|*/30,31,32,33,34,35,36,/*|*/50,51,52,53};

#if 0
       std::vector<int> v2{41,28,10,25,48,13,8,27,45,47,11,9,26,42,44,29,46,40,43,12};
#else
       // std::vector<int> v2{8,9,10,11,12,13,25,26,27,28,29,40,41,42,43,44,45,46,47,48};
       std::vector<int> v2{48,47,46,45,44,43,42,41,40,29,28,27,26,25,13,12,11,10,9,8};
#endif

       std::sort(v1.begin(), v1.end(), std::greater<int>());
       std::vector<std::tuple<int, int, int, int>> tvec;
       printf("Size of v1/v2 = %lu,%lu\n", v1.size(), v2.size());

       int len = v1.size();
       for (int i = 0; i < len; ++i) {
          tvec.emplace_back(std::make_tuple(0, v1[i], 1, v2[i]));
       }

       separate_segments(tvec, std::make_pair(0, len -1), 10);
    }

    {
       std::vector<std::pair<int, int>> firstVec, secondVec;
       printf("Size: %lu %lu\n", firstVec.size(), secondVec.size());
       for (int i = 0; i < 10; ++i) {
          firstVec.emplace_back(10, 10);
       }
       for (const auto &v : firstVec) {
          printf("%d %d\n", v.first, v.second);
       }

       for (auto &v : firstVec) {
          v.first -= 2;
          v.second -= 9;
       }

       for (const auto &v : firstVec) {
          printf("%d %d\n", v.first, v.second);
       }
    }

    {
       std::vector<int> ivec0{0,0,0,0,0,1,1,1,2,2,2,3,3,4,5,5,5,5,5,5,6,7};
       std::vector<int> ivec1{1,1,1,2,2,2,3,3,4,5,5,5,5,5,5,6,7,8,8,8,9,9};

       size_t idx = 0;
       int m = ivec0[idx];
       int n = ivec1[idx];
       const size_t len = ivec0.size();

       size_t i = 0;
       for (; i < len + 1; ++i) {
          /*
          if (i == len) {
             printf("---------- range %lu-%lu, path = %d vs. %d\n", idx, i - 1, m, n);
             continue;
          }
          */

          if (i == len || m != ivec0[i] || n != ivec1[i]) {
             printf("---------- range %lu-%lu, path = %d vs. %d\n", idx, i - 1, m, n);
             idx = i;
             if (i != len) {
                m = ivec0[i];
                n = ivec1[i];
             } else {
                printf("Last one\n");
             }
          }
          if (i != len) {
             printf("%lu: %d %d\n", i, ivec0[i], ivec1[i]);
          }
       }
       // printf("---------- range %lu-%lu, path = %d vs. %d\n", idx, i - 1, m, n);
    }

    {
       std::vector<int> x(10, 0);

       for (size_t i = 0; i < 10; ++i) {
          vec_list v1(std::move(x));
          v1.print_all();

          // x.resize(10, i + 5);
          x.clear();
          x.resize(10, i + 5);
          // std::fill(x.begin(), x.end(), i);
       }

    }

    {
       GeoPoint ap(23, 78);
       ap.pprint();
       //ap.pprint(23);

       Point3D bp(11, 0, 99);
       bp.pprint();

       Point3D cp(std::move(bp));
       cp.pprint();
    }

    {
       auto pvalid = [](const std::vector<int> &vec) {
          if (validate_order(vec)) {
             printf("Valid order\n");
          } else {
             printf("Invalid order\n");
          }
       };
       std::vector<int> x{0, 0, 0, 1, 1, 1, 2, 3, 3, 3, 4, 4, 4, 5};
       std::vector<int> y{0, 0, 0, 2, 2, 1, 1, 3, 3, 3, 4, 4, 4, 5};
       std::vector<int> z{0, 0, 0, 1, 1, 1, 2, 3, 3, 3, 0, 0, 4, 5};
       std::vector<int> zz{1, 2, 3, 4, 0, 10, 9, 8, 7, 6, 3, 2};
       pvalid(x);
       pvalid(y);
       pvalid(z);
       pvalid(zz);
    }


    {
       printf("========\n");
       Point3D ap(11, 0, 99);
       ap.pprint();

       Point3D bp(345, 102, 915);
       bp.pprint();

       ap.swap(bp);

       ap.pprint();
       bp.pprint();

       printf("====R===\n");
       ap.GeoPoint::pprint();
       printf("====S===\n");
    }

    {
       using hotspot_map = std::unordered_map<std::pair<int32_t, bool>, std::unordered_set<std::pair<int32_t, bool>, keyHash>, keyHash>;

       std::unordered_set<std::pair<int32_t, bool>, keyHash> uset;
       uset.insert(std::make_pair(3, true));
       printf("Size of uset: %lu\n", uset.size());

       hotspot_map m;
       std::pair<int32_t, bool> x;
       m.insert(std::make_pair(x, uset));
       printf("Size of uset: %lu\n", m.size());
    }


    {
       std::list<int> llist{1, 2, 3, 5, 8, 7, 5, 0, 5, 2, 5};
       std::list<int>::iterator fi = llist.begin();
       for (auto itr = llist.begin(); itr != llist.end(); ++itr) {
          if (*itr == 2) {
             llist.erase(itr);
             break;
          }
       }

       printf("List values after erase\n");
       for (auto i : llist){
          printf("%d ", i);
       }
       printf("\n");
       printf("The 1st after removal is: %d\n", *(fi));
       // printf("The 2nd after removal is: %d\n", *(++fi));
       // printf("The 2nd after removal is: %d\n", *(fi + 1));
       printf("The 2nd after removal is: %d\n", *(std::next(fi)));

       std::list<int>::iterator ii = llist.begin();
       printf("First one is: %d\n", *ii);
       --ii;
       printf("The one before first one is: %d\n", *ii);
       ++ii;
       printf("First one is: %d\n", *ii);
    }

    {
       zoo_data zdata("SilverZoo", 50, 120);
       zdata.generate_year_workers();
       std::cout << "Worker at a certain year " << zdata[5].first << ": " << zdata[5].second << "\n";
       zdata[5] = std::make_pair(2001, 856);
       std::cout << "Worker at a certain year " << zdata[5].first << ": " << zdata[5].second << "\n";
    }

    {
       // typedef ISK_TYPE
       using FIX_TYPE = ISK_TYPE;
       std::cout << "Fix type is " << FIX_TYPE::TYPE_TRIPLE << "\n";
       std::cout << "Fix type is " << FIX_TYPE::TYPE_TBONE << "\n";
    }

#if 0
    {
       boost::shared_ptr<Point3D> p(new Point3D(2, 4, 6));

       Point3D *rawp = p.get();
       Point3D *none = nullptr;

       // p.reset(none, release_deleter<Point3D>());
       p.reset(none, do_nothig_deleter<Point3D>());

       if (p) {
          std::cout << "p now is NOT null\n";
       } else {
          std::cout << "p now is null\n";
       }

       if (rawp) {
          std::cout << "rawp now is NOT null: " << rawp << "\n";
       } else {
          std::cout << "rawp now is null\n";
       }

       rawp->pprint();

       delete rawp;
    }
#endif // 0

    {
       boost::shared_ptr<Point3D> p(new Point3D(2, 4, 6));
       std::cout << "Current p usage: " << p.use_count() << "\n";

       boost::shared_ptr<Point3D> &pother = p;
       std::cout << "Current p usage after alias: " << p.use_count() << "\n";

       boost::shared_ptr<Point3D> pother2 = p;
       std::cout << "Current p usage after alias: " << p.use_count() << "\n";
    }

    {
       for (int i = 0; i < 10; ++i) {
          std::list<int> g{i};
          printf("Element in group: %d\n", g.size());
       }
       // initializer_list for std::list
       std::list<int> group{23};
       printf("Element in group: %d\n", group.size());
       for (const auto &v : group) {
          printf("%d\n", v);
       }
    }

    {
       const int x = 5;
       const int y = 2;

       // const double z = (x + y) / 2;
       const double z = (x + y) / 2.0;
       printf("double varible z is %.2f\n", z);
    }

    {
       int n = 0;
       /*
       int value = 0;
       bool dir = true;
       */
       int value = 10;
       bool dir = false;
       while (n < 10) {
          ++n;
          dir ? ++value : --value;
       }
       printf("value = %d\n", value);
    }

    // {
    //    int x = 10;
    //    int n = 0;
    //    int x_div_n = 0;
    //    try {
    //       int x_div_n = x / n;
    //    } catch (...) {
    //       printf("Exception catched.\n");
    //    }

    //    printf("x_div_n = %d\n", x_div_n);
    // }

#if 1
    {
       std::list<int> ilist{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

       std::list<int>::iterator itr = ilist.begin();

       printf("--- List test ---\n", itr);
       std::list<int>::iterator itr_0 = itr;
       printf("0 %d\n", *itr);
       std::list<int>::iterator itr_1 = (itr = std::next(itr));
       printf("1 %d\n", *itr);
       std::list<int>::iterator itr_2 = (itr = std::next(itr));
       printf("2 %d\n", *itr);
       std::list<int>::iterator itr_3 = (itr = std::next(itr));
       printf("3 %d\n", *itr);
       std::list<int>::iterator itr_4 = (itr = std::next(itr));
       printf("4 %d\n", *itr);
       std::list<int>::iterator itr_5 = (itr = std::next(itr));
       printf("5 %d\n", *itr);
       std::list<int>::iterator itr_6 = (itr = std::next(itr));
       printf("6 %d\n", *itr);
       std::list<int>::iterator itr_7 = (itr = std::next(itr));
       printf("7 %d\n", *itr);
       std::list<int>::iterator itr_8 = (itr = std::next(itr));
       printf("8 %d\n", *itr);
       std::list<int>::iterator itr_9 = (itr = std::next(itr));
       printf("9 %d\n", *itr);

       ilist.erase(itr_4);

       ilist.insert(itr_6, 255);

       printf("%d\n", *itr_5);
       printf("%d\n", *itr_6);

       printf("--- List test print ---\n", itr);
       for (const auto i : ilist) {
          printf("%d, ", i);
       }
       printf("\n");

    }
#endif // 0

    {
       // typedef std::vector<int32_t> index_type;
       using index_type = std::tuple<int32_t, int32_t, int32_t>;

       // ??? Not a constant lambda expression?
       // constexpr auto set_hash_value = [](const index_type &t) -> std::size_t
       auto set_hash_value = [](const index_type &t) -> std::size_t
       {
          printf("set_hash_valu executed once\n");
          std::vector<int32_t> ivec{std::get<0>(t), std::get<1>(t), std::get<2>(t)};
          std::sort(ivec.begin(), ivec.end());
          std::size_t hvalue = 0;
          /*
          boost::hash_combine(hvalue, std::get<0>(t));
          boost::hash_combine(hvalue, std::get<1>(t));
          boost::hash_combine(hvalue, std::get<2>(t));
          */
          boost::hash_combine(hvalue, ivec[0]);
          boost::hash_combine(hvalue, ivec[1]);
          boost::hash_combine(hvalue, ivec[2]);
          return hvalue;
       };

       // ??? Not a constant lambda expression?
       // constexpr auto set_key_equal = [](const index_type &ta, const index_type &tb) -> bool
       auto set_key_equal = [](const index_type &ta, const index_type &tb) -> bool
       {
          printf("set_key_equal executed once\n");
          std::vector<int32_t> a{std::get<0>(ta), std::get<1>(ta), std::get<2>(ta)};
          std::vector<int32_t> b{std::get<0>(tb), std::get<1>(tb), std::get<2>(tb)};
          assert((a.size() == 3) && (b.size() == 3));
          std::sort(a.begin(), a.end());
          std::sort(b.begin(), b.end());
          for (int i = 0; i < 3; ++i) {
             if (a[i] != b[i]) {
                return false;
             }
          }

          return true;
       };

       std::unordered_set<index_type, decltype(set_hash_value), decltype(set_key_equal)> iset(100, set_hash_value, set_key_equal);

       index_type t0{0, 1, 2};
       index_type t1{1, 2, 4};
       index_type t2{1, 0, 2};
       iset.insert(t0);
       iset.insert(t1);

       printf("Start to find t2\n");
       if (iset.find(t2) == iset.end()) {
          printf("t2 is NOT found in iset\n");
       } else {
          printf("t2 is found in iset\n");
       }
       printf("Finished finding t2\n");

       bool test_0 = set_key_equal(t0, t2);
       printf("test_0 = %s\n", test_0 ? "True" : "False");

    }

#if 0
    {
       using valClass::ENUM_KEYS = VENUM;
       VENUM enval0 = VENUM::DEV;
       VENUM enval1 = VENUM::FIL;
       // VENUM enval2 = VENUM::LLX;
       if (enval0 == enval1) {
          printf("enval0 == enval1\n");
       } else {
          printf("enval0 != enval1\n");
       }
    }
#endif // 0

    {
       std::unordered_set<std::pair<int32_t, int32_t>, keyPairHash, keyPairEqual> uset;
       uset.insert(std::make_pair(3, 2));
       uset.insert(std::make_pair(9, 5));
       for (const auto &v : uset) {
          printf("Pair: %d-%d\n", v.first, v.second);
       }
       const std::pair<int32_t, int32_t> p0{4,6};
       const std::pair<int32_t, int32_t> p1{2,3};
       const std::pair<int32_t, int32_t> p2{5,9};

       if (uset.find(p0) == uset.end()) {
          printf("p0 doesn't exist!\n");
       } else {
          printf("p0 exist!\n");
       }

#if 1
       if (uset.find(p1) == uset.end()) {
          printf("p1 doesn't exist!\n");
       } else {
          printf("p1 exist!\n");
       }
       
       if (uset.find(p2) == uset.end()) {
          printf("p2 doesn't exist!\n");
       } else {
          printf("p2 exist!\n");
       }
#endif // 0
    }

    {
       partial_specialization_test<ISK_TYPE::TYPE_0>(10);
       partial_specialization_test<ISK_TYPE::TYPE_1>(10);
    }

    {
       std::list<uint32_t> listA{10, 11, 12, 13};
       std::list<uint32_t> listB{5, 6, 7, 8, 9};
       std::list<uint32_t> listC{0, 1, 2, 3, 4};

       auto &dlist = listA;

       /*
       listA.insert(listA.begin(), listB.begin(), listB.end());
       listA.insert(listA.begin(), listC.begin(), listC.end());
       */
       dlist.insert(dlist.begin(), listB.begin(), listB.end());
       dlist.insert(dlist.begin(), listC.begin(), listC.end());

       printf("===List insertion test===\n");
       for (const auto v : dlist) {
          printf("%u ", v);
       }
       printf("\n");
    }


    {
       std::list<int> intlist = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
       std::list<int> tmplist;

       auto itr = intlist.begin();
       auto nextitr = intlist.begin();
       while (itr != intlist.end()) {
          nextitr = std::next(itr);
          if (*itr % 2 == 0) {
             tmplist.splice(tmplist.end(), intlist, itr);
          }
          itr = nextitr;
       }

       printf("===intlist===\n");
       for (const auto v : intlist) {
          printf("%d ", v);
       }
       printf("\n");

       printf("===tmplist===\n");
       for (const auto v : tmplist) {
          printf("%d ", v);
       }
       printf("\n");
    }

    std::cout << "---------- END OF FUNCTION: " << __FUNCTION__ << std::endl;

} /* function normal_test_all */


} /* End of namespace NORMAL */
