#include "catch.hpp"
#include "../HD/flhdc.h"

TEST_CASE("Little or Big Endian", "[Endianness]") {

    /*
    If this test passes, the system is Little Endian
    if this test fails, the following may fail and need to be reworked:
    hdbitset::rotl
    hdbitset::rotr
    hdbitset::scroll
    */

    int n = 1;
    REQUIRE(*(char*)&n == 1);

}

TEST_CASE("Shared Pointer Size", "[PointerSize]") {

    /*
    If this test passes, the size of the pointer pointed to by the shared ptr is 8 bytes.
    If this test fails, adjust the appropriate hdbitset_collection_key_t type in types.h 
    so that hdbitset_collection_key_t is large enough to contain an integer value of the pointer
    and is large enough so that it can be used as a generic id for a really large number of bitsets.
    */
    constexpr unsigned cuExpectedSize = 8;

    flhdc::p_randengine_t pRandEngine = flhdc::randengine_factory::factory(1, 2, 3);
    flhdc::p_hdbitset10k_t p10k = flhdc::hdbitset10k_factory::raw(pRandEngine);

    REQUIRE( sizeof(p10k.get()) == cuExpectedSize);

}