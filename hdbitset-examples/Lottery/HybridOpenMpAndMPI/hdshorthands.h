#pragma once
#include <memory>

// these might depend on storage size; assuming 64 bits
const unsigned uHd256	= 64 * 4;
const unsigned uHd10048 = 64 * 157;

namespace hyperdimensional {
	template<unsigned uHd10048> class hdbitset;
	typedef hyperdimensional::hdbitset<uHd10048> HdBitset;
}

typedef std::shared_ptr<hyperdimensional::HdBitset> SP_HdBitset;
