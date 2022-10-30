#ifndef EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM22
#define EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM22

#include "../utilities/utilities.hpp"
#include <stdint.h>
#include <string>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_22 {

/**
 * This is a class using PImpl idiom, by a raw pointer.
 *
 * FooPImplRawPtr has only one data member, which is a raw pointer to a private
 * class defined in this class.
 *
 * The advantage of using the PImpl idiom is that if any real data member in the
 * implementation class change, only that source file gets changes, while this
 * header file won't be changed, thus those files which includes this file will
 * not be impacted, and compile time won't get longer.
 */
class FooPImplRawPtr {
public:
    static uint32_t id_cnt;

public:
    /* Default ctor implementation is in source file */
    FooPImplRawPtr();

    /* Custom ctor implementation is in source file */
    FooPImplRawPtr(const std::string &n);

    /* The dtor implementation is in source file */
    ~FooPImplRawPtr();

public:
    uint32_t id() const;
    void echo() const;

private:
    class FooPImpl;

private:
    FooPImpl *m_pimpl;
}; // end class FooPImplRawPtr


void test_pimpl_use_raw_ptr();


void test_all();

} // namespace item_22

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM22
