#ifndef _NORMAL_HPP
#define _NORMAL_HPP
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <math.h>
#include <cstdarg>
// #include <windows.h>


#include <functional>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/optional.hpp>
#include <boost/functional/hash.hpp>
#include <boost/shared_ptr.hpp>


namespace NORMAL {

namespace smart_pointer_test {

class CircularTestB;
class CircularTestA {
public:
    CircularTestA() { std::cout << "Constructing CircularTestA\n"; }
    ~CircularTestA() { std::cout << "Destructing CircularTestA\n"; }
    void set_ptr(const std::shared_ptr<CircularTestB> &p) { m_ptr = p; }

private:
    std::weak_ptr<CircularTestB> m_ptr;
    // std::shared_ptr<CircularTestB> m_ptr;
}; // class CircularTestA

class CircularTestB {
public:
    CircularTestB() { std::cout << "Constructing CircularTestB\n"; }
    ~CircularTestB() { std::cout << "Destructing CircularTestB\n"; }
    void set_ptr(const std::shared_ptr<CircularTestA> &p) { m_ptr = p; }

private:
    std::shared_ptr<CircularTestA> m_ptr;
}; // class CircularTestB

} // namespace smart_pointer_test

void tester_constexpr();


using path_query_result_type = std::tuple<int, int, int, int>;
using size_type = std::size_t;

void separate_segments(const std::vector<path_query_result_type> &arr,
                        std::pair<size_type, size_type> range,
                        double gap);


enum ISK_TYPE {
    TYPE_0 = 0,
    TYPE_1,
    TYPE_2,
    TYPE_3,
    TYPE_UNKOWN,
    TYPE_TRIPLE = TYPE_0,
    TYPE_CONCAT = TYPE_1,
    TYPE_MERGE = TYPE_2,
    TYPE_TBONE = TYPE_3,
};


template <typename T>
class do_nothig_deleter{
public:
   void operator()(T* ptr) { }
};
#if 0
template <typename T>
class release_deleter{
public:
   using atomic_bool_type = bool;
public:
   release_deleter() : released_(new atomic_bool_type(false)){}
   void release() {
      // released_->set(true);
      released_ = true;
   }
   void operator()(T* ptr) {
      //if(!released_->get()) {
      if(!released_.get()) {
         delete ptr;
      }
   }
private:
   boost::shared_ptr<atomic_bool_type> released_;
};
#endif // 0


template <typename T>
class B
{
public:
    std::function<void(const T&)> func;
};


class A
{
public:
    void print() const // in A
    {
        std::cout << "Hallo A" << std::endl;
    }
};

typedef enum GLOBAL_KEYS { DEV = 0, FIL, LLX, LLY, URX, URY, SHK, DBU, ORT, ROT, CTX, CTY, LYT, DEL, HSK, BTS, PTS} GKEY;

// Function declarations
void normal_test_all();

// End of function declarations


class valClass {
public:
    valClass() : m_id(-1), m_value(23), m_value2(24) {}
    valClass(int m) : m_id(m), m_value(23), m_value2(24) {}
    valClass(int m, int n) : m_id(m), m_value(n), m_value2(24) {}
    valClass(int m, int n, int k) : m_id(m), m_value(n), m_value2(k) {}
    ~valClass() {
        printf("valClass (m_id = %d) is deconstructing...\n", m_id);
    }
public:
    void print() const {
        printf("Value of inside: %d\n", m_value);
        printf("Value of m_id: %d\n", m_id);
    }
public:
    typedef enum ENUM_KEYS { DEV = 0, FIL, LLX, LLY, URX, URY, SHK, DBU, ORT, ROT, CTX, CTY, LYT, DEL, HSK, BTS, PTS} EKEY;

public:
    struct hashval {
        std::size_t operator()(const ENUM_KEYS &e) const {
            return (int)e;
        }
    };
    struct hasheq {
        bool operator()(const ENUM_KEYS &e1, const ENUM_KEYS &e2) const {
            return (int)e1 == (int)e2;
        }
    };

