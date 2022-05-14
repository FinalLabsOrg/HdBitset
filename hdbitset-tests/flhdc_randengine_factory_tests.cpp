#include "catch.hpp"
#include "../HD/flhdc.h"

using namespace flhdc;

TEST_CASE("randengine_factory::factory()", "[randengine_factory]") {

	GIVEN("Several randengines from the factory") {

		p_randengine_t pRandEngine1 = randengine_factory::factory(0, 1, 2);
		p_randengine_t pRandEngine2 = randengine_factory::factory(0, 1, 2);
		p_randengine_t pRandEngine3 = randengine_factory::factory(0, 1, 2);
		p_randengine_t pRandEngine4 = randengine_factory::factory(0, 1, 2);

		THEN("They are different") {
			REQUIRE(pRandEngine1!= pRandEngine2);
			REQUIRE(pRandEngine1 != pRandEngine3);
			REQUIRE(pRandEngine1 != pRandEngine4);
			REQUIRE(pRandEngine2 != pRandEngine3);
			REQUIRE(pRandEngine2 != pRandEngine4);
			REQUIRE(pRandEngine3 != pRandEngine4);
		}

	}

}