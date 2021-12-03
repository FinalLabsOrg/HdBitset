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
		Returns a smart pointer to the copy of the attribute
		*/
		static std::shared_ptr<hdbitset<uSize>> copy(std::shared_ptr<hdbitset<uSize>> pCopyable);

		/*
		Returns a smart pointer to a newly created all-one hdbitset<uSize>
		*/
		static std::shared_ptr<hdbitset<uSize>> one();

		/*
		Returns a smart pointer to a newly created and shuffled hdbitset<uSize>
		*/
		static std::shared_ptr<hdbitset<uSize>> random();

		/*
		Returns a smart pointer to a newly created default hdbitset<uSize>
		*/
		static std::shared_ptr<hdbitset<uSize>> raw();

		/*
		Returns a smart pointer to a newly created all-zero hdbitset<uSize>
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