    /**
     * Test for variable nubmer of arguments
     */
    void msg_print(const char *fmt, ...) {
        static std::shared_ptr<char> carr(new char[128 * 1024]);
        va_list args;
        va_start(args, fmt);
        vsnprintf(carr.get(), 128 * 1024, fmt, args);
        va_end(args);
        std::string s(carr.get());
        const int slen = s.size();
        std::cout << "Out string is "
                  << s
                  << "\nLength is "
                  << slen << std::endl;
    }

public:
    std::unordered_map<EKEY, int, hashval, hasheq> m_emap;

protected:
    int m_id;
private:
    int m_value;
public:
    int m_value2;
};


class data_storage {
public:
    int m_int;
    double m_double;
    bool m_bool;
    std::string m_name;
    std::shared_ptr<valClass> m_value;

    void print() {
        printf("m_name: %s\n", m_name.c_str());
        printf("=== Content of data_storage ===\n");
        printf("m_int: %d\n", m_int);
        printf("m_double: %.2f\n", m_double);
        printf("m_bool: %s\n", m_bool ? "True" : "False");
        if (m_value) {
            m_value->print();
        } else {
            printf("m_value is NULL\n");
        }
    }
};

struct Dstorage {
public:
    int m_int;
    double m_double;
    bool m_bool;
    std::string m_name;
    valClass *m_value;
    void print() {
        printf("m_name: %s\n", m_name.c_str());
        printf("=== Content of data_storage ===\n");
        printf("m_int: %d\n", m_int);
        printf("m_double: %.2f\n", m_double);
        printf("m_bool: %s\n", m_bool ? "True" : "False");
        if (m_value) {
            m_value->print();
        } else {
            printf("m_value is NULL\n");
        }
    }
};


class zoo {
public:
    zoo() : m_name("unkown zoo"), m_tiger(2), m_monkey(3) { }
    zoo(int t, int m) : m_name("unkown zoo"), m_tiger(t), m_monkey(m) { }
    zoo(const std::string &s, int t, int m) : m_name(s), m_tiger(t), m_monkey(m) { }
    ~zoo() { }

public:
    void print() const {
        printf("Zoo Name is: %s, Tiger: %d, Monkey: %d\n", m_name.c_str(), m_tiger, m_monkey);
    }

public:
    zoo& operator=(zoo &&rhs) noexcept {
        if (this != &rhs) {
            m_name = rhs.m_name;
            m_tiger = rhs.m_tiger;
            m_monkey = rhs.m_monkey;
        }
        return *this;
    }
    zoo(zoo &&rhs) {
        if (this != &rhs) {
            m_name = rhs.m_name;
            m_tiger = rhs.m_tiger;
            m_monkey = rhs.m_monkey;
        }
    }
    /*
    bool operator==(const zoo& rhs) const {
        return m_name == rhs.get_name() && m_tiger == rhs.get_tiger() && m_monkey == rhs.get_monkey();
    }
    bool operator==(const std::string& s) const {
        return m_name == s;
    }
    */
public:
    const int get_tiger() const { return m_tiger; }
    const int get_monkey() const { return m_monkey; }
    const std::string get_name() const { return m_name; }

public:
    static bool less_than(const zoo &z1, const zoo &z2) {
       return z1.m_tiger < z2.m_tiger;
    }

private:
    std::string m_name;
    int m_tiger;
    int m_monkey;
};

class newZoo {
public:
    newZoo() : m_tiger(2), m_monkey(3) { }
    newZoo(int t, int m) : m_tiger(t), m_monkey(m) { }
    newZoo(const zoo &z) : m_tiger(z.get_tiger()), m_monkey(z.get_monkey()) { }
    ~newZoo() { }

public:
    void print() const {
        printf("Tiger: %d, Monkey: %d\n", m_tiger, m_monkey);
    }
    void set(newZoo&& v) {
        m_tiger = v.get_tiger();
        m_monkey = v.get_monkey();
    }
    static newZoo convertFrom(const zoo &z) { return newZoo(z); }
public:
    void set_tiger(int t) { m_tiger = t; }
    void set_monkey(int m) { m_monkey = m; }
public:
    const int get_tiger() const { return m_tiger; }
    const int get_monkey() const { return m_monkey; }

private:
    int m_tiger;
    int m_monkey;
    std::unordered_map<std::string, std::list<int>> m_map;
};
struct newZoo_hashfunc {
    std::size_t operator() (const newZoo &z) const {
        return std::hash<int>()(z.get_tiger());
    }
};

struct newZoo_equal {
    bool operator() (const newZoo &lhs, const newZoo &rhs) const {
        return lhs.get_tiger() == rhs.get_tiger();
    }
};
struct newZoo_less {
    bool operator() (const newZoo &lhs, const newZoo &rhs) const {
        return lhs.get_tiger() < rhs.get_tiger();
    }
};

class Point {
public:
    static const int value_count = 2;
    typedef int (data_type)[value_count];

public:
    Point() { }
    Point(int x, int y) {
        m_value[0] = x;
        m_value[1] = y;
    }
    ~Point() { }

private:
    int m_value[2];
}; // End of class point


/**
 * A class to test for data reference
 *
 */
class dataRef {
public:
    static const unsigned int vcount = 10;
    typedef int (data_type)[vcount];
    typedef const int (const_data_type)[vcount];
    typedef data_type (&data_reference);
    typedef const_data_type (&const_data_reference);

public:
    dataRef() { fill(3); printf("[LONGC_TEST] Constructing object of dataRef (ctor1)\n"); }
    dataRef(int v) { fill(v); printf("[LONGC_TEST] Constructing object of dataRef (ctor2)\n"); }
    ~dataRef() { printf("[LONGC_TEST] Deconstructing object of dataRef\n"); }

