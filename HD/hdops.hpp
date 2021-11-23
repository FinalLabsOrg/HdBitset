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
		
		std::shared_ptr<hdbitset<uSize>> pMajority = factory<uSize>::zero();

		bool bMapSizeIsOdd = pMap->size() % 2;
		unsigned uTrueBitCount;
		unsigned uExpectedMinimum;

		if(bMapSizeIsOdd) 
			uExpectedMinimum = static_cast<unsigned>(pMap->size()) / 2;
		else
			uExpectedMinimum = (static_cast<unsigned>(pMap->size()) + 1u) / 2;

		for (unsigned u = 0; u < uSize; u++) {

			uTrueBitCount = bMapSizeIsOdd ? 0 : (rand() % 2);

			for (auto it = pMap->begin(); it != pMap->end(); it++) {
				uTrueBitCount += it->second->operator[](u);
				if (uTrueBitCount > uExpectedMinimum) break;
			}
			pMajority->operator[](u) = uTrueBitCount > uExpectedMinimum;

		}

		return pMajority;

	}


	template<unsigned uSize>
	inline std::shared_ptr<hdbitset<uSize>> hdops<uSize>::XOR(const std::shared_ptr<hdbitset<uSize>> pLeft, const std::shared_ptr<hdbitset<uSize>> pRight)
	{
		return std::make_shared<hdbitset<uSize>>((*pLeft) ^ (*pRight));
	}

}
