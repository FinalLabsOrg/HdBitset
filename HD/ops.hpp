#pragma once

#include <memory>
#include "hdbitset.hpp"

namespace hyperdimensional {

	template<unsigned uSize>
	class ops
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
		Returns false if the contens of the two underlying bitsets are identical
		*/
		static bool noteq(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd);
	};

	template<unsigned uSize>
	inline bool ops<uSize>::eq(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd)
	{
		return (*pLeftHd) == (*pRightHd);
	}

	template<unsigned uSize>
	inline unsigned ops<uSize>::hamming(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd)
	{
		return static_cast<unsigned>( ((*pLeftHd) ^ (*pRightHd)).count() );
	}

	template<unsigned uSize>
	inline bool ops<uSize>::noteq(const std::shared_ptr<hdbitset<uSize>> pLeftHd, const std::shared_ptr<hdbitset<uSize>> pRightHd)
	{
		return !eq(pLeftHd, pRightHd);
	}

}