    // Here define data reference to use operator []
    operator data_reference() { return m_value; }
    operator const_data_reference() const { return m_value; }

    int size() const { return vcount; }

protected:
    void fill(int v) {
        std::fill_n(&m_value[0], vcount, v);
    }

protected:
    int m_value[vcount];
};

class fooValue {
public:
    fooValue(const std::string &s, double d, double i) : m_str(s), dval(d), ival(i) { }
    fooValue() : m_str(""), dval(0), ival(0) { }
    ~fooValue() { }

    void print() const {
        printf("[LONGC_TEST] Values: m_str: %s, dval: %.2f, ival: %d.\n", m_str.c_str(), dval, ival);
    }

public:
    std::string m_str;
    double dval;
    int    ival;
};

class foo {
public:
    foo() : m_key("") { }
    foo(const std::string &s) : m_key(s) { }
    ~foo() { }
public:
    void print() const {
        printf("[LONGC_TEST] foo key value: %s.\n", m_key.c_str());
        for (const auto &v : m_values) {
            v.print();
        }
    }
    void add(fooValue&& v) {
        printf("[LONGC_TEST] Move version for 'add' executed.\n");
        m_values.push_back(v);
    }

    template<typename T> boost::tuple<T, T> gettuple(T x, T y) const;
public:
    std::string m_key;
    std::list<fooValue> m_values;
};

template<typename T> boost::tuple<T, T> foo::gettuple(T x, T y) const {
    T a, b;
    a = 2 * x + 4 * y;
    b = 3 * x + 7 * y;

    return boost::make_tuple(a, b);
}


class zoo_data {
public:
    zoo_data() : m_name("unkown zoo_data"), m_tiger(2), m_monkey(3) { }
    zoo_data(int t, int m) : m_name("unkown zoo_data"), m_tiger(t), m_monkey(m) { }
    zoo_data(const std::string &s, int t, int m) : m_name(s), m_tiger(t), m_monkey(m) { }
    ~zoo_data() { }

public:
    void print() const {
        printf("zoo_data Name is: %s, Tiger: %d, Monkey: %d\n", m_name.c_str(), m_tiger, m_monkey);
    }

public:
    bool operator==(const zoo_data& rhs) const {
        return m_name == rhs.get_name() && m_tiger == rhs.get_tiger() && m_monkey == rhs.get_monkey();
    }
    bool operator==(const std::string& s) const {
        return m_name == s;
    }

    void generate_year_workers() {
       const int wsize = 10;
       m_year_workers.resize(wsize);
       for (int i = 0; i < wsize; ++i) {
          m_year_workers[i] = std::make_pair(1990 + i, 156 + i);
       }
    }

    std::pair<int, int>& operator[](int i) { return m_year_workers[i]; }
    const std::pair<int, int>& operator[](int i) const { return m_year_workers[i]; }

public:
    const int get_tiger() const { return m_tiger; }
    const int get_monkey() const { return m_monkey; }
    const boost::optional<int>& get_crocodile() const { return m_crocodile; }
    const std::string get_name() const { return m_name; }

public:
    // static void o

private:
    std::string m_name;
    int m_tiger;
    int m_monkey;
    boost::optional<int> m_crocodile;

