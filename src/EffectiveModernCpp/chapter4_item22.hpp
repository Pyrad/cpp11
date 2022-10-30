#ifndef EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM22
#define EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM22

#include "../utilities/utilities.hpp"
#include <memory>
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


/**
 * This is a class using PImpl idiom, by a std::unique_ptr.
 *
 * This is the unique_ptr version of FooPImplRawPtr
 */
class FooPImplUniquePtr {
public:
    static uint32_t id_cnt;

public:
    /* Default ctor implementation is in source file */
    FooPImplUniquePtr();

    /* Custom ctor implementation is in source file */
    FooPImplUniquePtr(const std::string &n);

    /**
     * The dtor implementation is in source file
     *
     * Now we use a std::unique_ptr to manange the resource and we don't
     * have to release the resource on our own.
     *
     * But we have to define a destructor here, and implementate it in the
     * source file, because if we don't provide the default destructor, the
     * compiler will generate one for us, and in that function the std::unique_ptr
     * will release the resources.
     *
     * That's where the problem is, because in the normal default deleter's
     * implementation for std::unique_ptr, static_assert() will be used to make
     * sure the raw pointer it points to is not an incomplete type.
     * Becuase the special functions are normally delcared as inline, thus the
     * auxiliar class (FooPImpl) is an incomplete type here, then an error occurs.
     *
     * So the solution to this issue it to declare the destructor in the header
     * file and then implementate it in the source file.
     * Anothe convenient solution is to just declare it as "=default"
     */
    ~FooPImplUniquePtr();

public:
    uint32_t id() const;
    void echo() const;

private:
    class FooPImpl;

private:
    std::unique_ptr<FooPImpl> m_pimpl;

}; // end class FooPImplUniquePtr


void test_pimpl_use_raw_ptr();

void test_pimpl_use_unique_ptr();


void test_all();

} // namespace item_22

} // namespace chapter_4

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER4_ITEM22
