#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include "hdbitset.hpp"
#include "hdbitset_factory.hpp"
#include "types.h"

namespace flhdc {

	template<const unsigned cuSize>
	class ops
	{
	private:
		typedef hdbitset<cuSize> hdbitset_t;
		typedef std::shared_ptr<hdbitset_t> p_hdbitset_t;

	public:

		/*
		@return a smart pointer to a new hdbitset in which the hdbitset content of pValue is bound to the hdbitset content of pMarker.
		*/
		static p_hdbitset_t bind(const p_hdbitset_t pMarker, const p_hdbitset_t pValue);

		/*
		@return a smart pointer to a new hdbitset 
		in which the first cuCut bits are from pHdBitset1 
		and cuSize-cuCut bits are from pHdBitset2
		*/
		static p_hdbitset_t crossover(const p_hdbitset_t pHdBitset1, const p_hdbitset_t pHdBitset2, const unsigned cuCut);

		/*
		@return true if the contens of the two underlying bitsets are identical
		*/
		static bool eq(const p_hdbitset_t pLeftHd, const p_hdbitset_t pRightHd);

		/*
		@return the Hamming distance between the two bitsets.
		*/
		static unsigned hamming(const p_hdbitset_t pLeftHd, const p_hdbitset_t pRightHd);

		/*
		@return false if the contens of the two underlying bitsets are identical.
		*/
		static bool noteq(const p_hdbitset_t pLeftHd, const p_hdbitset_t pRightHd);

		/*
		Recovers (unbinds, releases) the hdbitset content of pValue using pMarker and pBound, assuming pBound = bound(pMarker, pValue).
		@return a smart pointer to a new hdbitset whose value reflects the hdbitset content of pValue.
		*/
		static p_hdbitset_t release(const p_hdbitset_t pMarker, const p_hdbitset_t pBound);

		/*
		Extends the current sequence, overwriting the first attribute. If no sequence exists yet, use the first element in the place of pCurrentSequence.
		*/
		static void sequence_extend(p_hdbitset_t & pCurrentSequence, const p_hdbitset_t pExtension);

		/*
		pLeft XOR pRight
		@return a smart pointer to a hdbitset orthogonal to both pLeft and pRight
		*/
		static p_hdbitset_t XOR(const p_hdbitset_t pLeft, const p_hdbitset_t pRight);

		/*
		pLeft = pLeft XOR pRight; no new bitset is created
		*/
		static void XOR_inplace(p_hdbitset_t & pLeft, const p_hdbitset_t pRight);

	};


	template<const unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> ops<cuSize>::bind(const p_hdbitset_t pMarker, const p_hdbitset_t pValue)
	{
		return XOR(pMarker, pValue);
	}

	template<unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> ops<cuSize>::crossover(const p_hdbitset_t pHdBitset1, const p_hdbitset_t pHdBitset2, const unsigned cuCut)
	{

		if (cuCut>cuSize) {
			throw std::logic_error("Invalid cut position for crossover.");
		}

		p_hdbitset_t pCrossover = hdbitset_factory<cuSize>::raw(pHdBitset1->get_randengine());
		for (unsigned u = 0; u < cuCut; u++) {
			pCrossover->operator[](u) = pHdBitset1->operator[](u);
		}
		for (unsigned u = cuCut; u < cuSize; u++) {
			pCrossover->operator[](u) = pHdBitset2->operator[](u);
		}
		return pCrossover;
	}

	template<const unsigned cuSize>
	inline bool ops<cuSize>::eq(const p_hdbitset_t pLeftHd, const p_hdbitset_t pRightHd)
	{
		return (*pLeftHd) == (*pRightHd);
	}

	template<const unsigned cuSize>
	inline unsigned ops<cuSize>::hamming(const p_hdbitset_t pLeftHd, const p_hdbitset_t pRightHd)
	{
		return XOR(pLeftHd, pRightHd)->count_u();
	}

	template<const unsigned cuSize>
	inline bool ops<cuSize>::noteq(const p_hdbitset_t pLeftHd, const p_hdbitset_t pRightHd)
	{
		return !eq(pLeftHd, pRightHd);
	}

	template<const unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> ops<cuSize>::release(const p_hdbitset_t pMarker, const p_hdbitset_t pBound)
	{
		return XOR(pMarker, pBound);
	}

	template<const unsigned cuSize>
	inline void ops<cuSize>::sequence_extend(p_hdbitset_t & pCurrentSequence, const p_hdbitset_t pExtension)
	{
		pCurrentSequence->rotl();
		XOR_inplace(pCurrentSequence, pExtension);
	}

	template<const unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> ops<cuSize>::XOR(const p_hdbitset_t pLeft, const p_hdbitset_t pRight)
	{
		return std::make_shared<hdbitset_t>((*pLeft) ^ (*pRight), pLeft->get_randengine());
	}

	template<const unsigned cuSize>
	inline void ops<cuSize>::XOR_inplace(p_hdbitset_t& pLeft, const p_hdbitset_t pRight)
	{
		(*pLeft) ^= (*pRight);
	}

}
