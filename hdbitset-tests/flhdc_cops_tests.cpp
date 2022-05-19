#include "catch.hpp"
#include "../HD/flhdc.h"

#include <memory>

using namespace flhdc;

TEST_CASE("cops::bundle", "[cops]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    const unsigned uSize = 10048;
    p_collection_t<hdbitset_collection_key_t, std::shared_ptr<hdbitset<uSize>>> pMap = std::make_shared<collection_t<hdbitset_collection_key_t, std::shared_ptr<hdbitset<uSize>>>>();

    GIVEN("Given 3 zero and 2 one hdbitsets in a map") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero1 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetZero2 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetZero3 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne1 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne2 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne3 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne4 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetRandom = hdbitset_factory<uSize>::random(pHdRandom);

        pMap->insert({ 1, pBitsetZero1 });
        pMap->insert({ 2, pBitsetZero2 });
        pMap->insert({ 3, pBitsetZero3 });
        pMap->insert({ 4, pBitsetOne1 });
        pMap->insert({ 5, pBitsetOne2 });

        std::shared_ptr<hdbitset<uSize>> pBundle = cops<uSize>::bundle(pMap);

        THEN("Their bundle is zero") {

            REQUIRE(ops<uSize>::eq(pBundle, pBitsetZero1));
            REQUIRE(ops<uSize>::eq(cops<uSize>::bundle(pMap), cops<uSize>::majority(pMap)));

        }

        AND_GIVEN("A 3rd one bitset and a random bitset in the map") {

            pMap->insert({ 6, pBitsetOne3 });
            pMap->insert({ 7, pBitsetRandom });

            std::shared_ptr<hdbitset<uSize>> pBundle = cops<uSize>::bundle(pMap);

            THEN("The bundle is the random vector") {

                REQUIRE(7 == pMap->size());
                REQUIRE(ops<uSize>::eq(pBundle, pBitsetRandom));
                REQUIRE(ops<uSize>::eq(cops<uSize>::bundle(pMap), cops<uSize>::majority(pMap)));

            }

            AND_GIVEN("An 8th (one) bitset in the map") {

                pMap->insert({ 8, pBitsetOne4 });

                std::shared_ptr<hdbitset<uSize>> pBundle = cops<uSize>::bundle(pMap);

                THEN("The bundle is a new random vector") {

                    REQUIRE(8 == pMap->size());
                    REQUIRE(ops<uSize>::noteq(pBundle, pBitsetRandom));
                    REQUIRE(!pBundle->uniform());

                    // these are most likely never equal because bundle and majority uses different random values
                    CHECK_FALSE(ops<uSize>::eq(cops<uSize>::bundle(pMap), cops<uSize>::majority(pMap)));

                }

            }

        }

    }

}

