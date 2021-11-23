#include "catch.hpp"
#include "../HD/factory.hpp"
#include "../HD/ops.hpp"

using namespace hyperdimensional;

TEST_CASE("factory::copy()", "[factory]") {

	srand(0);

	const unsigned uSize = 10048;

	GIVEN("factory::random() is just called") {

		std::shared_ptr<hdbitset<uSize>> pHdBitset = factory<uSize>::random();

		WHEN("Copying the thus created bitset") {

			std::shared_ptr<hdbitset<uSize>> pCopy = factory<uSize>::copy(pHdBitset);

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

TEST_CASE("factory::one()", "[factory]") {

	GIVEN("factory::one() is called") {

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = factory<10048>::one();

		THEN("the generated object has the appropriate size") {

			REQUIRE(pHdbitset_10048->size_u() == 10048);

			AND_THEN("the generated object is all ones") {

				REQUIRE(pHdbitset_10048->all());

			}

		}

	}

}

TEST_CASE("factory::random()", "[factory]") {

	GIVEN("factory::random() is called") {

		srand(0);

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = factory<10048>::random();

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

TEST_CASE("factory::raw()", "[factory]") {

	GIVEN("factory::raw() is called using various parameters") {

		std::shared_ptr<hdbitset<5>> pHdbitset_5 = factory<5>::raw();
		std::shared_ptr<hdbitset<500>> pHdbitset_500 = factory<500>::raw();
		std::shared_ptr<hdbitset<5000>> pHdbitset_5000 = factory<5000>::raw();
		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = factory<10048>::raw();

		THEN("size_u() for each case returns the correct size") {

			REQUIRE(pHdbitset_5->size_u() == 5);
			REQUIRE(pHdbitset_500->size_u() == 500);
			REQUIRE(pHdbitset_5000->size_u() == 5000);
			REQUIRE(pHdbitset_10048->size_u() == 10048);

		}

	}

}

TEST_CASE("factory::zero()", "[factory]") {

	GIVEN("factory::zero() is called") {

		std::shared_ptr<hdbitset<10048>> pHdbitset_10048 = factory<10048>::zero();

		THEN("the generated object has the appropriate size") {

			REQUIRE(pHdbitset_10048->size_u() == 10048);

			AND_THEN("the generated object is all zeros") {

				REQUIRE(pHdbitset_10048->none());

			}

		}

	}

}

