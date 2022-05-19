#include "catch.hpp"
#include "../HD/flhdc.h"

#include <memory>

using namespace flhdc;

TEST_CASE("ops::bind", "[ops]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    const unsigned uSize = 10048;

    GIVEN("An all-zero and an all-one bitset") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne = hdbitset_factory<uSize>::zero(pHdRandom);

        THEN("Their binding is all one") {

            REQUIRE(ops<uSize>::bind(pBitsetOne, pBitsetZero)->all());
            REQUIRE(ops<uSize>::bind(pBitsetZero, pBitsetOne)->all());

        }

    }

    GIVEN("Two random HdBtisets") {

        std::shared_ptr<hdbitset<uSize>> pBitset1 = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitset2 = hdbitset_factory<uSize>::random(pHdRandom);

        THEN("Their binding equals their XORed values") {

            std::shared_ptr<hdbitset<uSize>> pBoundBitset = ops<uSize>::bind(pBitset1, pBitset2);

            hdbitset<uSize> oBind((*pBitset1) ^ (*pBitset2), pHdRandom);

            REQUIRE(*pBoundBitset == oBind);
            REQUIRE_FALSE(pBoundBitset->uniform());


        }

    }

}

TEST_CASE("ops::crossover", "[ops]") {

    p_randengine_t pRandEngine = randengine_factory::factory(0, 0, 0);
    unsigned uCut = 3333;

    GIVEN("A zero and a one bitset") {

        p_hdbitset10k_t pZero = hdbitset10k_factory::zero(pRandEngine);
        p_hdbitset10k_t pOne = hdbitset10k_factory::one(pRandEngine);

        THEN("Their crossover shows uCut ones") {
            p_hdbitset10k_t pCrossover = ops10k::crossover(pOne, pZero, uCut);
            unsigned uOne = pCrossover->count_u();
            REQUIRE(uOne == uCut);

            AND_THEN("Their reverse crossover shows uSize-uCut ones") {
                p_hdbitset10k_t pCrossover2 = ops10k::crossover(pZero, pOne, uCut);
                uOne = pCrossover2->count_u();
                unsigned uSize = pCrossover2->size_u(); // 10048
                REQUIRE(uOne == (uSize - uCut));
            }

        }

    }

    GIVEN("Two random hdbitsets and their flips") {

        p_hdbitset10k_t pBitset1 = hdbitset10k_factory::random(pRandEngine);
        p_hdbitset10k_t pFlippedBitset1 = hdbitset10k_factory::copy(pBitset1);
        pFlippedBitset1->flip();

        p_hdbitset10k_t pBitset2 = hdbitset10k_factory::random(pRandEngine);
        p_hdbitset10k_t pFlippedBitset2 = hdbitset10k_factory::copy(pBitset2);
        pFlippedBitset2->flip();

        THEN("Their crossover flipped and their flippied crossover are the same") {

            p_hdbitset10k_t pCrossover = ops10k::crossover(pBitset1, pBitset2, 5000);
            pCrossover->flip();

            pBitset1->flip();
            pBitset2->flip();
            p_hdbitset10k_t pFlippedCrossover = ops10k::crossover(pBitset1, pBitset2, 5000);

            REQUIRE(ops10k::eq(pCrossover, pFlippedCrossover));

        }

    }

}

TEST_CASE("ops::eq() and ops::noteq()", "[ops]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    GIVEN("Two empty HdBitsets are created") {

        const unsigned uSize = 10048;

        std::shared_ptr<hdbitset<uSize>> pHdBitset1 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pHdBitset2 = hdbitset_factory<uSize>::zero(pHdRandom);

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

    GIVEN("One random HdBitsets and its copy are created") {

        const unsigned uSize = 10048;

        std::shared_ptr<hdbitset<uSize>> pHdBitset1 = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pHdBitset2 = hdbitset_factory<uSize>::copy(pHdBitset1);

        THEN("Initially they are equal") {
            REQUIRE(ops<uSize>::eq(pHdBitset1, pHdBitset2));
            REQUIRE_FALSE(ops<uSize>::noteq(pHdBitset1, pHdBitset2));
        }

        WHEN("Altering one of the bitsets") {

            pHdBitset1->flip(5500);

            THEN("They are no longer equal") {
                REQUIRE_FALSE(ops<uSize>::eq(pHdBitset1, pHdBitset2));
                REQUIRE(ops<uSize>::noteq(pHdBitset1, pHdBitset2));
            }

            AND_WHEN("Altering the other the bitset similarly") {

                pHdBitset2->flip(5500);

                THEN("They are equal again") {
                    REQUIRE(ops<uSize>::eq(pHdBitset1, pHdBitset2));
                    REQUIRE_FALSE(ops<uSize>::noteq(pHdBitset1, pHdBitset2));
                }

            }

        }

    }

}

