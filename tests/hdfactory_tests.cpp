#include "catch.hpp"
#include "../HD/hdfactory.hpp"
#include "../HD/hdops.hpp"

using namespace hyperdimensional;

TEST_CASE("hdfactory::copy()", "[hdfactory]") {

	srand(0);

	const unsigned uSize = 10048;

	GIVEN("hdfactory::random() is just called") {

		std::shared_ptr<hdbitset<uSize>> pHdBitset = hdfactory<uSize>::random();

		WHEN("Copying the thus created bitset") {

			std::shared_ptr<hdbitset<uSize>> pCopy = hdfactory<uSize>::copy(pHdBitset);

			THEN("The copy is a new object which has the same content as the original") {
				REQUIRE_FALSE(pHdBitset == pCopy);
				REQUIRE(hdops<uSize>::eq(pHdBitset, pCopy));
			}

			pHdBitset->flip();

			AND_THEN("The copy is not the same as a modified original") {
				REQUIRE_FALSE(hdops<uSize>::eq(pHdBitset, pCopy));
			}

			pCopy->flip();

			AND_THEN("The similarly changed copy is the same as the modified") {
				REQUIRE(hdops<uSize>::eq(pHdBitset, pCopy));
			}

		}

	}

}

TEST_CASE("hdfactory::one()", "[hdfactory]") {

	GIVEN("hdfactory::one() is called") {

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdfactory<10048>::one();

		THEN("the generated object has the appropriate size") {

			REQUIRE(pHdbitset_10048->size_u() == 10048);

			AND_THEN("the generated object is all ones") {

				REQUIRE(pHdbitset_10048->all());

			}

		}

	}

}

TEST_CASE("hdfactory::random()", "[hdfactory]") {

	GIVEN("hdfactory::random() is called") {

		srand(0);

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdfactory<10048>::random();

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

TEST_CASE("hdfactory::raw()", "[hdfactory]") {

	GIVEN("hdfactory::raw() is called using various parameters") {

		std::shared_ptr<hdbitset<5>> pHdbitset_5 = hdfactory<5>::raw();
		std::shared_ptr<hdbitset<500>> pHdbitset_500 = hdfactory<500>::raw();
		std::shared_ptr<hdbitset<5000>> pHdbitset_5000 = hdfactory<5000>::raw();
		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdfactory<10048>::raw();

		THEN("size_u() for each case returns the correct size") {

			REQUIRE(pHdbitset_5->size_u() == 5);
			REQUIRE(pHdbitset_500->size_u() == 500);
			REQUIRE(pHdbitset_5000->size_u() == 5000);
			REQUIRE(pHdbitset_10048->size_u() == 10048);

		}

	}

}

TEST_CASE("hdfactory::zero()", "[hdfactory]") {

	GIVEN("hdfactory::zero() is called") {

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = hdfactory<10048>::zero();

		THEN("the generated object has the appropriate size") {

			REQUIRE(pHdbitset_10048->size_u() == 10048);

			AND_THEN("the generated object is all zeros") {

				REQUIRE(pHdbitset_10048->none());

			}

		}

	}

}

