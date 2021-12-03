#pragma once

#include <map>
#include <memory>
#include "hdbitset.hpp"
#include "hdfactory.hpp"

namespace hyperdimensional {

	template<unsigned uSize>
	class hdops
	{
	public:

		/*
		Returns true if the contens of the two underlying bitsets are identical
		*/
		static bool eq(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd);

		/*
		Returns the Hamming distance
		*/
		static unsigned hamming(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd);

		/*
		@return SP_HdBitset with 1s and 0s set according to the majority rule using the input pMap
		In the case of an even number of inputs, an additional random vector is addded to map
		*/
		static std::shared_ptr<hdbitset<uSize>> majority(const std::shared_ptr<std::map<unsigned, std::shared_ptr<hdbitset<uSize>>>> pMap);

		/*
		Returns false if the contens of the two underlying bitsets are identical
		*/
		static bool noteq(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd);

		/*
		pLeft XOR pRight
		@return SP_HdBitset orthogonal to both pLeft and pRight
		*/
		static std::shared_ptr<hdbitset<uSize>> XOR(const std::shared_ptr<hdbitset<uSize>> pLeft, const std::shared_ptr<hdbitset<uSize>> pRight);

	};


	template<unsigned uSize>
	inline bool hdops<uSize>::eq(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd)
	{
		return (*pLeftHd) == (*pRightHd);
	}

	template<unsigned uSize>
	inline unsigned hdops<uSize>::hamming(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd)
	{
		return hdops<uSize>::XOR(pLeftHd, pRightHd)->count_u();
	}

	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdops<uSize>::majority(const std::shared_ptr<std::map<unsigned, std::shared_ptr<hdbitset<uSize>>>> pMap)
	{
		
		unsigned uCountOfBitsThatAreSet, uCountRequiredForMajority;

		unsigned uMapSize = static_cast<unsigned>(pMap->size());
		bool bMapSizeIsOdd = uMapSize % 2;

		std::shared_ptr<hdbitset<uSize>> pMajority = hdfactory<uSize>::zero();

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
	inline bool hdops<uSize>::noteq(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd)
	{
		return !eq(pLeftHd, pRightHd);
	}

	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdops<uSize>::XOR(const std::shared_ptr<hdbitset<uSize>> pLeft, const std::shared_ptr<hdbitset<uSize>> pRight)
	{
		return std::make_shared<hdbitset<uSize>>((*pLeft) ^ (*pRight));
	}

}