TEST_CASE("ops::hamming()", "[ops]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);
    const unsigned uSize = 10048;

    GIVEN("Two zero and two one bitsets") {

        std::shared_ptr<hdbitset<uSize>> pZero1 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pZero2 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pOne1 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pOne2 = hdbitset_factory<uSize>::one(pHdRandom);

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

        std::shared_ptr<hdbitset<uSize>> pHdBitset1 = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pHdBitset2 = hdbitset_factory<uSize>::random(pHdRandom);

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

TEST_CASE("ops::release", "[ops]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);
    const unsigned uSize = 10048;

    GIVEN("An all-zero and an all-one bitset") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne = hdbitset_factory<uSize>::zero(pHdRandom);

        THEN("Their output is all one") {

            REQUIRE(ops<uSize>::release(pBitsetOne, pBitsetZero)->all());
            REQUIRE(ops<uSize>::release(pBitsetZero, pBitsetOne)->all());

        }

    }

    GIVEN("Two random HdBtisets") {

        std::shared_ptr<hdbitset<uSize>> pBitset1 = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitset2 = hdbitset_factory<uSize>::random(pHdRandom);

        THEN("Their release equals their XORed values") {

            std::shared_ptr<hdbitset<uSize>> pReleasedBitset = ops<uSize>::release(pBitset1, pBitset2);

            hdbitset<uSize> oBind((*pBitset1) ^ (*pBitset2), pHdRandom);

            REQUIRE(*pReleasedBitset == oBind);
            REQUIRE_FALSE(pReleasedBitset->uniform());


        }

    }

    GIVEN("Two random HdBitsets") {

        std::shared_ptr<hdbitset<uSize>> pMarker = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pValue = hdbitset_factory<uSize>::random(pHdRandom);

        THEN("Their binding and subsequent release equals pValue") {

            std::shared_ptr<hdbitset<uSize>> pBound = ops<uSize>::bind(pMarker, pValue);
            std::shared_ptr<hdbitset<uSize>> pReleased = ops<uSize>::release(pMarker, pBound);

            REQUIRE(ops<uSize>::eq(pValue, pReleased));

        }

    }

}

TEST_CASE("ops::sequence_extend()", "[ops]") {

    GIVEN("Three random hdbitsets") {

        constexpr unsigned uSize = 10048;
        std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

        std::shared_ptr<hdbitset<uSize>> pSequence1     = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pSequence2     = hdbitset_factory<uSize>::zero(pHdRandom);

        std::shared_ptr<hdbitset<uSize>> pFirst         = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pSecond        = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pThird         = hdbitset_factory<uSize>::random(pHdRandom);

        THEN("Extending the initial sequence with pFirst equals rotl(pSequence1) ^ pFirst") {

            ops<uSize>::sequence_extend(pSequence1, pFirst);

            pSequence2->rotl();
            pSequence2 = ops<uSize>::XOR(pSequence2, pFirst);
            REQUIRE(ops<uSize>::eq(pSequence1, pSequence2));

            AND_THEN("Any new sequences follow the same pattern") {

                ops<uSize>::sequence_extend(pSequence1, pSecond);

                pSequence2->rotl();
                pSequence2 = ops<uSize>::XOR(pSequence2, pSecond);
                REQUIRE(ops<uSize>::eq(pSequence1, pSequence2));

            }

        }


    }

}

TEST_CASE("ops::XOR", "[ops]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    const unsigned uSize = 10048;

    GIVEN("An all-zero and an all-one bitset") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne = hdbitset_factory<uSize>::zero(pHdRandom);

        THEN("Their XOR is all one") {

            REQUIRE(ops<uSize>::XOR(pBitsetOne, pBitsetZero)->all());

        }

    }

    GIVEN("Two random HdBtisets") {

        std::shared_ptr<hdbitset<uSize>> pBitset1 = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitset2 = hdbitset_factory<uSize>::random(pHdRandom);

        THEN("Their binding equals their XORed values") {

            std::shared_ptr<hdbitset<uSize>> pBoundBitset = ops<uSize>::XOR(pBitset1, pBitset2);

            hdbitset<uSize> oXor((*pBitset1) ^ (*pBitset2), pHdRandom);

            REQUIRE(*pBoundBitset == oXor);
            REQUIRE_FALSE(pBoundBitset->uniform());


        }

    }

}

TEST_CASE("ops::XOR_inplace", "[ops]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    const unsigned uSize = 10048;

    GIVEN("An all-zero and an all-one bitset") {

        std::shared_ptr<hdbitset<uSize>> pZero = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pOne = hdbitset_factory<uSize>::one(pHdRandom);

        THEN("Their XOR_inplace is all one") {
            ops<uSize>::XOR_inplace(pZero, pOne);
            REQUIRE(pZero->all());
        }

    }

    GIVEN("Two random HdBtisets") {

        std::shared_ptr<hdbitset<uSize>> pBitset1 = hdbitset_factory<uSize>::random(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitset2 = hdbitset_factory<uSize>::random(pHdRandom);

        THEN("Their XOR_inplace creates the XOR-ed value") {

            hdbitset<uSize> oXor((*pBitset1) ^ (*pBitset2), pHdRandom);

            ops<uSize>::XOR_inplace(pBitset1, pBitset2);

            REQUIRE(*pBitset1 == oXor);
            REQUIRE_FALSE(pBitset1->uniform());


        }

    }

}
