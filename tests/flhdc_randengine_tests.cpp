#include "catch.hpp"
#include "../HD/flhdc.h"

using namespace flhdc;

TEST_CASE("randengine::rand()", "[randengine]") {

	GIVEN("randengine::rand() is called a large number of times") {

		randengine oHdRandom(0, 0, 0);

		unsigned uRandomCount = 50000;
		unsigned uOne = 0;
		for (unsigned u = 0; u < uRandomCount; u++) {
			uOne += oHdRandom.rand_bin();
		}
		
		THEN("Number of set bits is usually within 2% range of midpoint") {
			double dRatio = (double)uOne / (double) uRandomCount;
			Approx aRatioTarget = Approx(0.5).epsilon(0.01);
			CHECK(dRatio == aRatioTarget);
		}

	}

}

TEST_CASE("randengine::get_mt19937()", "[randengine]") {

	GIVEN("Two randengines") {

		p_randengine_t pRandEngine1 = randengine_factory::factory(1, 2, 3);
		p_randengine_t pRandEngine2 = randengine_factory::factory(1, 2, 3);

		THEN("They return different objects with the same appropriate type") {

			REQUIRE_FALSE(pRandEngine1->get_mt19937() == pRandEngine2->get_mt19937());
			REQUIRE(typeid(pRandEngine1->get_mt19937()) == typeid(p_mt19937_t));
			REQUIRE(typeid(pRandEngine2->get_mt19937()) == typeid(p_mt19937_t));

		}

	}

}