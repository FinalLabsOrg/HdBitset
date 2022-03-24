#pragma once
#include <random>

namespace hyperdimensional {

	class hdrandom
	{
	private:
		const unsigned uDiscard = 700000;
		std::uniform_int_distribution<unsigned> oDistribution;
		std::mt19937_64 oGenerator;
	public:
		inline hdrandom();
		inline unsigned rand();
	};

	/*
	* Seeding; see Nils_M's answer here:
	* https://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
	*/
	hdrandom::hdrandom() :
		oGenerator(std::random_device{}()),
		oDistribution(0, 1)
	{
		oGenerator.discard(uDiscard);
	}

	unsigned hdrandom::rand()
	{
		return oDistribution(oGenerator);
	}

}