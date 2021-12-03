#include "catch.hpp"

TEST_CASE("Little or Big Endian", "[Endianness]") {

    /*
    If this test passes, the system is Little Endian
    if this test fails, the following may fail and need to be reworked:
    hdbitset::rotl
    hdbitset::rotr
    hdbitset::permutate
    */

    int n = 1;
    REQUIRE(*(char*)&n == 1);

}

