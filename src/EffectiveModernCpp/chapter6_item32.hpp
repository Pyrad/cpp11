#ifndef EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM32
#define EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM32

#include "../utilities/utilities.hpp"
#include <string>

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_32 {

class Foobar {
public:
    Foobar() = default;
    Foobar(int val, const std::string &name) : m_value(val), m_name(name) { }

public:
    bool is_validated() const { return m_value > 0; }
    bool is_processed() const { return (m_value > 0) && (m_name != "unknown"); }
    bool is_archived() const { return (m_value < 0) && (m_name == "archived"); }

private:
    int m_value = 0;
    std::string m_name = "unknown";
}; // end class Foobar

/**
 * @brief To show C++11 can achieve move capture by a class
 */
class IsValAndArch { // "is validated and archived"
public:
    // Type alias
	using DataType = std::unique_ptr<Foobar>;

	// Use of std::move
    explicit IsValAndArch(DataType&& ptr) : pw(std::move(ptr)) {}

    // Define a callable operator
	bool operator()() const { return pw->is_validated() && pw->is_archived(); }

private:
    // A data member for capture
	DataType pw;

}; // end class IsValAndArch

/**
 * As we know, C++11 doesn't support move capture, C++14 enhanced this
 * and C++14 now support move capture.
 *
 * What are the 2 cases which C++14 support while C++11 doesn't?
 * --------------------------------------------------------------
 * (1) Move a move-only object into a closure (e.g., std::unique_ptr, std::future)
 * (2) Move an object whose move operation is more effective than its copy operation
 *     to a closure, for example, containers in STL.
 *
 * C++14 introduced a mechanism called "init capture", which is also called
 * "generalized lambda capture".
 *
 * What can a generalized lambda capture explicitly specify?
 * --------------------------------------------------------------
 * (1) Names for the data members in the closure generated by the lambda
 * (2) An expression initializing the data members
 */
void test_cxx14_supports_move_capture();

/**
 * The essence of a lambda is that have compiler create a class and create
 * an object of that class by using an easy way.
 *
 * There's nothing you can do with a lambda that you can't do by hand.
 */
void test_cxx11_achieve_move_capture_by_class();

void test_all();

} // namespace item_32

} // namespace chapter_6

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM32
