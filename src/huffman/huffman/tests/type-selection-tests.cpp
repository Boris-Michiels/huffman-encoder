#ifdef TEST_BUILD

#include "util.h"
#include "catch.hpp"
#include <type_traits>


void test_types()
{
#define CHECK_TYPE(N, TYPE) static_assert(std::is_same<SelectIntegerTypeByBytes<N>::type, TYPE>::value, "SelectIntegerTypeByBytes<" #N "> is not " #TYPE)
    CHECK_TYPE(1, uint8_t);
    CHECK_TYPE(2, uint16_t);
    CHECK_TYPE(3, uint32_t);
    CHECK_TYPE(4, uint32_t);
    CHECK_TYPE(5, uint64_t);
    CHECK_TYPE(6, uint64_t);
    CHECK_TYPE(7, uint64_t);
    CHECK_TYPE(8, uint64_t);
#undef CHECK_TYPE

#define CHECK_TYPE(N, TYPE) static_assert(std::is_same<SelectIntegerTypeByDomainSize<N>::type, TYPE>::value, "SelectIntegerTypeByDomainSize<" #N "> is not " #TYPE)
    CHECK_TYPE(2, uint8_t);
    CHECK_TYPE(255, uint8_t);
    CHECK_TYPE(256, uint8_t);
    CHECK_TYPE(65535, uint16_t);
    CHECK_TYPE(65536, uint16_t);
    CHECK_TYPE(65537, uint32_t);
    CHECK_TYPE(4294967295, uint32_t);
    CHECK_TYPE(4294967296, uint32_t);
    CHECK_TYPE(4294967297, uint64_t);
#undef CHECK_TYPE
}

#endif