    std::vector<std::pair<int, int>> m_year_workers;
};

template<typename T> struct SharedArray {
    template<typename IIterator>
    SharedArray(IIterator begin, IIterator end) {
        boost::shared_ptr<std::vector<T>> store(new std::vector<T>(begin, end));
        if (store->size())
            m_data.reset(&(*store)[0]);
    }

    T* begin() { return m_data.get(); }
    T* end() { return m_data.get() + 10; }

    boost::shared_ptr<T[]> m_data;
};

template<typename T, bool> inline int getAniValue(const T &v) { return 0; }

template<> inline int getAniValue<zoo_data, true>(const zoo_data &v) { return v.get_monkey(); }
template<> inline int getAniValue<zoo_data, false>(const zoo_data &v) { return v.get_tiger(); }

template<> inline int getAniValue<zoo, true>(const zoo &v) { return v.get_tiger(); }
template<> inline int getAniValue<zoo, false>(const zoo &v) { return v.get_monkey(); }



bool test_temporary_param(std::string &str);

int sparse2dense(double* sparse, int row_sparse, int col_sparse, double* dense, int row_dense, int col_dense);

void print_matrix( const char* desc, int m, int n, double* a, int lda );

bool custom_compare(int x, bool (*func)(int i) = nullptr);

class vec_list {
public:
   vec_list(std::vector<int> &&vec) : m_vec(vec) { }
public:
   void print_all() {
      for (const auto v : m_vec) {
         printf("%d-", v);
      }
      printf("\n");
   }
public:
   std::vector<int> m_vec;
};

class GeoPoint {
public:
   GeoPoint() = delete;
   GeoPoint(int i, int j) : m_x(i), m_y(j) { }
   GeoPoint(const GeoPoint &p) : m_x(p.x()), m_y(p.y()) { }
   GeoPoint(GeoPoint &&p) = default;
public:
   const int& x() const { return m_x; }
   const int& y() const { return m_y; }
   int& x() { return m_x; }
   int& y() { return m_y; }
   virtual void pprint() {
      printf("2D point (x, y) = (%d, %d)\n", m_x, m_y);
   }
public:
   int m_x;
   int m_y;
};

class Point3D : public GeoPoint {
public:
   Point3D() = delete;
   Point3D(int i, int j, int k) : GeoPoint(i, j), m_z(k) { }
   // Point3D(const Point3D &p) : GeoPoint(p.x(), p.y()), m_z(p.z()) { }
   Point3D(const Point3D &p) = default;
   Point3D(Point3D &&p) = default;
   virtual ~Point3D() { }

public:
   const int& z() const { return m_z; }
   int& z() { return m_z; }
   void pprint() {
      printf("3D point (x, y, z) = (%d, %d, %d)\n", m_x, m_y, m_z);
   }
public:
   Point3D& operator=(const Point3D &pt) {
      m_x = pt.x();
      m_y = pt.y();
      m_z = pt.z();

      return *this;
   }

   void swap(Point3D &pt) {
      Point3D temp(*this);
      *this = pt;
      pt = temp;
   }
public:
   int m_z;
};

struct keyHash {
   std::size_t operator() (std::pair<int32_t, bool> const &v) const noexcept {
      std::size_t h1 = std::hash<int32_t>{}(v.first);
      std::size_t h2 = std::hash<bool>{}(v.second);
      boost::hash_combine(h1, h2);
      return h1;
   }
};

struct keyPairHash {
   std::size_t operator() (std::pair<int32_t, int32_t> const &v) const noexcept {
      std::size_t h = std::hash<int32_t>{}(v.first < v.second ? v.first : v.second);
      std::size_t h0 = std::hash<int32_t>{}(v.first < v.second ? v.second : v.first);
      boost::hash_combine(h, h0);
      return h;
   }
};

struct keyPairEqual {
   bool operator() (std::pair<int32_t, int32_t> const &a, std::pair<int32_t, int32_t> const &b) const noexcept {
      printf("keyPairEqual executed once!\n");
      return ((a == b) || ((a.first == b.second) && (a.second == b.first)));
   }
};

template<ISK_TYPE IType>
void partial_specialization_test(const int i) {
   if (IType == TYPE_0) {
      printf("partial_specialization_test write TYPE_0\n");
   } else {
      printf("partial_specialization_test write no type\n");
   }
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

    

} // namespace NORMAL

#endif //_NORMAL_HPP
