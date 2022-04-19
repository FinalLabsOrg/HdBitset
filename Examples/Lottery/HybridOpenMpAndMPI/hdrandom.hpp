#pragma once
#include <algorithm>
#include <random>
#include "hdthread.hpp"

namespace hyperdimensional {

	class hdrandom
	{
	private:
		/*
		See Nils_M's & Brett Hale's answers here:
		https://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
		*/
		const unsigned uDiscard = 700000;
		std::uniform_int_distribution<unsigned> oDistribution;
		std::mt19937 oGenerator;
	public:
		inline hdrandom();
		inline unsigned rand();
		inline void seed();

	};

	hdrandom::hdrandom() :
		oDistribution(0, 1),
		oGenerator()
	{
		seed();
	}

	unsigned hdrandom::rand()
	{
		return oDistribution(oGenerator);
	}

	/*
	* Seeding.
	We want to avoid, by all means, generating the same sequence on multiple threads. 
	Thus the numeric thread id is included in the seed sequence.

	See also this explanation. We did not verify this, but discard() should achieve the same:
	https://simplecxx.github.io/2018/11/03/seed-mt19937.html
	*/
	inline void hdrandom::seed()
	{
		unsigned uThreadId = hdthread::getMyThreadId();
		std::random_device oRandomDevice;
		std::seed_seq oSeedSequence{ oRandomDevice(), uThreadId };
		oGenerator.seed(oSeedSequence);
		oGenerator.discard(uDiscard);
	}

}