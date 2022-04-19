#pragma once

#include <bitset>
#include <memory>
#include "hdbitset.hpp"

namespace hyperdimensional {

	template<unsigned uSize>
	class hdfactory
	{
	public:

		/*
		@return a smart pointer to the copy of the attribute
		*/
		static std::shared_ptr<hdbitset<uSize>> copy(std::shared_ptr<hdbitset<uSize>> pCopyable);

		/*
		@return a smart pointer to a newly created all-one hdbitset<uSize>
		*/
		static std::shared_ptr<hdbitset<uSize>> one();

		/*
		@return a smart pointer to a newly created and shuffled hdbitset<uSize>
		shuffling yields non-uniform bits(i.e. at least one is different from the others)
		*/
		static std::shared_ptr<hdbitset<uSize>> random();

		/*
		@return a smart pointer to a newly created and shuffled hdbitset<uSize>
		Shuffling is limited to uShuffledWidth; all other bits are kept 0
		Shuffling yields non-uniform bits(i.e. at least one is different from the others)
		*/
		static std::shared_ptr<hdbitset<uSize>> random(unsigned uShuffledWidth);

		/*
		@return a smart pointer to a newly created default hdbitset<uSize>
		*/
		static std::shared_ptr<hdbitset<uSize>> raw();

		/*
		@return a smart pointer to a newly created all-zero hdbitset<uSize>
		*/
		static std::shared_ptr<hdbitset<uSize>> zero();
	};

	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdfactory<uSize>::copy(std::shared_ptr<hdbitset<uSize>> pCopyable)
	{
		return std::make_shared<hdbitset<uSize>>(*pCopyable);
	}


	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdfactory<uSize>::one()
	{
		std::shared_ptr<hdbitset<uSize>> pGenerated = raw();
		pGenerated->set();
		return pGenerated;
	}

	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdfactory<uSize>::random()
	{
		std::shared_ptr<hdbitset<uSize>> pGenerated = raw();
		pGenerated->shuffle();
		return pGenerated;
	}

	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdfactory<uSize>::random(unsigned uShuffledWidth)
	{
		std::shared_ptr<hdbitset<uSize>> pGenerated = raw();
		pGenerated->shuffle(uShuffledWidth);
		return pGenerated;
	}

	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdfactory<uSize>::raw()
	{
		return std::make_shared<hdbitset<uSize>>();
	}

	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdfactory<uSize>::zero()
	{
		std::shared_ptr<hdbitset<uSize>> pGenerated = raw();
		pGenerated->reset();
		return pGenerated;
	}

}