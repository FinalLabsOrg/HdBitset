#pragma once
#include <iostream>
#include <memory>
#include <omp.h>
#include "../HD/flhdc.h"

using namespace flhdc;

class omp_tests
{
private:

	/*
	Stochastic test to see if the generator of each thread is different from the other threads.
	*/
	static bool testHdRandom_randgenerator_factory();

	/*
	Stochastic test to see if the threads generate differring random values;
	This is a statistical test and might fail on occasion (very-very rarely though).
	*/
	static bool testHdRandom_rand_bin();

public:
	/*
	OMP Tests.
	@return True if the test fails.
	*/
	static bool testmain(bool bRunOmpTests);

};


bool omp_tests::testHdRandom_randgenerator_factory() {

	unsigned uMaxThreads = omp_get_max_threads();
	std::vector<p_randengine_t> oRandengineCollector(uMaxThreads, 0);
	std::vector<p_mt19937_t> oMtCollector(uMaxThreads, 0);
	unsigned uActualThreadCount = 0;

#pragma omp parallel reduction(+: uActualThreadCount)
	{
		unsigned uThisThreadId = omp_get_thread_num();

		p_randengine_t pHdRandom = randengine_factory::factory(0, uThisThreadId, 0);

		oRandengineCollector[uThisThreadId] = pHdRandom;
		oMtCollector[uThisThreadId] = pHdRandom->get_mt19937();
		uActualThreadCount = 1;

	}

	bool bAllReUnique = true;
	{

		std::map<p_randengine_t, bool> aUniquality;
		for (unsigned u = 0; u < uMaxThreads; u++) {
			auto iter = aUniquality.find(oRandengineCollector[u]);
			if (iter != aUniquality.end()) {
				bAllReUnique = false;
				break;
			}
			else {
				aUniquality[oRandengineCollector[u]] = true;
			}
		}

	}

	bool bAllMtUnique = true;
	{

		std::map<p_mt19937_t, bool> aUniquality;
		for (unsigned u = 0; u < uMaxThreads; u++) {
			auto iter = aUniquality.find(oMtCollector[u]);
			if (iter != aUniquality.end()) {
				bAllMtUnique = false;
				break;
			}
			else {
				aUniquality[oMtCollector[u]] = true;
			}
		}

	}

	bool bTestPassed =
		(uActualThreadCount == uMaxThreads)
		&&
		bAllReUnique
		&&
		bAllMtUnique;

	if (!bTestPassed) {
		std::cout << "testHdRandom_randgenerator_factory failed." << std::endl;
		std::cout << "Expected uActualThreadCount == uMaxThreads:" << uActualThreadCount << std::endl;
		std::cout << "uMaxThreads = " << uMaxThreads << std::endl;
		std::cout << "uActualThreadCount = " << uActualThreadCount << std::endl;
		std::cout << "Expected bAllReUnique == true:" << std::endl;
		std::cout << "bAllUnique = " << bAllReUnique << std::endl;
		std::cout << "Expected bAllMtUnique == true:" << std::endl;
		std::cout << "bAllMtUnique = " << bAllReUnique << std::endl;
	}

	return !bTestPassed;

}


bool omp_tests::testHdRandom_rand_bin() {

	typedef unsigned long long wide_unsigned_t;

	unsigned uMaxThreads = omp_get_max_threads();
	std::vector<wide_unsigned_t> oCollector(uMaxThreads, 0);
	unsigned uActualThreadCount = 0;

#pragma omp parallel reduction(+: uActualThreadCount)
	{
		unsigned uWideLength = sizeof(wide_unsigned_t) * 8;
		unsigned uThisThreadId = omp_get_thread_num();
		p_randengine_t pHdRandom = randengine_factory::factory(0, uThisThreadId, 0);
		wide_unsigned_t uGenerated = 0;

		for (unsigned uBitposition = 0; uBitposition < uWideLength; uBitposition++) {
			unsigned uMask = 1 << uBitposition;
			unsigned uRand = pHdRandom->rand_bin();
			uGenerated = ((uGenerated & ~uMask) | (uRand << uBitposition));
		}

		oCollector[uThisThreadId] = uGenerated;
		uActualThreadCount = 1;

	}

	std::map<wide_unsigned_t, bool> aUniquality;
	bool bAllUnique = true;
	for (unsigned u = 0; u < uMaxThreads; u++) {
		auto iter = aUniquality.find(oCollector[u]);
		if (iter != aUniquality.end()) {
			bAllUnique = false;
			break;
		}
		else {
			aUniquality[oCollector[u]] = true;
		}
	}

	bool bTestPassed =
		(uActualThreadCount == uMaxThreads)
		&&
		bAllUnique;

	if (!bTestPassed) {
		std::cout << "testHdRandomOMP failed." << std::endl;
		std::cout << "Expected uActualThreadCount == uMaxThreads:" << uActualThreadCount << std::endl;
		std::cout << "uMaxThreads = " << uMaxThreads << std::endl;
		std::cout << "uActualThreadCount = " << uActualThreadCount << std::endl;
		std::cout << "Expected bAllUnique == true:" << std::endl;
		std::cout << "bAllUnique = " << bAllUnique << std::endl;
		std::cout << "This is a statistical test and might fail on occasion (very-very rarely though)." << std::endl;
	}

	return !bTestPassed;

}

inline bool omp_tests::testmain(bool bRunOmpTests)
{
	bool bTestFailed = false;
	if (bRunOmpTests) {
		std::cout << "Running Omp tests." << std::endl;
		bTestFailed |= testHdRandom_randgenerator_factory();
		bTestFailed |= testHdRandom_rand_bin();
	}
	else {
		std::cout << "Skipping Omp tests." << std::endl;
	}
	return bTestFailed;
}
