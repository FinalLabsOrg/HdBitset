#pragma once
#include <algorithm>
#include <random>
#include "types.h"

namespace flhdc {

	class randengine
	{
	private:

		/*
		See Nils_M's & Brett Hale's answers here:
		https://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
		*/
		const unsigned uDiscard = 700000;

		std::uniform_int_distribution<unsigned> oDistribution;
		p_mt19937_t pGenerator;
	public:
		/*
		Construction taking arguments for seeding.
		*/
		inline randengine(unsigned uMpiThreadId, unsigned uOmpThreadId, unsigned uCustomSeed);

		/*
		@return shared pointer to the seeded mt19937 random generator
		*/
		p_mt19937_t get_mt19937() const;

		/*
		@return random value using the binary distribution
		*/
		inline unsigned rand_bin();

		/*
		Seeds the mt19937 generator and discards a larger batch of initial numbers
		*/
		inline void seed(unsigned uMpiThreadId, unsigned uOmpThreadId, unsigned uCustomSeed);
	};

	randengine::randengine(unsigned uMpiThreadId, unsigned uOmpThreadId, unsigned uCustomSeed) :
		oDistribution(0, 1),
		pGenerator(std::make_shared<std::mt19937>())
	{
		seed(uMpiThreadId, uOmpThreadId, uCustomSeed);
	}

	inline p_mt19937_t randengine::get_mt19937() const
	{
		return pGenerator;
	}

	unsigned randengine::rand_bin()
	{
		return oDistribution(*pGenerator);
	}

	/*
	* Seeding.
	We want to avoid, by all means, generating the same sequence on multiple threads. 
	Thus the numeric thread ids are included in the seed sequence.

	See also this explanation. We did not verify this, but discard() should achieve the same:
	https://simplecxx.github.io/2018/11/03/seed-mt19937.html
	*/
	inline void randengine::seed(unsigned uMpiThreadId, unsigned uOmpThreadId, unsigned uCustomSeed)
	{
		std::random_device oRandomDevice;
		std::seed_seq oSeedSequence{ oRandomDevice(), uMpiThreadId, uOmpThreadId, uCustomSeed };
		pGenerator->seed(oSeedSequence);
		pGenerator->discard(uDiscard);
	}

}