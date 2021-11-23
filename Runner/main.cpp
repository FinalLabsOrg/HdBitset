#include <iostream>
#include "../HD/factory.hpp"
#include "../HD/convenience.h"
#include "../HD/ops.hpp"
#include <chrono>
#include <ctime>

using namespace hyperdimensional;
using namespace std;

int main() {

	auto timenow =
		chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout << ctime(&timenow) << endl;

	srand(1211);

	SP_HdBitset pReferenceBitset = factory<uHdSize>::random();

	for (unsigned u = 0; u < 100000; u++) {
		SP_HdBitset pTrialBitset = factory<uHdSize>::random();
		if (ops<uHdSize>::eq(pReferenceBitset, pTrialBitset)) {
			std::cout << "Match " << u << std::endl;
		}
	}

	timenow =
		chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout << ctime(&timenow) << endl;

	return 0;

}