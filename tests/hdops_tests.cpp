#include "catch.hpp"
#include "../HD/hdbitset.hpp"
#include "../HD/hdfactory.hpp"
#include "../HD/hdops.hpp"

#include <memory>

using namespace hyperdimensional;

TEST_CASE("hdops::majority", "[hdops]") {

    const unsigned uSize = 10048;
    std::shared_ptr<std::map<unsigned, std::shared_ptr<hdbitset<uSize>>>> pMap = std::make_shared<std::map<unsigned, std::shared_ptr<hdbitset<uSize>>>>();
    std::shared_ptr<hdbitset<uSize>> pMajority;

    GIVEN("Given 3 zero and 2 one hdbitsets in a map") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero1 = hdfactory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pBitsetZero2 = hdfactory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pBitsetZero3 = hdfactory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne1 = hdfactory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne2 = hdfactory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne3 = hdfactory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne4 = hdfactory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetRandom = hdfactory<uSize>::random();

        pMap->insert({ 1, pBitsetZero1 });
        pMap->insert({ 2, pBitsetZero2 });
        pMap->insert({ 3, pBitsetZero3 });
        pMap->insert({ 4, pBitsetOne1 });
        pMap->insert({ 5, pBitsetOne2 });

        pMajority = hdops<uSize>::majority(pMap);

        THEN("Their majority is zero") {

            REQUIRE(hdops<uSize>::eq(pMajority, pBitsetZero1));

        }

        AND_GIVEN("A 3rd one bitset and a random bitset in the map") {

            pMap->insert({ 6, pBitsetOne3 });
            pMap->insert({ 7, pBitsetRandom });

            pMajority = hdops<uSize>::majority(pMap);

            THEN("The majority is the random vector") {

                REQUIRE(7 == pMap->size());
                REQUIRE(hdops<uSize>::eq(pMajority, pBitsetRandom));

            }

            AND_GIVEN("An 8th (one) bitset in the map") {

                pMap->insert({ 8, pBitsetOne4 });

                pMajority = hdops<uSize>::majority(pMap);

                THEN("The majority is a new random vector") {

                    REQUIRE(8 == pMap->size());
                    REQUIRE(hdops<uSize>::noteq(pMajority, pBitsetRandom));
                    REQUIRE(!pMajority->uniform());

                }

            }

         }

    }

}

TEST_CASE("hdops::XOR", "[hdops]") {

    const unsigned uSize = 10048;

    GIVEN("An all-zero and an all-one bitset") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero = hdfactory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne = hdfactory<uSize>::zero();

        THEN("Their binding is all one") {

            REQUIRE(hdops<uSize>::XOR(pBitsetOne, pBitsetZero)->all());

        }

    }

    GIVEN("Two random HdBtisets") {

        std::shared_ptr<hdbitset<uSize>> pBitset1 = hdfactory<uSize>::random();
        std::shared_ptr<hdbitset<uSize>> pBitset2 = hdfactory<uSize>::random();

        THEN("Their binding equals their XORed values") {

            std::shared_ptr<hdbitset<uSize>> pBoundBitset = hdops<uSize>::XOR(pBitset1, pBitset2);

            hdbitset<uSize> oXor((*pBitset1) ^ (*pBitset2));

            REQUIRE(*pBoundBitset == oXor);
            REQUIRE_FALSE(pBoundBitset->uniform());


        }

    }

}