TEST_CASE("cops::get_closest_key", "[cops]") {
    
    p_randengine_t pRandengine = randengine_factory::factory(0, 0, 0);
    p_hdbitset10k_t pReference = hdbitset10k_factory::random(pRandengine);

    p_collection10k_t pCollection = std::make_shared<collection10k_t>();

    GIVEN("An empty initial map") {

        THEN("A logic_error exception is thrown") {
            REQUIRE_THROWS_AS(cops10k::get_closest_key(pCollection, pReference), std::logic_error);
        }

    }

    GIVEN("The good element in the map") {

        hdbitset_collection_key_t uExpectedId = 1111;
        pCollection->insert({ uExpectedId, pReference });

        THEN("The get_closest_key ID is the Id of this element") {

            hdbitset_collection_key_t uActualClosestId = cops10k::get_closest_key(pCollection, pReference);
            REQUIRE(uExpectedId == uActualClosestId);

            AND_GIVEN("The several additional random elements in the map") {

                pCollection->insert({ 1, hdbitset10k_factory::random(pReference->get_randengine())});
                pCollection->insert({ 111, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ 1110, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ 1112, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ 2, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ 3, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ 222, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ 333, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ 444, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ 555, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ -111, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ -1111, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ -1110, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ -12, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ -3, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ -41, hdbitset10k_factory::random(pReference->get_randengine()) });
                pCollection->insert({ -51, hdbitset10k_factory::random(pReference->get_randengine()) });

                THEN("The get_closest_key ID is still the Id of the reference element") {

                    uActualClosestId = cops10k::get_closest_key(pCollection, pReference);
                    REQUIRE(uExpectedId == uActualClosestId);

                }

                AND_GIVEN("The reference is perturbed a bit") {

                    p_hdbitset10k_t pReference2 = hdbitset10k_factory::copy(pReference);
                    pReference2->flip(0);
                    pReference2->flip(1);
                    pReference2->flip(2);
                    pReference2->flip(3);
                    pReference2->flip(4);
                    pReference2->flip(5);
                    pReference2->flip(6);
                    pReference2->flip(7);
                    pReference2->flip(8);

                    THEN("The get_closest_key ID is still the Id of the reference element") {

                        uActualClosestId = cops10k::get_closest_key(pCollection, pReference2);
                        REQUIRE(uExpectedId == uActualClosestId);

                        AND_THEN("Adding this perturbed item, its ID will be the get_closest_key one") {

                            hdbitset_collection_key_t uExpectedId2 = 9999;
                            pCollection->insert({ uExpectedId2, pReference2 });

                            uActualClosestId = cops10k::get_closest_key(pCollection, pReference2);
                            REQUIRE(uExpectedId2 == uActualClosestId);

                        }

                    }

                }
                
                AND_GIVEN("A brand new reference is used") {

                    p_hdbitset10k_t pReference3 = hdbitset10k_factory::random(pReference->get_randengine());

                    /*
                    This is a stochastic test and will fail on occasion.
                    */
                    THEN("The get_closest_key ID is not (neccesarily) the Id of the reference element") {

                        uActualClosestId = cops10k::get_closest_key(pCollection, pReference3);
                        REQUIRE(uExpectedId != uActualClosestId);

                    }

                }
            
            }

        }

    }

}

TEST_CASE("cops::majority", "[cops]") {

    std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

    const unsigned uSize = 10048;
    p_collection_t<hdbitset_collection_key_t, std::shared_ptr<hdbitset<uSize>>> pMap = std::make_shared<collection_t<hdbitset_collection_key_t, std::shared_ptr<hdbitset<uSize>>>>();
    std::shared_ptr<hdbitset<uSize>> pMajority;

    GIVEN("An empty initial map") {

        THEN("A logic_error exception is thrown") {
            REQUIRE_THROWS_AS(cops<uSize>::majority(pMap), std::logic_error);
        }

    }

    GIVEN("Given 3 zero and 2 one hdbitsets in a map") {

        std::shared_ptr<hdbitset<uSize>> pBitsetZero1 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetZero2 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetZero3 = hdbitset_factory<uSize>::zero(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne1 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne2 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne3 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetOne4 = hdbitset_factory<uSize>::one(pHdRandom);
        std::shared_ptr<hdbitset<uSize>> pBitsetRandom = hdbitset_factory<uSize>::random(pHdRandom);

        pMap->insert({ 1, pBitsetZero1 });
        pMap->insert({ 2, pBitsetZero2 });
        pMap->insert({ 3, pBitsetZero3 });
        pMap->insert({ 4, pBitsetOne1 });
        pMap->insert({ 5, pBitsetOne2 });

        pMajority = cops<uSize>::majority(pMap);

        THEN("Their majority is zero") {

            REQUIRE(ops<uSize>::eq(pMajority, pBitsetZero1));

        }

        AND_GIVEN("A 3rd one bitset and a random bitset in the map") {

            pMap->insert({ 6, pBitsetOne3 });
            pMap->insert({ 7, pBitsetRandom });

            pMajority = cops<uSize>::majority(pMap);

            THEN("The majority is the random vector") {

                REQUIRE(7 == pMap->size());
                REQUIRE(ops<uSize>::eq(pMajority, pBitsetRandom));

            }

            AND_GIVEN("An 8th (one) bitset in the map") {

                pMap->insert({ 8, pBitsetOne4 });

                pMajority = cops<uSize>::majority(pMap);

                THEN("The majority is a new random vector") {

                    REQUIRE(8 == pMap->size());
                    REQUIRE(ops<uSize>::noteq(pMajority, pBitsetRandom));
                    REQUIRE(!pMajority->uniform());

                }

            }

        }

    }

}