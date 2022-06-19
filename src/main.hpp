#ifndef _MAIN_HPP
#define _MAIN_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <memory>
#include <fstream>
#include <forward_list>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <functional>
#include <set>
#include <map>
#include <unordered_map>
#include <tuple>
#include <random>
#include <chrono>
#include <ctime>
// #include <unistd.h>

#include <boost/bimap.hpp>
#include <boost/circular_buffer.hpp>
// #include <boost/functional/hash.hpp>

namespace mainspace {


void test_boost_all() {
    std::cout << "Start of function: " << __FUNCTION__ << "\n";

    // boost::bimap
    {
        printf("\n");
        typedef boost::bimap<std::string, int> bimap;
        bimap animals;

        animals.insert({"cat", 4});
        animals.insert({"shark", 0});
        animals.insert({"spider", 8});

        std::cout << animals.left.count("cat") << '\n';
        std::cout << animals.right.count(8) << '\n';
    }

    // boost::circular_buffer
    {
        printf("\n");
        typedef boost::circular_buffer<int> circular_buffer;
        circular_buffer cb{3};

        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);

        std::cout << std::boolalpha << cb.is_linearized() << '\n';

        circular_buffer::array_range ar1, ar2;

        ar1 = cb.array_one();
        ar2 = cb.array_two();
        std::cout << ar1.second << ";" << ar2.second << '\n';

        for (int i : cb)
            std::cout << i << '\n';

        cb.linearize();

        ar1 = cb.array_one();
        ar2 = cb.array_two();
        std::cout << ar1.second << ";" << ar2.second << '\n';
    }

    std::cout << "End of function: " << __FUNCTION__ << "\n";

} /* func tester_boost_all */


/// ATTENTION!!!
/// the warning option '-Wall' has been turn off since no need for this project
/// just testing new features for c++11

int A_CONST_J = 0;
constexpr int A_CONST_I = 42;
void tester_constexpr() {
    std::cout << "START OF " << __FUNCTION__ << std::endl;
    constexpr int x = 9;
    std::cout << x << std::endl;

    int t = -1;

    // A pointer points to a constant value
    const int *p = nullptr;
    int *np = new int(99);
    p = np;

    // A pointer points to a constant value
    // But this value need to be a constant
    constexpr int *q = nullptr;
    // q = &t; // Error to assign a mutable value to read-only 'q'

    int *ip = new int(88);
    // q = ip; // Error to assign a mutable value to read-only 'q'

    // A constant pointer points to a constant
    constexpr const int *pp = &A_CONST_I;// OK, since A_CONST_I is defined globaly(outside functions)
    constexpr int *qq = &A_CONST_J;// OK, since A_CONST_J is defined globaly(outside functions)

    delete ip;
    delete np;

    std::cout << "End of tester\n" << std::endl;
}


void tester_alias_declaration() {
    std::cout << "START OF " << __FUNCTION__ << std::endl;
    using MY_INT = int;
    MY_INT test = 233;
    std::cout << test << std::endl;
    std::cout << "End of tester\n" << std::endl;
}

void tester_auto() {
    std::cout << "START OF " << __FUNCTION__ << std::endl;

    std::string val1("Good");
    std::string val2(" Day");
    auto item = val1 + val2;
    std::cout << item << std::endl;

    auto i = 0, *p = &i; // Good, since basic types are both 'int'
    // auto sz = 0, pi = 3.14; // Error, since basic types are differnt(int and double)

    const int ci = i, &cr = ci;
    // For non-reference variable, top-level constant identifier will be ignored
    auto b = ci; // b is an integer
    auto c = cr; // c is an integer
    auto d = &i; // d is a pointer to integer
    auto e = &ci; // e is a pointer to constant integer
    const auto f = ci; // f is a constant integer

    // For reference variable, top-level constant identifier will !!NOT!! be ignored
    auto &g = ci; // g is a reference to a CONSTANT integer
    // auto &h = 42; // Error! h is a variable(mutable), 42 is literal constant
    const auto &j = 42; // Good, j is a const reference to a literal constant

    auto k = ci, &l = i; // Good, k is an integer, l is a reference to an integer
    auto &m = ci, *pp = &ci; // Good, m is an const reference to a constant value
                            // pp is a pointer to a constant value

    std::cout << "End of tester\n" << std::endl;
}


void tester_decltype() {
    std::cout << "START OF " << __FUNCTION__ << std::endl;

    const int ci = 0, &cj = ci;
    decltype(ci) x = 0; // x is a constant integer
    decltype(cj) y = x; // y is a reference to a constant integer
    // decltype(cj) z; // Error! z is a reference to a constant integer, must be initialized

    std::cout << "End of tester\n" << std::endl;
}


// Inside class initialization
class tester_initialization_in_class {
public:
    tester_initialization_in_class() {}
    tester_initialization_in_class(int a): m_a(a) {}
    tester_initialization_in_class(double b): m_b(b) {}
    tester_initialization_in_class(int a, double b): m_a(a), m_b(b) {}

public:
    void print() const {
        std::cout << "m_a: " << m_a << ", m_b: " << m_b << std::endl;
    }
public:
    int m_global = 3;
private:
    int m_a = 1;
    double m_b = 23.1;
};

    
constexpr int getLiteralVal() {
    return 88;
}

// Validation class
class valClass {
public:
    valClass() = default; // If 'default' is specified, compiler will generate the body of this ctor automatically
    valClass(int a): m_a(a) {}
    valClass(double b): m_b(b) {}
    valClass(int a, double b): m_a(a), m_b(b) {}

    // Below are 2 delegating constructors
    // Notice that m_str shouldn't be in the initialization list because of delegation
    valClass(const std::string& s) : valClass() { m_str = s; }
    valClass(const std::string& s, int a) : valClass(a) { m_str = s; }

    // IMPORTATN NOTICE:
    //   Classes that define a move constructor or move-assignment operator must
    //   also define their own copy operations. Otherwise, those members are deleted
    //   by default.
    //
    // Use rvalue-reference to do copy initialization
    // 'noexcept' here to declare no exception garanteed
    valClass(valClass &&vc) noexcept; // a move constructor
    valClass(const valClass &vc); // a copy constructor