TEST_CASE("hdops::eq() and hdops::noteq()", "[hdops]") {

    GIVEN("Two empty HdBitsets are created") {

        const unsigned uSize = 10048;

        std::shared_ptr<hdbitset<uSize>> pHdBitset1 = hdfactory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pHdBitset2 = hdfactory<uSize>::zero();

        THEN("Initially they are equal") {
            REQUIRE(hdops<uSize>::eq(pHdBitset1, pHdBitset2));
            REQUIRE_FALSE(hdops<uSize>::noteq(pHdBitset1, pHdBitset2));
        }

        WHEN("Altering one of the bitsets") {

            pHdBitset1->set(0);

            THEN("They are no longer equal") {
                REQUIRE_FALSE(hdops<uSize>::eq(pHdBitset1, pHdBitset2));
                REQUIRE(hdops<uSize>::noteq(pHdBitset1, pHdBitset2));
            }

            THEN("The altered one is neither all ones, neither all zeroes and is not uniform") {
                REQUIRE_FALSE(pHdBitset1->all());
                REQUIRE_FALSE(pHdBitset1->none());
                REQUIRE_FALSE(pHdBitset1->uniform());
            }

        }

    }

    GIVEN("One random HdBitsets and its copy are created") {

        const unsigned uSize = 10048;

        std::shared_ptr<hdbitset<uSize>> pHdBitset1 = hdfactory<uSize>::random();
        std::shared_ptr<hdbitset<uSize>> pHdBitset2 = hdfactory<uSize>::copy(pHdBitset1);

        THEN("Initially they are equal") {
            REQUIRE(hdops<uSize>::eq(pHdBitset1, pHdBitset2));
            REQUIRE_FALSE(hdops<uSize>::noteq(pHdBitset1, pHdBitset2));
        }

        WHEN("Altering one of the bitsets") {

            pHdBitset1->flip(5500);

            THEN("They are no longer equal") {
                REQUIRE_FALSE(hdops<uSize>::eq(pHdBitset1, pHdBitset2));
                REQUIRE(hdops<uSize>::noteq(pHdBitset1, pHdBitset2));
            }

            AND_WHEN("Altering the other the bitset similarly") {

                pHdBitset2->flip(5500);

                THEN("They are equal again") {
                    REQUIRE(hdops<uSize>::eq(pHdBitset1, pHdBitset2));
                    REQUIRE_FALSE(hdops<uSize>::noteq(pHdBitset1, pHdBitset2));
                }

            }

        }

    }

}

TEST_CASE("hdops::hamming()", "[hdops]") {

    const unsigned uSize = 10048;

    GIVEN("Two zero and two one bitsets") {

        std::shared_ptr<hdbitset<uSize>> pZero1 = hdfactory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pZero2 = hdfactory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pOne1 = hdfactory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pOne2 = hdfactory<uSize>::one();

        THEN("pZero1 vs pZero2 distance is zero") {

            REQUIRE_FALSE(hdops<uSize>::hamming(pZero1, pZero2));
            REQUIRE_FALSE(hdops<uSize>::hamming(pZero2, pZero1));

        }

        THEN("pOne1 vs pOne2 distance is zero") {

            REQUIRE_FALSE(hdops<uSize>::hamming(pOne1, pOne2));
            REQUIRE_FALSE(hdops<uSize>::hamming(pOne2, pOne1));

        }

        THEN("pOne1 vs pZero1 distance is size()") {

            REQUIRE(hdops<uSize>::hamming(pOne1, pZero1) == pOne1->size_u());
            REQUIRE(hdops<uSize>::hamming(pZero1, pOne1) == pOne1->size_u());

        }

    }

    GIVEN("HdOperator::factoryShuffle is just called for two vectors") {

        srand(0);

        std::shared_ptr<hdbitset<uSize>> pHdBitset1 = hdfactory<uSize>::random();
        std::shared_ptr<hdbitset<uSize>> pHdBitset2 = hdfactory<uSize>::random();

        THEN("Distance is commutative") {
            REQUIRE(hdops<uSize>::hamming(pHdBitset1, pHdBitset2) == hdops<uSize>::hamming(pHdBitset2, pHdBitset1));
        }

        THEN("Distance of pHdBitset1 and pHdBitset2 is usually within 10% range of size/2") {

            unsigned uDistance = hdops<uSize>::hamming(pHdBitset1, pHdBitset2);
            double dRatio = (double)uDistance / (double)pHdBitset1->size_u();
            Approx aRatioTarget = Approx(0.5).epsilon(0.05);
            CHECK(dRatio == aRatioTarget);

        }

    }

}