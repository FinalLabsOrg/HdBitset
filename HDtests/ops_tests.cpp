#include "catch.hpp"
#include "../HD/factory.hpp"
#include "../HD/ops.hpp"

using namespace hyperdimensional;

TEST_CASE("ops::eq() and ops::noteq()", "[ops]") {

    GIVEN("Two empty HdBitsets are created") {

        const unsigned uSize = 10048;

        std::shared_ptr<hdbitset<uSize>> pHdBitset1 = factory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pHdBitset2 = factory<uSize>::zero();

        THEN("Initially they are equal") {
            REQUIRE(ops<uSize>::eq(pHdBitset1, pHdBitset2));
            REQUIRE_FALSE(ops<uSize>::noteq(pHdBitset1, pHdBitset2));
        }

        WHEN("Altering one of the bitsets") {

            pHdBitset1->set(0);

            THEN("They are no longer equal") {
                REQUIRE_FALSE(ops<uSize>::eq(pHdBitset1, pHdBitset2));
                REQUIRE(ops<uSize>::noteq(pHdBitset1, pHdBitset2));
            }

            THEN("The altered one is neither all ones, neither all zeroes and is not uniform") {
                REQUIRE_FALSE(pHdBitset1->all());
                REQUIRE_FALSE(pHdBitset1->none());
                REQUIRE_FALSE(pHdBitset1->uniform());
            }

        }

    }

}

TEST_CASE("hamming()", "[ops]") {

    const unsigned uSize = 10048;

    GIVEN("Two zero and two one bitsets") {

        std::shared_ptr<hdbitset<uSize>> pZero1 = factory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pZero2 = factory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pOne1 = factory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pOne2 = factory<uSize>::one();

        THEN("pZero1 vs pZero2 distance is zero") {

            REQUIRE_FALSE(ops<uSize>::hamming(pZero1, pZero2));
            REQUIRE_FALSE(ops<uSize>::hamming(pZero2, pZero1));

        }

        THEN("pOne1 vs pOne2 distance is zero") {

            REQUIRE_FALSE(ops<uSize>::hamming(pOne1, pOne2));
            REQUIRE_FALSE(ops<uSize>::hamming(pOne2, pOne1));

        }

        THEN("pOne1 vs pZero1 distance is size()") {

            REQUIRE(ops<uSize>::hamming(pOne1, pZero1) == pOne1->size_u());
            REQUIRE(ops<uSize>::hamming(pZero1, pOne1) == pOne1->size_u());

        }

    }

    GIVEN("HdOperator::factoryShuffle is just called for two vectors") {

        srand(0);

        std::shared_ptr<hdbitset<uSize>> pHdBitset1 = factory<uSize>::random();
        std::shared_ptr<hdbitset<uSize>> pHdBitset2 = factory<uSize>::random();

        THEN("Distance is commutative") {
            REQUIRE(ops<uSize>::hamming(pHdBitset1, pHdBitset2) == ops<uSize>::hamming(pHdBitset2, pHdBitset1));
        }

        THEN("Distance of pHdBitset1 and pHdBitset2 is usually within 10% range of size/2") {

            unsigned uDistance = ops<uSize>::hamming(pHdBitset1, pHdBitset2);
            double dRatio = (double)uDistance / (double)pHdBitset1->size_u();
            Approx aRatioTarget = Approx(0.5).epsilon(0.05);
            CHECK(dRatio == aRatioTarget);

        }

    }

}