    //---------------------------------------------
    //
    // Important Notice for reference qualifier
    //
    //  Since 'sorted' is a same name
    //  so all of them have to be added reference qualifier
    //  or none of them should be added a reference qualifier
    //  Which is to say, all have or all have none
    //
    //---------------------------------------------
    // Reference qualifier, can be used as rvalue
    // No matter inside or outside class, '&&' must be kept
    valClass sorted() &&;
    // Can be used as lvalue, '&' must be kept inside or outside class
    valClass sorted() const &;

public:
    valClass& operator=(const valClass &d); // it will be declared as '=default' below outside class definition

    // this can also be declared as 'delete' to prevent assignment
    // But then it should be declared inside the class body, not outside
    // valClass& valClass::operator=(const valClass &d) = delete

public:
    enum ErrCode {TA = 0, TB, TC, TD};

public:
    virtual void print() const {
        std::cout << "m_a: " << m_a << ", m_b: " << m_b << std::endl;
    }

    virtual void print_id() const {
        std::cout << "ID is: " << m_a << "\n";
    }

    inline const int get_id() const { return m_a; }
    inline int get_id() { int r = m_a; return r; }

#if 0
public:
    static std::size_t hasher(const valClass &v) {
        return std::hash<int>()(v.get_id());
    }
    static bool equalOperator(const valClass &lhs, const valClass &rhs) {
        return lhs.get_id() == rhs.get_id();
    }

    bool operator==(const valClass &rhs) const {
        return get_id() == rhs.get_id();
    }
#endif // 0

public:
    void error_msg(std::initializer_list<std::string> il);
    void error_msg(ErrCode e, std::initializer_list<std::string> il);

    const std::vector<std::string> process_list_return_1();
    const std::vector<int> process_list_return_2();
    const std::vector<int> process_list_return_empty();

    // constexpr function in a class must be static
    // It can be non-static outside of a class
    static constexpr int getLiteralVal() { return 23; }

public:
    std::vector<int> m_vec;

public:
    int m_global = 3;

private:
    // Initialization inside class must use '=' or '{...}']
    int m_a = 1;
    double m_b = 23.1;
    std::string m_str{"yy"};
}; /* class valClass */

struct hashfunc_valClass {
    std::size_t operator() (const valClass &k) const {
        return std::hash<int>()(k.get_id());
    }
};

struct equal_valClass {
    bool operator() (const valClass &lhs, const valClass &rhs) const {
        return lhs.get_id() == rhs.get_id();
    }
};

// Reference qualifier, can be used as rvalue
// No matter inside or outside class, '&&' must be kept
valClass valClass::
sorted() && {
    std::sort(m_vec.begin(), m_vec.end());
    return *this;
}

// Can be used as lvalue, '&' must be kept inside or outside class
valClass valClass::
sorted() const & {
    valClass ret(*this);
    std::sort(ret.m_vec.begin(), ret.m_vec.end());

    return ret;
}

// this can also be declared as 'delete' to prevent assignment
// But then it should be declared inside the class body, not outside
valClass& valClass::operator=(const valClass &d) = default;

// If you define a move constructor, you must define a
// copy constructor at the same time
valClass::valClass(valClass &&vc) noexcept {
    m_global = vc.m_global;
    m_a = vc.m_a;
    m_b = vc.m_b;
    m_str = vc.m_str;
}

valClass::valClass(const valClass &vc) {
    m_global = std::move(vc.m_global);
    m_a = std::move(vc.m_a);
    m_b = std::move(vc.m_b);
    m_str = std::move(vc.m_str);
}

void valClass::
error_msg(std::initializer_list<std::string> il) {
    printf("Error Message length is: %zu\n", il.size());
    for (const auto &str : il) {
        printf("->%s\n", str.c_str());
    }
}

void valClass::
error_msg(ErrCode e, std::initializer_list<std::string> il) {
    printf("Error Code is: %d, Error message length is: %zu\n", e, il.size());
    for (const auto &str : il) {
        printf("->%s\n", str.c_str());
    }
}

const std::vector<std::string> valClass::
process_list_return_1() {
    std::string str1("Nice");
    std::string str2("to");
    std::string str3("meet");
    std::string str4("you");
    return {str1, str2, str3, str4};
    // return {"Nice", "to", "meet", "you"};
}

const std::vector<int> valClass::
process_list_return_2() {
    int a = 23;
    int b = 24;
    int c = 25;
    return {a, b, c};
}

const std::vector<int> valClass::
process_list_return_empty() {
    return {};
}

// 'final' means this class can't be inheritted anymore
class newValClass final : public valClass {
public:
    // Inherit constructors(non-default and others) of base class
    // This will tell the compiler to generate codes like this,
    // newValClass(<params list>) : valClass(<param arg list>) { }
    // for example
    // newValClass(int a) : valClass(a) { }
    // newValClass(double b) : valClass(b) { }
    // newValClass(int a, double b) : valClass(a, b) { }
    // Be careful, this won't change the access level of the ctors
    // i.e., if a ctor is private in base, then after generated in child class
    // it is still a private class, even 'using' is in public domain in child class
    // Notice, this still can not inherit default-ctor, copy ctor & move ctor
    using valClass::valClass;

public:
    // 'override' means it overrides the print() in parent
    void print() const override {
        print_id();
        printf("------This is class newValClass calling print() in base class------START\n");
        valClass::print();
        printf("------This is class newValClass calling print() in base class------END\n");
    }

    // 'final' here means 'print_id' can't be overridden in a inheritted class
    void print_id() const final {
        printf("The ID of this newValClass is: %d\n", m_cid);
    }

public:
    int m_cid = 1;
    int m_child_value = 3;

};

class Debug {
    // constant constructors
    // it must initialize all its data members
public:
    constexpr Debug(bool b = true): hw(b), io(b), other(b) { }
    constexpr Debug(bool b, bool i, bool o): hw(b), io(i), other(o) { }

public:
    constexpr bool any() { return hw || io || other; }

private:
    bool hw;
    bool io;
    bool other;
}; /* class Debug*/

