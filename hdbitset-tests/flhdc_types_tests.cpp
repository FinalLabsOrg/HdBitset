#include "catch.hpp"
#include "../HD/flhdc.h"

using namespace flhdc;

TEST_CASE("type", "[type]") {

	GIVEN("flhdc_type.h is included") {

		p_randengine_t pHdRandom = randengine_factory::factory(0, 0, 0);

		hdbitset10k_t oHdBitset1(pHdRandom);
		hdbitset10k_t oHdBitset2(pHdRandom);

		p_hdbitset10k_t pHdBitset1 = hdbitset10k_factory::one(pHdRandom);
		p_hdbitset10k_t pHdBitset2 = hdbitset10k_factory::one(pHdRandom);
		p_hdbitset10k_t pHdBitset3 = hdbitset10k_factory::zero(pHdRandom);

		p_hdbitset_t<cuHd10k> pHdBitset4 = hdbitset10k_factory::zero(pHdRandom);

		THEN("They compile and work.") {

			REQUIRE(ops10k::eq(pHdBitset1, pHdBitset2));
			REQUIRE(ops10k::noteq(pHdBitset2, pHdBitset3));

			ops10k oOps10k;
			cops10k oCops10k;

			REQUIRE(sizeof(oOps10k) == sizeof(ops<cuHd10k>));
			REQUIRE(sizeof(oCops10k) == sizeof(cops<cuHd10k>));

		}

		THEN("Sizes are as expected.") {

			REQUIRE(pHdBitset1->size_u() == cuHd10k);
			REQUIRE(pHdBitset2->size_u() == cuHd10k);
			REQUIRE(pHdBitset3->size_u() == cuHd10k);

			REQUIRE(pHdBitset4->size_u() == cuHd10k);

		}

		THEN("p_hdbitset10k_t and p_hdbitset_t<cuHd10k> type ids are the same") {
			REQUIRE(typeid(pHdBitset3) == typeid(pHdBitset4));
		}

	}

	GIVEN("p_collection_t") {

		p_collection_t<unsigned, int> pCollection = std::make_shared<collection_t<unsigned, int>>();
		std::shared_ptr<std::map<unsigned, int>> pMap = std::make_shared<std::map<unsigned, int>>();

		THEN("its type is as expected") {
			REQUIRE(typeid(pCollection) == typeid(pMap));
		}

		AND_GIVEN("collection10k_t and p_collection10k_t are used") {

			collection10k_t oCollection;
			p_collection10k_t pCollection = std::make_shared<collection_t<hdbitset_collection_key_t, p_hdbitset10k_t>>();

			THEN("their types match") {
				REQUIRE(typeid(oCollection) == typeid(*pCollection));
			}

		}

	}

	GIVEN("hdbitset_collection_key_t is used") {

		THEN("it can store the hdbitset smart pointer") {

			p_randengine_t pRandEngine = randengine_factory::factory(1, 2, 3);

			std::shared_ptr<hdbitset<256>> pH1 = std::make_shared<hdbitset<256>>(pRandEngine);
			REQUIRE(sizeof(hdbitset_collection_key_t) > sizeof(pH1.get()));

			p_hdbitset10k_t p10k = hdbitset10k_factory::raw(pRandEngine);
			REQUIRE(sizeof(hdbitset_collection_key_t) > sizeof(p10k.get()));

		}

	}

	GIVEN("mpi_transfer_chunk_t is used") {

		THEN("it compiles") {

			mpi_transfer_chunk_t iChunk = 0;
			REQUIRE(sizeof(iChunk) > 1);
		}

	}

	GIVEN("p_mt19937_t is used") {

		p_randengine_t pRandEngine = randengine_factory::factory(1, 2, 3);
		p_mt19937_t pMt19937;
		
		THEN("Types match") {
			REQUIRE(typeid(p_mt19937_t) == typeid(pRandEngine->get_mt19937()));
		}
		
	}

}