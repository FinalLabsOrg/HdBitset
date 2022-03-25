#include "catch.hpp"
#include "../HD/hdrandom.hpp"

using namespace hyperdimensional;

TEST_CASE("hdrandom::rand()", "[hdrandom]") {

	GIVEN("hdrandom::rand() is called a large number of times") {

		hdrandom oHdRandom;

		unsigned uRandomCount = 50000;
		unsigned uOne = 0;
		for (unsigned u = 0; u < uRandomCount; u++) {
			uOne += oHdRandom.rand();
		}
		
		THEN("Number of set bits is usually within 10% range of midpoint") {
			double dRatio = (double)uOne / (double) uRandomCount;
			Approx aRatioTarget = Approx(0.5).epsilon(0.05);
			CHECK(dRatio == aRatioTarget);
		}

	}

}