class divide {
public:
    int operator() (int denominator, int divisor) {
        return denominator / divisor;
    }
}; /* class divide */

int add(int i, int j) {
    return i + j;
}

void test_std_regex() {
    // std::regex
    printf("\n");
    std::regex pattern("\\bRETICLE_DIM\\s*(\\(.*\\))?([^()]*)");
    std::vector<std::string> svec = {"RETICLE_DIM", "RETICLE_DIM(mm", "RETICLE_DIM(mm)", "stxRRETICLE_DIM ( mm )", "RETICLE_DIM (    mm)", "RETICLE_DIM ( mmm)", "RETICLE_DIM  mm"};
    for (const auto &str : svec) {
        std::smatch mres;
        std::cout << str << " is matched: ";
        // if (std::regex_search(str, mres, pattern)) {
        if (std::regex_match(str, mres, pattern)) {
            std::cout << "YES" << " Matched substrings: " << "\n";
            int i = 0;
            for (const auto &s : mres)  {
                std::cout << "  [" << i++ << "] "<< s << ", Size: " << s.length() << "\n";
            }
            std::string str1(boost::algorithm::trim_copy(mres.str(1)));
            if (!str1.empty() && str1.front() == '(' && str1.back() == ')') {
                str1.erase(0, 1);
                str1.erase(str1.size() - 1, 1);
                boost::algorithm::trim(str1);
                if (str1 == "mm") {
                    std::cout << "Unit is mm";
                } else if (str1 == "um") {
                    std::cout << "Unit is um";
                } else {
                    std::cout << "Unit is not specified";
                }
                std::cout << "\n";
            }
            std::string str2(boost::algorithm::trim_copy(mres.str(2)));
            std::cout << "\n";
        } else {
            std::cout << "NO" << "\n";
        }
        std::cout << "\n";
    }
    std::regex LLXpattern("D?LLX(\\(.*\\))?");
    std::vector<std::string> vecllx = {"LLX", "DLLX", "LLX(um)", "DLLX(mm)"};
    for (const auto &str : vecllx) {
        std::smatch mres;
        std::cout << str << " is matched: ";
        if (std::regex_search(str, mres, LLXpattern)) {
            std::cout << "YES" << " Matched substrings: " << "\n";
            int i = 0;
            std::string ss(mres[1].str());
            std::cout << "MATCHED: " << ss << std::endl;
            for (const auto &s : mres)  {
                std::cout << "  [" << i++ << "] "<< s << ", Size: " << s.length() << "\n";
            }
        }
    }

    printf("\nTesting match for DEVICE|TOPCELL\n");
    std::regex multipattern("(\\bDEVICE\\b|\\bTOPCELL\\b)");
    std::vector<std::string> pstrings = {"DEVICE", "TOPCELL", "NDEVICE", "NTOPCELL", "DEVICEL", "TOPCELLD", "D DEVICE", "TOPCELL X"};
    for (const auto &str : pstrings) {
        std::smatch mres;
        std::cout << str << " is matched: ";
        if (std::regex_search(str, mres, multipattern)) {
            std::cout << "YES" << " Matched substrings: " << "\n";
            int i = 0;
            std::string ss(mres[1].str());
            std::cout << "MATCHED: " << ss << std::endl;
            for (const auto &s : mres)  {
                std::cout << "  [" << i++ << "] "<< s << ", Size: " << s.length() << "\n";
            }
        } else {
            std::cout << "NO!" << "\n";
        }
    }
} /* test_std_regex */

void check_size(const std::string &s, std::string::size_type sz) {
    if (s.size() >= sz) {
        std::cout << "True: The size of string: " << s << ", is greater than " << sz << "\n";
    } else {
        std::cout << "False: The size of string: " << s << ", is less than " << sz << "\n";
    }
}

void fargstest(int a, int b, int c, int d, int e) {
    std::cout << "The first argument is: " << a << "\n";
    std::cout << "The second argument is: " << b << "\n";
    std::cout << "The third argument is: " << c << "\n";
    std::cout << "The fourth argument is: " << d << "\n";
    std::cout << "The fifth argument is: " << e << "\n";
}

std::shared_ptr<std::string> getFruit(int i) {
    static std::vector<std::string> fs{"Apple", "Orange", "Grape", "Watermelon", "Pear", "Litchi", "KiwiFruit", "Strawberry", "Banana"};
    if (i >= 0 && i <= (int)fs.size()) {
        return std::make_shared<std::string>(fs[i]);
    } else {
        return std::make_shared<std::string>(fs[0]);
    }
}

