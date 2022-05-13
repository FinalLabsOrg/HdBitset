#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <iostream>
#include "../HD/flhdc.h"
#include "omp_tests.hpp"

constexpr bool bRunCatchTests = true;
constexpr bool bRunOmpTests = _OPENMP;

bool catch_testmain(int argc, char* const argv[], bool bRunCatchTests) {

	bool bTestFailed = false;

	if (bRunCatchTests) {
		std::cout << "Running Catch tests." << std::endl;
		bTestFailed |= Catch::Session().run(argc, argv);
	}
	else {
		std::cout << "Skipping Catch tests." << std::endl;
	}

	return bTestFailed;

}

int main(int argc, char* const argv[])
{
	bool bTestFailed = false;
	
	bTestFailed |= catch_testmain(argc, argv, bRunCatchTests);
	bTestFailed |= omp_tests::testmain(bRunOmpTests);

	std::cout << std::endl << (bTestFailed ? "Some" : "No") << " tests failed." << std::endl;
	return bTestFailed;

}