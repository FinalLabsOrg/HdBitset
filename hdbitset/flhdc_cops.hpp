#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include "flhdc_hdbitset.hpp"
#include "flhdc_hdbitset_factory.hpp"
#include "flhdc_ops.hpp"
#include "flhdc_types.h"

namespace flhdc {

	/*
	Collection Operations.
	hdbitset operations involving a group of bitsets stored as vectors, maps, etc. are placed here.
	Pronounced cee-ops.
	Bad boys, bad boys whatcha gonna do?
	*/
	template<const unsigned cuSize>
	class cops
	{
	private:
		typedef hdbitset<cuSize> hdbitset_t;
		typedef std::shared_ptr<hdbitset_t> p_hdbitset_t;
		typedef p_collection_t<hdbitset_collection_key_t, p_hdbitset_t> p_hdbitset_collection_t;

	public:
		/*
		@return a smart pointer to a new hdbitset which is the best unified representation of the input pCollection.
		*/
		static p_hdbitset_t bundle(const p_hdbitset_collection_t pCollection);

		/*
		@return the index of the pCollection element that has the smallest hamming distance from pComparable.
		If several elements have the same best distance, the one encountered first is returned.
		*/
		static hdbitset_collection_key_t get_closest_key(const p_hdbitset_collection_t pCollection, p_hdbitset_t pComparable);

		/*
		@return a smart pointer to a new hdbitset with 1s and 0s set according to the majority rule using the input pCollection.
		In the case of an even number of inputs, an additional new random vector is added to the majority using the random generator obtained from the 0th element of the map.
		@throws std::logic_error in the case of empty maps
		*/
		static p_hdbitset_t majority(const p_hdbitset_collection_t pCollection);
	};


	template<const unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> cops<cuSize>::bundle(const p_hdbitset_collection_t pCollection)
	{
		return majority(pCollection);
	}

	template<unsigned cuSize>
	inline hdbitset_collection_key_t cops<cuSize>::get_closest_key(const p_hdbitset_collection_t pCollection, p_hdbitset_t pComparable)
	{

		if (!static_cast<unsigned>(pCollection->size())) {
			throw std::logic_error("Map for get_closest_key is empty.");
		}

		// worst case is a hamming distance of cuSize;
		unsigned uBestHamming = cuSize;

		// use the first element as the dummy initial best match
		// this is to bypass a convoluted iterator syntax with auto
		auto itBest = pCollection->begin();

		for (auto it = pCollection->begin(); it != pCollection->end(); it++) {

			unsigned uCurrentHamming = ops<cuSize>::hamming(it->second, pComparable);
			
			// stop search if hamming distance == 0
			if (!uCurrentHamming) {
				return it->first;
			}
			else if(uCurrentHamming < uBestHamming) {
				itBest = it;
				uBestHamming = uCurrentHamming;
			}
		}

		return itBest->first;
		
	}

	template<const unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> cops<cuSize>::majority(const p_hdbitset_collection_t pCollection)
	{
		unsigned uMapSize = static_cast<unsigned>(pCollection->size());

		if (!uMapSize) {
			throw std::logic_error("Map for majority is empty.");
		}

		p_randengine_t pGenerator = pCollection->begin()->second->get_randengine();
		p_hdbitset_t pMajority = hdbitset_factory<cuSize>::zero(pGenerator);

		unsigned uCountOfBitsThatAreSet, uCountRequiredForMajority;

		bool bMapSizeIsOdd = uMapSize % 2;
		if (bMapSizeIsOdd)
			uCountRequiredForMajority = uMapSize / 2;
		else
			uCountRequiredForMajority = (uMapSize + 1u) / 2; // accounting for the extra random bit

		/*
		@todo should break out of the loop also when the count of unset bits have exceeded uCountRequiredForMajority
		*/
		for (unsigned uBitByBit = 0; uBitByBit < cuSize; uBitByBit++) {

			// adding a random bit in the case of even bitsets
			uCountOfBitsThatAreSet = bMapSizeIsOdd ? 0 : pGenerator->rand_bin();

			for (auto it = pCollection->begin(); it != pCollection->end(); it++) {

				uCountOfBitsThatAreSet += it->second->operator[](uBitByBit); // no need for range checking here

				if (uCountOfBitsThatAreSet > uCountRequiredForMajority)
					break;

			}

			pMajority->operator[](uBitByBit) = uCountOfBitsThatAreSet > uCountRequiredForMajority;

		}

		return pMajority;

	}

}