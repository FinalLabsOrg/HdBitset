#include "catch.hpp"
#include "../HD/flhdc.h"

using namespace flhdc;

TEST_CASE("hdbitset_factory::copy()", "[hdbitset_factory]") {

	const unsigned uSize = 10048;
	std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

	GIVEN("hdbitset_factory::random() is just called") {

		std::shared_ptr<hdbitset<uSize>> pHdBitset = hdbitset_factory<uSize>::random(pHdRandom);

		WHEN("Copying the thus created bitset") {

			std::shared_ptr<hdbitset<uSize>> pCopy = hdbitset_factory<uSize>::copy(pHdBitset);

			THEN("The copy is a new object which has the same content as the original") {
				REQUIRE_FALSE(pHdBitset == pCopy);
				REQUIRE(ops<uSize>::eq(pHdBitset, pCopy));
			}

			pHdBitset->flip();

			AND_THEN("The copy is not the same as a modified original") {
				REQUIRE_FALSE(ops<uSize>::eq(pHdBitset, pCopy));
			}

			pCopy->flip();

			AND_THEN("The similarly changed copy is the same as the modified") {
				REQUIRE(ops<uSize>::eq(pHdBitset, pCopy));
			}

		}

	}

}

TEST_CASE("hdbitset_factory::one()", "[hdbitset_factory]") {

	std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

	GIVEN("hdbitset_factory::one() is called") {

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdbitset_factory<10048>::one(pHdRandom);

		THEN("the generated object has the appropriate size") {

			REQUIRE(pHdbitset_10048->size_u() == 10048);

			AND_THEN("the generated object is all ones") {

				REQUIRE(pHdbitset_10048->all());

			}

		}

	}

}

TEST_CASE("hdbitset_factory::random()", "[hdbitset_factory]") {

	GIVEN("hdbitset_factory::random() is called") {

		std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdbitset_factory<10048>::random(pHdRandom);

		THEN("the generated object has the appropriate size") {

			REQUIRE(pHdbitset_10048->size_u() == 10048);

			AND_THEN("the generated object is not uniform") {

				REQUIRE_FALSE(pHdbitset_10048->uniform());

			}

			THEN("Number of set bits is usually within 10% range of midpoint") {
				double dRatio = (double)pHdbitset_10048->count_u() / (double)10048;
				Approx aRatioTarget = Approx(0.5).epsilon(0.05);
				CHECK(dRatio == aRatioTarget);
			}

		}

	}

}

TEST_CASE("hdbitset_factory::random(uShuffledWidth)", "[hdbitset_factory]") {

	GIVEN("hdbitset_factory::random(uShuffledWidth) is called") {

		std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);
		unsigned uShuffledWidth = 5000;

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdbitset_factory<10048>::random(uShuffledWidth, pHdRandom);

		THEN("the generated object has the appropriate size") {

			REQUIRE(pHdbitset_10048->size_u() == 10048);

			AND_THEN("the generated object is not uniform") {
				REQUIRE_FALSE(pHdbitset_10048->uniform());
			}

			THEN("Number of set bits is usually within 10% range of uShuffledWidth midpoint") {
				double dRatio = (double)pHdbitset_10048->count_u() / (double)uShuffledWidth;
				Approx aRatioTarget = Approx(0.5).epsilon(0.05);
				CHECK(dRatio == aRatioTarget);
			}

		}

	}

}

TEST_CASE("hdbitset_factory::raw()", "[hdbitset_factory]") {

	GIVEN("hdbitset_factory::raw() is called using various parameters") {

		std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

		std::shared_ptr<hdbitset<5>> pHdbitset_5 = hdbitset_factory<5>::raw(pHdRandom);
		std::shared_ptr<hdbitset<500>> pHdbitset_500 = hdbitset_factory<500>::raw(pHdRandom);
		std::shared_ptr<hdbitset<5000>> pHdbitset_5000 = hdbitset_factory<5000>::raw(pHdRandom);
		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdbitset_factory<10048>::raw(pHdRandom);

		THEN("size_u() for each case returns the correct size") {

			REQUIRE(pHdbitset_5->size_u() == 5);
			REQUIRE(pHdbitset_500->size_u() == 500);
			REQUIRE(pHdbitset_5000->size_u() == 5000);
			REQUIRE(pHdbitset_10048->size_u() == 10048);

		}

	}

}

TEST_CASE("hdbitset_factory::zero()", "[hdbitset_factory]") {

	std::shared_ptr<randengine> pHdRandom = randengine_factory::factory(0, 0, 0);

	GIVEN("hdbitset_factory::zero() is called") {

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdbitset_factory<10048>::zero(pHdRandom);

		THEN("the generated object has the appropriate size") {

			REQUIRE(pHdbitset_10048->size_u() == 10048);

			AND_THEN("the generated object is all zeros") {

				REQUIRE(pHdbitset_10048->none());

			}

		}

	}

}