// The main function to test a lot of features of C++11 library
void test_all() {

    // deduced type of string::size
    {
    	printf("\n");
        std::string str("GoodDayToday");
        auto len = str.size();
        std::cout << "Length of str is " << len << std::endl;
    }

    // new for-loop
    {
    	printf("\n");
    	std::string str("Good Day Today! Isn't it?");
    	decltype(str.size()) pcnt{0};
    	for (auto c : str) {
    		if (ispunct(c)) {
        		std::cout << "->(IsPunct) " << c << std::endl;
        		pcnt++;
    		} else {
        		std::cout << "-> " << c << std::endl;
    		}
    	}
    	std::cout << "Total punct: " << pcnt << std::endl;

    	for (auto &c : str) {
    		c = toupper(c);
    	}
    	std::cout << "String after change: " << str << std::endl;

    	for (const auto &c : str) {
    		std::cout << "!-> " << c << std::endl;
    	}
    	std::cout << "String after change: " << str << std::endl;

    	// 2D array
    	int ia[3][4] = {
    			{0, 1, 2, 3},
				{4, 5, 6, 7},
				{8, 9, 10, 11}
    	};
    	// Use auto
    	for (auto p = ia; p != ia + 3; p++) {
    		for (auto q = *p; q != *p + 4; q++) {
    			std::cout << *q << " ";
    		}
    		std::cout << std::endl;
    	}
    	// Use begin, end
    	for (auto p = std::begin(ia); p != std::end(ia); p++) {
    		for (auto q = std::begin(*p); q != std::end(*p); q++) {
    			std::cout << *q << " ";
    		}
    		std::cout << std::endl;
    	}

    }

    // new vector initialization
    {
    	printf("\n");

    	std::vector<std::string> svec1{"good", "day", "today"};
    	std::vector<std::string> svec2 = {"good", "day", "today"};

    	//assignment
    	svec1 = {"To", "Meet", "You", "Again"};
    }

    // cbegin(), cend()
    {
    	printf("\n");
    	std::vector<std::string> svec1{"good", "day", "today"};
    	std::vector<std::string>::const_iterator i{svec1.cbegin()};
    	std::vector<std::string>::const_iterator j(svec1.cend());
    }

    // std::begin(), std::end()
    {
    	printf("\n");
    	int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    	int *pstart = std::begin(ia);
    	int *pend = std::end(ia);
    	std::ptrdiff_t d1 = pstart - pend;

    	std::vector<std::string> svec1{"good", "day", "today"};
    	auto ps = std::begin(svec1);
    	auto pe = std::end(svec1);
    	auto d2 = ps - pe;

    	std::vector<std::string> svec2{"good", "day", "today"};
    	decltype(svec2.begin()) s = std::begin(svec2);
    	decltype(svec2.end()) e = std::end(svec2);
    	auto d3 = s - e;
    }

    // sizeof operator
    {
        printf("\n");
        tester_initialization_in_class a(1, 9.9);
        std::size_t sz = sizeof tester_initialization_in_class::m_global;
        auto sz1 = sizeof tester_initialization_in_class::m_global;
        decltype(tester_initialization_in_class::m_global) sz2 = sizeof tester_initialization_in_class::m_global;
        printf("Size of tester_initialization_in_class's member m_global: %llu\n", sz);
        printf("Size of tester_initialization_in_class's member m_global: %llu\n", sz1);
        printf("Size of tester_initialization_in_class's member m_global: %d\n", sz2);
    }


    // for range loop
    {
        printf("\n");
        for (auto i : {2, 4, 6, 8, 10}) {
            printf("%d ", i);
        }
        printf("\n");

    	std::vector<std::string> svec{"good", "day", "today"};
        for (auto s : svec) {
            printf("%s ", s.c_str());
        }
        printf("\n");

    	std::vector<int> ivec{1, 3, 5, 7, 9};
        for (auto &i : ivec) {
            i *= 10;
        }

        for (auto i : ivec) {
            printf("%d ", i);
        }
        printf("\n");
    }

    // std::initializer_list<T>
    {
        printf("\n");
        valClass obj;
        obj.error_msg({"FunctionX", "ok", "BAD"});

        std::initializer_list<std::string> slist{"good", "day", "today"};
        obj.error_msg(slist);

        std::initializer_list<std::string> slist2 = {"GOOD", "DAY", "TODAY"};
        obj.error_msg(slist2);

        // Another overloaded function
        obj.error_msg(valClass::TC, slist);
        obj.error_msg(valClass::ErrCode(1), slist);
    }

    // Return a list in function
    {
        printf("\n");
        valClass obj;
        std::vector<std::string> ivec(obj.process_list_return_1());
        std::vector<int> svec(obj.process_list_return_2());
        printf("Size of ivec is: %llu, size of ivec2 is %llu\n", ivec.size(), svec.size());

        std::vector<int> ivec2(obj.process_list_return_2());
        ivec2 = obj.process_list_return_empty();
        printf("Size of ivec2 is: %llu\n", ivec2.size());
    }


    // constexpr function
    {
        printf("\n");
        // valClass obj;
        constexpr int foo = valClass::getLiteralVal();
        constexpr int foo2 = getLiteralVal();
        printf("A constant value returned from a constant member function is: %d\n", foo);
        printf("A constant value returned from a constant global function is: %d\n", foo2);
    }

    // Constant ctors
    {
        printf("\n");
        Debug X(false);
        printf("If any ? %s\n", X.any() ? "YES" : "NO");
    }

    // Using std::string to open file is allowed in C++11
    {
#if 0 // Disable it since file operations, can enable it again when needed
        std::string fname("test.txt");
        std::fstream f(fname, std::fstream::out);
        if (f.is_open()) {
            std::cout << "File is opened" << std::endl;
            f << "Good day today\n";
        } else {
            std::cout << "File is NOT opened" << std::endl;
            f << "Good day today\n";
        }
        f.close();
#endif
    }

    // std::forwad_list & std::array
    {
        printf("\n");
        // std::forwad_list
        // IMPORTANT NOTICE: forward_list has no 'size()' function
        std::forward_list<int> first;                      // default: empty
        std::forward_list<int> second (3,77);              // fill: 3 seventy-sevens
        std::forward_list<int> third (second.begin(), second.end()); // range initialization
        std::forward_list<int> fourth (third);            // copy constructor
        std::forward_list<int> fifth (std::move(fourth));  // move ctor. (fourth wasted)
        std::forward_list<int> sixth = {3, 52, 25, 90};    // initializer_list constructor

        std::cout << "first:" ; for (int& x: first)  std::cout << " " << x; std::cout << '\n';
        std::cout << "second:"; for (int& x: second) std::cout << " " << x; std::cout << '\n';
        std::cout << "third:";  for (int& x: third)  std::cout << " " << x; std::cout << '\n';
        std::cout << "fourth:"; for (int& x: fourth) std::cout << " " << x; std::cout << '\n';
        std::cout << "fifth:";  for (int& x: fifth)  std::cout << " " << x; std::cout << '\n';
        std::cout << "sixth:";  for (int& x: sixth)  std::cout << " " << x; std::cout << '\n';

        // std::array
        std::array<int, 5> arr = {2, 167, 39, 77, 50};
        std::array<int, 5> otherArr;
        std::array<int, 5> otherArr2 = {10}; // First element is 10, others are all '0's.
        std::cout << "arr contains:";
        for ( auto it = arr.begin(); it != arr.end(); ++it ) {
            std::cout << ' ' << *it;
        }
        std::cout << '\n';
        std::cout << "First element of arr is:" << arr[0] << std::endl;

        otherArr = arr;
        for (const auto &val : otherArr) {
            std::cout << ' ' << val;
        }
        std::cout << '\n';
    }

    // cbegin() & cend() & crbegin() & crend()
    {
        printf("\n");
        std::vector<int> ivec = {1, 2, 3, 4, 5, 6, 7};

        for (auto i = ivec.cbegin(); i != ivec.cend(); i++) {
            std::cout << *i << " ";
        }
        std::cout << "\n";

        for (auto i = ivec.crbegin(); i != ivec.crend(); i++) {
            std::cout << *i << " ";
        }
        std::cout << "\n";

    }


    // Universal swap : std::swap
    {
        printf("\n");
        std::vector<int> ivec1 = {1, 2, 3, 4, 5, 6, 7};
        std::vector<int> ivec2 = {10, 20, 30};
        std::swap(ivec1, ivec2);
        for (const auto &v : ivec1) {
            std::cout << v << " ";
        }
        std::cout << "\n";

        for (const auto &v : ivec2) {
            std::cout << v << " ";
        }
        std::cout << "\n";

    }

    // emplace, emplace_front, emplace_back
    {
        std::vector<valClass> valvec;
        for (int i = 0; i < 5; i++) {
            // Use default ctor of valClass
            valvec.emplace_back();
        }
        std::vector<valClass>::iterator itr = valvec.begin() + 2;
        valvec.emplace(itr, 300); // use ctor of valClass which accepts an integer
        valvec.emplace_back(300, 1.36); // use ctor of valClass which accepts an integer and a double
    }


    // String converters
    {
        printf("\n");
        int i = 42;
        std::string str = std::to_string(i);
        int a = std::stoi(str);
        long int b = std::stol(str);
        unsigned long int c = std::stoul(str);
        long long int d = std::stoll(str);
        unsigned long long int e = std::stoull(str);
        float f = std::stof(str);
        double g = std::stod(str);
        long double h = std::stold(str);
        std::cout << a << " " << b << " " << c << " " << d << " " << e << " " << f << " " << g << " " << h << std::endl;
    }

    // lambda in C++11
    {
        printf("\n");

        auto f = [] { return 42; };
        std::cout << f() << std::endl;

        auto x = [](const std::string &a, const std::string &b) { return a.size() < b.size() ? "YES" : "NO"; };
        std::cout << x("abc", "defg") << std::endl;

        auto sz = 9;
        auto y = [sz](const std::string &a) { return a.size() < sz ? "YES" : "NO"; };
        std::cout << y("abc") << std::endl;

        // Multiple captures
        auto m = 100;
        auto y2 = [sz, m] () { return sz > m; };
        if (y2()) {
            std::cout << "sz is greater than m" << "\n";
        } else {
            std::cout << "sz is less than m" << "\n";
        }

        auto value = 42;
        auto lobj = [value] { return value; };
        value = 0;
        // lambda object will copy the data of all arguments in captured list when created if not using reference
        std::cout << "Value returned is: " << lobj() << std::endl;

        // lambda object will NOT copy the data of all arguments in captured list when created if using reference
        value = 99;
        auto lobj2 = [&value] { return value; };
        std::cout << "Value returned(2) is: " << lobj2() << std::endl;
        value = -32;
        std::cout << "Value returned(3) is: " << lobj2() << std::endl;


        std::vector<int> ivec = {1, 2, 3, 4, 5, 6, 7};
        std::for_each(ivec.begin(), ivec.end(), [](const int v){ std::cout << v << " "; });
        std::cout << "\n";
        // actually in C++14, we can use 'auto' in parameter list of lambda
        std::for_each(ivec.begin(), ivec.end(), [](auto v){ std::cout << v << " "; });
        std::cout << "\n";

        // Implicit Captures
        // (1) All are references
        int a1 = 9;
        int a2 = 19;
        auto yy = [&] { return a1 > a2; };
        std::cout << "yy is " << (yy() ? "True" : "False") << "\n";
        a1 = 19;
        a2 = 9;
        std::cout << "yy is " << (yy() ? "True" : "False") << "\n";

        // (2) All are copies
        a1 = 10;
        a2 = 20;
        auto yyy = [=] { return a1 > a2; };
        std::cout << "yyy is " << (yyy() ? "True" : "False") << "\n";
        a1 = 30;
        a2 = 5;
        std::cout << "yyy is " << (yyy() ? "True" : "False") << "\n";

        // (3) Mixed, but default is reference
        int a3 = 0;
        // if use '&a3' in the capture list
        // then it WILL BE AN ERROR
        auto z = [&, a3] { return a2 < a3; };
        std::cout << "z is " << (z() ? "True" : "False") << "\n";

        // if use 'a3' instead of '&a3'
        // then it WILL BE AN ERROR
        auto zz = [=, &a3] { return a1 > a3; };
        std::cout << "zz is " << (zz() ? "True" : "False") << "\n";

        // Change captured ones
        a3 = 23;
        auto zzz = [a3] () mutable { return ++a3; };
        std::cout << "zzz is " << zzz() << "\n";

        // If multiple statements
        // then void is returned
        // unless a return type is specified
        auto xxx = [](int x) -> int { if (x > 0) return x; else return -x;};
        std::cout << "The absolute value of -9 is: " << xxx(-9) << "\n";
    }

    // std::bind
    {
        printf("\n");
        auto check_size_with_int_3 = std::bind(check_size, std::placeholders::_1, 3);
        std::string ss("Rainbow");
        check_size_with_int_3(ss);

        printf("\n");
        int px = 99;
        int py = 37;
        int pz = 11;
        auto f = std::bind(fargstest, px, py, std::placeholders::_2, pz, std::placeholders::_1);
        f(2381, -1539);

        int tx = 3409;
        // pass a reference
        printf("\n");
        auto fx = std::bind(fargstest, px, py, std::ref(tx), pz, std::placeholders::_1);
        fx(82);

        // pass a constant reference
        printf("\n");
        auto fy = std::bind(fargstest, px, py, std::cref(tx), pz, std::placeholders::_1);
        fy(13);
    }

    // Initialization of Associative Containers
    {
        std::set<std::string> sset = {"apple", "pear", "grape", "orange"};
        std::map<std::string, std::string> mmap = {{"Joyce", "Apple"}, {"James", "Pear"}, {"Austen", "Grape"}, {"Dickens", "Orange"}};

        printf("\n");
        for (const auto &s : sset) {
            std::cout << s << "\n";
        }

        printf("\n");
        for (const auto &vp : mmap) {
            std::cout << vp.first << " : " << vp.second << "\n";
        }
    }

    // Return a pair using pair initialization
    {
        printf("\n");
        auto f = [] () -> std::pair<std::string, std::string> { return {std::string("Rainbow"), std::string("Chaser")}; };
        auto p = f();
        std::cout << p.first << ", " << p.second << "\n";
    }

    // std::pair initialization
    {
        printf("\n");
        std::pair<std::string, int> apair{"RainbowChaser", 9};
        std::cout << "First: " << apair.first << ", Second: " << apair.second << "\n";
    }

    // Unordered containers
    {
        printf("\n");
        std::unordered_map<std::string, int> collection;
        collection.emplace("Apple", 3);
        collection.insert({"Pear", 2});
        collection.insert({{"Grape", 5}, {"Orange", 7}});
        collection.insert({"Watermelon", 1});
        for (const auto &vp : collection) {
            std::cout << "Fruit: " << vp.first << ", Total: " << vp.second << "\n";
        }

        std::cout << "    bucket_count: " << collection.bucket_count() << "\n";
        std::cout << "max_bucket_count: " << collection.max_bucket_count() << "\n";
        std::cout << "     load_factor: " << collection.load_factor() << "\n";


        printf("\n");
        std::unordered_map<valClass, int, hashfunc_valClass, equal_valClass> clt;
        valClass p1(3);
        valClass p2(2);
        clt.emplace(p1, 9);
        clt.emplace(p2, 70);
        std::cout << "Total size of clt is: " << clt.size() << "\n";
        for (const auto &v : clt) {
            std::cout << "Value: " << v.second << "\n";
            v.first.print();
        }
    }

    // std::shared_ptr, std::unique_ptr and std::weak_ptr
    // std::make_shared
    {
        printf("\n");
        std::shared_ptr<std::string> p1(new std::string(10, '9'));
        std::cout << "p1 string: " << *p1 << "\n";

        std::shared_ptr<std::string> p2 = std::make_shared<std::string>(10, '9');
        std::cout << "p2 string: " << *p2 << "\n";

        std::shared_ptr<std::string> p3 = std::make_shared<std::string>();
        std::cout << "p3 string: " << *p3 << "\n";

        std::shared_ptr<std::string> p4(getFruit(3));
        std::cout << "Get fruit(shared_ptr p4): " << *p4 << "\n";

        std::weak_ptr<std::string> p5(p4);
        if (auto pp = p5.lock()) {
            std::cout << "Get fruit(weak_ptr p5): " << *pp << "\n";
        } else {
            std::cout << "No fruit avalible(weak_ptr p5)" << "\n";
        }

        std::unique_ptr<std::string> p6(new std::string(2, 'A'));
        std::cout << "p6 string: " << *p6 << "\n";
    }

    // Use a list to dynamically allocate an array
    {
        printf("\n");
        printf("Int array allocated dynamically:\n");
        int *ip = new int[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (std::size_t i = 0; i < 10; i++) {
            std::cout << "(" << i << ")" << ip[i] << " ";
        }
        delete [] ip;
        std::cout << "\n";

        printf("String array allocated dynamically:\n");
        std::string *sp = new std::string[10]{"a", "an", "the", std::string(3, 'x')};
        for (std::size_t i = 0; i < 10; i++) {
            std::cout << "(" << i << ")" << sp[i] << " ";
        }
        std::cout << "\n";
        delete [] sp;

        // up is a pointer to an array of dynamically allocated
        std::unique_ptr<int[]> up(new int[10]);
        for (std::size_t i = 0; i < 10; i++) {
            up[i] = i + 10;
        }
        for (std::size_t i = 0; i < 10; i++) {
            std::printf("(%llu) %d ", i, up[i]);
        }
        std::printf("\n");
        // release will auto destroy the array allocated dynamically
        up.release();
    }

    // allocator<T>

    // rvalue reference
    {
        printf("\n");

        int i = 42;
        // Pass
        int &r = i;
        // Error, can't bind a lvalue to a rvalue-reference
        //   int &&rr = i;
        // Error, can'b bind a rvalue to a lvalue-reference
        //   int &r2 = i * 42;
        // Pass, const T& is versatile
        const int &r3 = i * 42;
        // Pass
        int &&rr2 = i * 42;
    }

    // rvalue reference - additional
    {
        printf("\n");

        int i = 42;

     // int &r = 6;                // Error, can't bind non-const reference to rvalue '6'
        int &&rr = 6;              // Pass
        const int &cr = 6;         // Pass
        const int &&crr = 6;       // Pass

        int &ir = i;               // Pass
     // int &&irr = i;             // Error, 'i' is a lvalue, can't bind lvalue to rvalue-reference
        const int &cir = i;        // Pass
     // const int &&cirr = i;      // Error, 'i' is a lvalue, can't bind lvalue to rvalue-reference

     // int &xir = i * 6;          // Error, 'i * 6' is a rvalue, can't bind a lvalue to a rvalue
        int &&xirr = i * 6;        // Pass
        const int &cxir = i * 6;   // Pass
        const int &&cxirr = i * 6; // Pass

        int &yr = ir;              // Pass
     // int &&yrr = ir;            // Error, 'ir' is a lvalue too
        const int &ycr = ir;       // Pass
     // const int &&ycrr = ir;     // Error, 'ir' is a lvalue too

        int &zr = rr;              // Pass
     // int &&zrr = rr;            // Error, 'ir' is a lvalue too
        const int &zcr = rr;       // Pass
     // const int &&zcrr = rr;     // Error, 'ir' is a lvalue too

     // int &pr = cr;              // Error, 'cr' is constant
     // int &&prr = cr;            // Error, 'cr' is a lvalue too
        const int &pcr = cr;       // Pass
     // const int &&pcrr = cr;     // Error, 'cr' is a lvalue too

     // int &qr = crr;             // Error, 'crr' is constant
     // int &&qrr = crr;           // Error, 'crr' is a lvalue too
        const int &qcr = crr;      // Pass
     // const int &&qcrr = crr;    // Error, 'crr' is a lvalue too

        printf("'i' is: %d\n", i);
        printf("'rr' is: %d\n", rr);
        printf("'cr' is: %d\n", cr);
        printf("'crr' is: %d\n", crr);
        printf("\n");

        printf("'ir' is: %d\n", ir);
        printf("'cir' is: %d\n", cir);
        printf("\n");

        printf("'xirr' is: %d\n", xirr);
        printf("'cxir' is: %d\n", cxir);
        printf("'cxirr' is: %d\n", cxirr);
        printf("\n");

        printf("'yr' is: %d\n", yr);
        printf("'ycr' is: %d\n", ycr);
        printf("\n");

        printf("'zr' is: %d\n", zr);
        printf("'zcr' is: %d\n", zcr);
        printf("\n");

        printf("'pcr' is: %d\n", pcr);
        printf("\n");

        printf("'qcr' is: %d\n", pcr);
        printf("\n");
    }

    // function
    {
        printf("\n");

        // A function pointer
        std::function<int(int, int)> f1 = add;
        // A temporary object
        std::function<int(int, int)> f2 = divide();
        // An object
        divide myDiv;
        std::function<int(int, int)> f3 = myDiv;
        // A lambda expression
        std::function<int(int, int)> f4 = [](int i, int j) { return i * j; };

        printf("f1(4, 2) return: %d\n", f1(4, 2));
        printf("f2(4, 2) return: %d\n", f2(4, 2));
        printf("f3(4, 2) return: %d\n", f3(4, 2));
        printf("f4(4, 2) return: %d\n", f4(4, 2));


        // function objects can be collected
        printf("\n");
        std::map<std::string, std::function<int(int, int)>> binops;
        binops.emplace("+", add);
        binops.emplace("-", std::minus<int>());
        binops.emplace("/", divide());
        binops.emplace("*", [](int i, int j) { return i * j; });

        // loop
        for (auto &f : binops) {
            printf(" '%s' (4, 2) returns: %d\n", f.first.c_str(), f.second(4, 2));
        }

        // Single calls
        printf("\n");
        std::cout << "f1(10, 5) returns: " << binops["+"](10, 5) << "\n";
        std::cout << "f2(10, 5) returns: " << binops["-"](10, 5) << "\n";
        std::cout << "f3(10, 5) returns: " << binops["/"](10, 5) << "\n";
        std::cout << "f4(10, 5) returns: " << binops["*"](10, 5) << "\n";

        // Initialization list
        printf("\n");
        std::map<std::string, std::function<int(int, int)>> binops2 = { {"+", f1}, {"-", std::minus<int>()}, {"/", divide()}, {"*", f4} };
        // loop
        for (auto &f : binops2) {
            printf(" '%s' (4, 2) returns: %d\n", f.first.c_str(), f.second(4, 2));
        }


    }

    // use 'using' to inherit ctors in parent class
    // this will tell compiler to generate them other than written by hand
    {
        printf("\n");
        newValClass obj(7778, -1839.26);
        obj.print();
    }

    // tuple
    {
        printf("\n");
        std::tuple<std::string, std::vector<double>, std::size_t, int, std::list<int>> 
            someValues("GoodDayToday", {3.14, 2.718}, 23, 42, {1, 3, 5, 7, 9});
        auto item = std::make_tuple("0-999-78345-X", 24, 20.00);
        auto book = std::get<0>(item);
        auto str = std::get<0>(someValues);
        printf("First element in item is: %s\n", book);
        printf("First element in someValues is: %s\n", str.c_str());

        // Check how many elements in a tuple
        // Actually here ITEMTYPE is std::tuple<std::string, int, double>
        // Here just to save time
        typedef decltype(item) ITEMTYPE;
        std::size_t sz = std::tuple_size<ITEMTYPE>::value;
        printf("The size of tuple item is: %llu\n", sz);
        // Represent the type of the second member in tuple item
        // Here cnt is int, which is the same type as the second member in item
        std::tuple_element<1, ITEMTYPE>::type cnt = std::get<1>(item);
        printf("The second value of tuple item is: %d\n", cnt);

    }

    // Regular expression: regex
    {
        printf("\n");
        // use cmatch instead of smatch to match the input sequence type
        std::regex r("[[:alnum:]]+\\.(cpp|cxx|cc)$", std::regex::icase);
        std::cmatch mres1;
        if (std::regex_search("myfile.cc", mres1, r)) {
            std::cout << "Matched string: " << mres1.str() << "\n";
        } else {
            std::cout << "Not match\n";
        }

        // use smatch since input sequence is string type
        printf("\n");
        std::string str("myfile.cpp");
        std::smatch mres2;
        if (std::regex_search(str, mres2, r)) {
            std::cout << "Matched string: " << mres2.str() << "\n";
        } else {
            std::cout << "Not match\n";
        }

        // sregex_iterator
        printf("\n");
        std::string file("receipt friend theif receive acheive");
        std::string pattern("[^c]ei");
        pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
        std::regex r2(pattern, std::regex::icase);
        // sregex_iterator uses pattern(here is r2) to call regex_search
        // and automatically locates in the first match
        std::sregex_iterator sitr(file.begin(), file.end(), r2);
        std::sregex_iterator end_itr; // iterator after last one, boundary indicator
        printf("Matched strings are:\n");
        for (; sitr != end_itr; ++sitr) {
            auto pos = sitr->prefix().length();
            pos = pos > 40 ? pos - 40 : 0;
            std::cout << sitr->prefix().str().substr(pos)
                      << "\n\t\t>>>" << sitr->str() << " <<<\n"
                      << sitr->suffix().str().substr(0, 40) << "\n";
        }

    }

    // random
    {
        // Generate 10 random unsigned int
        printf("\n");
        // e is a random number engine, which is a callable function object
        // Can give a seed to the engine to generate different sequence -> std::default_random_engine e(SEED)
        std::default_random_engine e;

        // std::default_random_engine e(time());
        for (std::size_t i = 0; i < 10; i++) {
            std::cout << e() << " ";
        }
        std::cout << "\n";

        // Generate 10 random unsigned int in range [0, 9]
        // u is a callable object
        std::uniform_int_distribution<unsigned> u(0, 9);
        // std::uniform_int_distribution<> u(0, 9); --> will generate integer type by default
        for (std::size_t i = 0; i < 10; i++) {
            std::cout << u(e) << " ";
        }
        std::cout << "\n";

        // Generate random real number
        std::uniform_real_distribution<double> d(0, 1);
        // std::uniform_real_distribution<> d(0, 1); --> will generate double type by default
        for (std::size_t i = 0; i < 10; i++) {
            std::cout << d(e) << " ";
        }
        std::cout << "\n";

        // Generate numbers obey normal distribution
        // generate 200 numbers centered around a mean of 4 with a standard deviation of 1.5.
        std::normal_distribution<> n(4, 1.5); // mean value is 4, standard deviation is 1.5
        std::vector<unsigned> vals(9);
        for (std::size_t i = 0; i < 200; i++) {
            unsigned v = lround(n(e));
            if (v < vals.size()) {
                ++vals[v];
            }
        }
        // print a map of distribution
        for (std::size_t j = 0; j != vals.size(); ++j) {
            std::cout << j << ": " << std::string(vals[j], '*') << "\n";
        }
    }


    // Time in C++11, chrono library
    {
        printf("\n\n");

        // Precision of periods in 3 kind of clocks
        // (1) system_clock
        // (2) steady_clock
        // (3) high_resolution_clock

        // Numerator / Denominator
        std::cout << "system_clock precision        : "
                  << std::chrono::system_clock::period::num << "/" << std::chrono::system_clock::period::den << " s" << std::endl;
        std::cout << "steady_clock precision        : "
                  << std::chrono::steady_clock::period::num << "/" << std::chrono::steady_clock::period::den << " s" << std::endl;
        std::cout << "high_resultion_clock precision: "
                  << std::chrono::high_resolution_clock::period::num << "/" << std::chrono::high_resolution_clock::period::den << " s" << std::endl;


        std::chrono::duration<int> oneHourDuration(60*60);

        // Use system clock to calculate elapsed time
        typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> nanoClockType;
        typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> microClockType;
        typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> milliClockType;
        typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> secClockType;
        typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes> minClockType;
        typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::hours> hClockType;

        nanoClockType tp0(oneHourDuration);
        std::cout << "One hour since system_clock epoch = " << tp0.time_since_epoch().count() << " ns" << std::endl;

        microClockType tp1(oneHourDuration);
        std::cout << "One hour since system_clock epoch = " << tp1.time_since_epoch().count() << " us" << std::endl;

        milliClockType tp2(oneHourDuration);
        std::cout << "One hour since system_clock epoch = " << tp2.time_since_epoch().count() << " ms" << std::endl;

        secClockType tp3(oneHourDuration);
        std::cout << "One hour since system_clock epoch = " << tp3.time_since_epoch().count() << " s" << std::endl;

        // Notice 'std::ratio<60>' is 'std::ratio<60, 1>', which means (60 / 1) seconds
        typedef std::chrono::duration<int, std::ratio<60>> minDurType;
        minClockType tp4(std::chrono::duration_cast<minDurType>(oneHourDuration));
        std::cout << "One hour since system_clock epoch = " << tp4.time_since_epoch().count() << " min" << std::endl;

        // Notice 'std::ratio<60*60>' == 'std::ratio<60*60, 1>,' which means (60*60 / 1) seconds
        typedef std::chrono::duration<int, std::ratio<60*60>> hDurType;
        hClockType tp5(std::chrono::duration_cast<hDurType>(oneHourDuration));
        std::cout << "One hour since system_clock epoch = " << tp5.time_since_epoch().count() << " h" << std::endl;

#if 0
        // ---------------------
        // !!! NOTICE !!!
        // ---------------------
        // Get time duration between 2 checkpoint and convert to microseconds
        // This code block will cause system to sleep for 1 second
        // Uncomment this block to test if needed

        time_t tt;
        std::chrono::system_clock::time_point checkPoint1(std::chrono::system_clock::now());
        tt = std::chrono::system_clock::to_time_t(checkPoint1);
        std::cout << "Current time is: " << ctime(&tt) << std::endl;
        sleep(1); // --> sleep for 1 second
        // for (int i = 0; i < 10000; i++) { int k = i + 9;}
        std::chrono::system_clock::time_point checkPoint2(std::chrono::system_clock::now());
        tt = std::chrono::system_clock::to_time_t(checkPoint2);
        std::cout << "Current time is: " << ctime(&tt) << std::endl;
        // Time elapsed
        std::chrono::system_clock::duration dur(checkPoint2 - checkPoint1);
        std::chrono::microseconds durInMicro = std::chrono::duration_cast<std::chrono::microseconds>(dur); // Convert to microseconds
        std::cout << "Elapsed time between 2 check points: " << durInMicro.count() << " us" << std::endl;
        std::chrono::milliseconds durInMilli = std::chrono::duration_cast<std::chrono::milliseconds>(dur); // Convert to milliseconds
        std::cout << "Elapsed time between 2 check points: " << durInMilli.count() << " ms" << std::endl;
        std::chrono::seconds durInSec = std::chrono::duration_cast<std::chrono::seconds>(dur); // Convert to seconds
        std::cout << "Elapsed time between 2 check points: " << durInSec.count() << " s" << std::endl;
#endif // 0

    }

    printf("\n");
} // function test_all()


} // namespace mainspace


#endif // _MAIN_HPP





























