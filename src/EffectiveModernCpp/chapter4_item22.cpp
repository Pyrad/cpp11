#include "chapter4_item22.hpp"
#include <stdio.h>
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
    FooPImpl() : m_id(FooPImplRawPtr::id_cnt++), m_name("unknown") { }
    FooPImpl(const std::string &n) : m_id(FooPImplRawPtr::id_cnt++), m_name(n) { }
    ~FooPImpl() { }

public:
    uint32_t id() const { return m_id; }

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
 * Implementation of the dtor
 */
FooPImplRawPtr::~FooPImplRawPtr() { }

uint32_t FooPImplRawPtr::id() const { return m_pimpl->id(); }

void FooPImplRawPtr::echo() const { fprintf(stdout, "[FooPImplRawPtr] A foo (ID = %u)\n", m_pimpl->id()); }


void test_pimpl_use_raw_ptr() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    FooPImplRawPtr fobj0;
    FooPImplRawPtr fobj1("sky");

    fprintf(stdout, "An object of class FooPImplRawPtr(ID = %u)\n", fobj0.id());
    fprintf(stdout, "An object of class FooPImplRawPtr(ID = %u)\n", fobj1.id());

}


void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_pimpl_use_raw_ptr();

}


} // namespace item_22

} // namespace chapter_4

} // namespace effective_mordern_cpp

