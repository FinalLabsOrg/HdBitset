#pragma once

#include "hdbitset.hpp"
#include "factory.hpp"
#include <memory>
#include <map>

namespace hyperdimensional {

	template<unsigned uSize>
	class hdops
	{
	public:

		/*
		@return SP_HdBitset with 1s and 0s set according to the majority rule using the input pMap
		In the case of an even number of inputs, an additional random vector is addded to map
		*/
		static std::shared_ptr<hdbitset<uSize>> majority(const std::shared_ptr<std::map<unsigned, std::shared_ptr<hdbitset<uSize>>>> pMap);

		/*
		pLeft XOR pRight
		@return SP_HdBitset orthogonal to both pLeft and pRight
		*/
		static std::shared_ptr<hdbitset<uSize>> XOR(const std::shared_ptr<hdbitset<uSize>> pLeft, const std::shared_ptr<hdbitset<uSize>> pRight);

	};


	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdops<uSize>::majority(const std::shared_ptr<std::map<unsigned, std::shared_ptr<hdbitset<uSize>>>> pMap)
	{
		
		unsigned uCountOfBitsThatAreSet, uCountRequiredForMajority;

		unsigned uMapSize = static_cast<unsigned>(pMap->size());
		bool bMapSizeIsOdd = uMapSize % 2;

		std::shared_ptr<hdbitset<uSize>> pMajority = factory<uSize>::zero();

		if(bMapSizeIsOdd) 
			uCountRequiredForMajority = uMapSize / 2;
		else
			uCountRequiredForMajority = (uMapSize + 1u) / 2; // accounting for the extra random bit

		/*
		* @todo OpenMP 'parallel for' candidate location depending on speed
		* most likely does not make sense
		* static schedule should be fine assuming random bitsets
		*/
		for (unsigned u = 0; u < uSize; u++) {

			// adding a random bit in the case of even bitsets
			uCountOfBitsThatAreSet = bMapSizeIsOdd ? 0 : (rand() % 2);

			for (auto it = pMap->begin(); it != pMap->end(); it++) {
				uCountOfBitsThatAreSet += it->second->operator[](u); // no need for range checking here
				if (uCountOfBitsThatAreSet > uCountRequiredForMajority) 
					break;
			}
			pMajority->operator[](u) = uCountOfBitsThatAreSet > uCountRequiredForMajority;

		}

		return pMajority;

	}


	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdops<uSize>::XOR(const std::shared_ptr<hdbitset<uSize>> pLeft, const std::shared_ptr<hdbitset<uSize>> pRight)
	{
		return std::make_shared<hdbitset<uSize>>((*pLeft) ^ (*pRight));
	}

}
