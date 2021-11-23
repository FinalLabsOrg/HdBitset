#include "catch.hpp"
#include "../HD/hdbitset.hpp"
#include "../HD/factory.hpp"
#include "../HD/ops.hpp"
#include "../HD/hdops.hpp"

#include <memory>

using namespace hyperdimensional;

TEST_CASE("hdops::majority", "[hdops]") {

    const unsigned uSize = 10048;
    std::shared_ptr<std::map<unsigned, std::shared_ptr<hdbitset<uSize>>>> pMap = std::make_shared<std::map<unsigned, std::shared_ptr<hdbitset<uSize>>>>();
    std::shared_ptr<hdbitset<uSize>> pMajority;

    GIVEN("Given 3 zero and 2 one hdbitsets in a map") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero1 = factory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pBitsetZero2 = factory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pBitsetZero3 = factory<uSize>::zero();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne1 = factory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne2 = factory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne3 = factory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne4 = factory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetRandom = factory<uSize>::random();

        pMap->insert({ 1, pBitsetZero1 });
        pMap->insert({ 2, pBitsetZero2 });
        pMap->insert({ 3, pBitsetZero3 });
        pMap->insert({ 4, pBitsetOne1 });
        pMap->insert({ 5, pBitsetOne2 });

        pMajority = hdops<uSize>::majority(pMap);

        THEN("Their majority is zero") {

            REQUIRE(ops<uSize>::eq(pMajority, pBitsetZero1));

        }

        AND_GIVEN("A 3rd one bitset and a random bitset in the map") {

            pMap->insert({ 6, pBitsetOne3 });
            pMap->insert({ 7, pBitsetRandom });

            pMajority = hdops<uSize>::majority(pMap);

            THEN("The majority is the random vector") {

                REQUIRE(7 == pMap->size());
                REQUIRE(ops<uSize>::eq(pMajority, pBitsetRandom));

            }

            AND_GIVEN("An 8th (one) bitset in the map") {

                pMap->insert({ 8, pBitsetOne4 });

                pMajority = hdops<uSize>::majority(pMap);

                THEN("The majority is a new random vector") {

                    REQUIRE(8 == pMap->size());
                    REQUIRE(ops<uSize>::noteq(pMajority, pBitsetRandom));
                    REQUIRE(!pMajority->uniform());

                }

            }

         }

    }

}

TEST_CASE("hdops::XOR", "[hdops]") {

    const unsigned uSize = 10048;

    GIVEN("An all-zero and an all-one bitset") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero = factory<uSize>::one();
        std::shared_ptr<hdbitset<uSize>> pBitsetOne = factory<uSize>::zero();

        THEN("Their binding is all one") {

            REQUIRE(hdops<uSize>::XOR(pBitsetOne, pBitsetZero)->all());

        }

    }

    GIVEN("Two random HdBtisets") {

        std::shared_ptr<hdbitset<uSize>> pBitset1 = factory<uSize>::random();
        std::shared_ptr<hdbitset<uSize>> pBitset2 = factory<uSize>::random();

        THEN("Their binding equals their XORed values") {

            std::shared_ptr<hdbitset<uSize>> pBoundBitset = hdops<uSize>::XOR(pBitset1, pBitset2);

            hdbitset<uSize> oXor((*pBitset1) ^ (*pBitset2));

            REQUIRE(*pBoundBitset == oXor);
            REQUIRE_FALSE(pBoundBitset->uniform());


        }

    }

}