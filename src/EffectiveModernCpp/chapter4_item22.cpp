#include "chapter4_item22.hpp"
#include <limits>
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

namespace effective_mordern_cpp {

namespace chapter_4 {

namespace item_22 {

uint32_t FooPImplRawPtr::id_cnt = 0;

/**
 * Implementation of private class FooPImpl in class FooPImplRawPtr
 */
class FooPImplRawPtr::FooPImpl {
public:
    // default ctor
    FooPImpl() : m_id(FooPImplRawPtr::id_cnt++), m_name("unknown") { }
    // custom ctor
    FooPImpl(const std::string &n) : m_id(FooPImplRawPtr::id_cnt++), m_name(n) { }
    // custom ctor for deep copy
    FooPImpl(const FooPImpl *p) :
        m_id(FooPImplRawPtr::id_cnt++),
        m_name(p ? p->name() : "unknown"),
        m_ivec(p ? p->vec() : std::vector<int>()) { }

public:
    uint32_t id() const { return m_id; }
    const std::string & name() const { return m_name; }
    const std::vector<int> & vec() const { return m_ivec; }

public:
    uint32_t m_id = 0;
    std::string m_name = "unknown";
    std::vector<int> m_ivec;
}; // end clas FooPImplRawPtr::FooPImpl

/**
 * Implementation of the default ctor
 */
FooPImplRawPtr::
FooPImplRawPtr() : m_pimpl(new FooPImpl()) { }

/**
 * Implementation of the custom ctor
 */
FooPImplRawPtr::
FooPImplRawPtr(const std::string &n) : m_pimpl(new FooPImpl(n)) { }


/**
 * Copy ctor of deep copy
 */
FooPImplRawPtr::
FooPImplRawPtr(const FooPImplRawPtr &other) :
    m_pimpl(new FooPImpl(other.ptr())) { }


/**
 * Move ctor
 */
FooPImplRawPtr::
FooPImplRawPtr(FooPImplRawPtr &&other) :
    m_pimpl(other.ptr()) { other.reset_ptr(); }

/**
 * Implementation of the dtor
 */
FooPImplRawPtr::~FooPImplRawPtr() {
    // Must check the ptr, because move ctor exists
    if (m_pimpl) {
        delete m_pimpl;
    }
}

uint32_t FooPImplRawPtr::id() const {
    if (m_pimpl) {
        return m_pimpl->id();
    } else {
        return std::numeric_limits<uint32_t>::max();
    }
}

void FooPImplRawPtr::echo() const {
    if (m_pimpl) {
        fprintf(stdout, "[FooPImplRawPtr] A foo (ID = %u)\n", m_pimpl->id());
    } else {
        fprintf(stdout, "[FooPImplRawPtr] A foo has already been reset/moved\n");
    }
}


uint32_t FooPImplUniquePtr::id_cnt = 0;

/**
 * Implementation of private class FooPImpl in class FooPImplUniquePtr
 */
class FooPImplUniquePtr::FooPImpl {
public:
    FooPImpl() : m_id(FooPImplUniquePtr::id_cnt++), m_name("unknown") { }
    FooPImpl(const std::string &n) : m_id(FooPImplUniquePtr::id_cnt++), m_name(n) { }
    // custom ctor for deep copy
    FooPImpl(const std::unique_ptr<FooPImpl> &p) :
        m_id(FooPImplUniquePtr::id_cnt++),
        m_name(p ? p->name() : "unknown"),
        m_ivec(p ? p->vec() : std::vector<int>()) { }
    ~FooPImpl() { }

public:
    uint32_t id() const { return m_id; }
    const std::string & name() const { return m_name; }
    const std::vector<int> & vec() const { return m_ivec; }

public:
    uint32_t m_id = 0;
    std::string m_name = "unknown";
    std::vector<int> m_ivec;
}; // end clas FooPImplUniquePtr::FooPImpl

/**
 * Implementation of the default ctor
 */
FooPImplUniquePtr::
FooPImplUniquePtr() : m_pimpl(std::make_unique<FooPImpl>()) { }

/**
 * Implementation of the custom ctor
 */
FooPImplUniquePtr::
FooPImplUniquePtr(const std::string &n) : m_pimpl(std::make_unique<FooPImpl>(n)) { }

/**
 * Implementation of the copy ctor
 */
FooPImplUniquePtr::
FooPImplUniquePtr(const FooPImplUniquePtr &other) :
    m_pimpl(std::make_unique<FooPImpl>(other.ptr())) {}

/**
 * Implementation of the move ctor.
 * In this function, other.ptr() needs to be an rvalue, because std::unique_ptr
 * only has move ctor, no copy ctor, so other.ptr() must have an overload which
 * returns an rvalue reference.
 */
FooPImplUniquePtr::
FooPImplUniquePtr(FooPImplUniquePtr &&other) :
    m_pimpl(other.ptr()) { }

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
 *
 * Another implementation of this kind of PImpl class is to use std::shared_ptr,
 * thus it can allow incomplete when using special member functions.
 */
FooPImplUniquePtr::~FooPImplUniquePtr() { }

uint32_t FooPImplUniquePtr::id() const {
    if (m_pimpl) {
        return m_pimpl->id();
    } else {
        return std::numeric_limits<uint32_t>::max();
    }
}

void FooPImplUniquePtr::echo() const {
    if (m_pimpl) {
        fprintf(stdout, "[FooPImplUniquePtr] A foo (ID = %u)\n", m_pimpl->id());
    } else {
        fprintf(stdout, "[FooPImplUniquePtr] A foo has already been reset/moved\n");
    }
}

void test_pimpl_use_raw_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    FooPImplRawPtr fobj0;        // Use default ctor
    FooPImplRawPtr fobj1("sky"); // Use custom ctor
    FooPImplRawPtr fobj2(fobj1); // Use copy ctor
    FooPImplRawPtr fobj3(std::move(fobj0)); // Use move ctor

    fprintf(stdout, "An object of class FooPImplRawPtr(ID = %u)\n", fobj0.id());
    fprintf(stdout, "An object of class FooPImplRawPtr(ID = %u)\n", fobj1.id());
    fprintf(stdout, "An object of class FooPImplRawPtr(ID = %u)\n", fobj2.id());
    fprintf(stdout, "An object of class FooPImplRawPtr(ID = %u)\n", fobj3.id());

}


void test_pimpl_use_unique_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    FooPImplUniquePtr fobj0;        // Use default ctor
    FooPImplUniquePtr fobj1("sky"); // Use custom ctor
    FooPImplUniquePtr fobj2(fobj1); // Use copy ctor
    FooPImplUniquePtr fobj3(fobj0); // Use move ctor

    fprintf(stdout, "An object of class FooPImplUniquePtr(ID = %u)\n", fobj0.id());
    fprintf(stdout, "An object of class FooPImplUniquePtr(ID = %u)\n", fobj1.id());
    fprintf(stdout, "An object of class FooPImplUniquePtr(ID = %u)\n", fobj2.id());
    fprintf(stdout, "An object of class FooPImplUniquePtr(ID = %u)\n", fobj3.id());
}


void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_pimpl_use_raw_ptr();

    test_pimpl_use_unique_ptr();
}


} // namespace item_22

} // namespace chapter_4

} // namespace effective_mordern_cpp

