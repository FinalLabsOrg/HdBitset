#pragma once

#include <memory>
#include "hdbitset.hpp"

/*
uHdSize = 10048 (==157*64)
*/
const unsigned uHdSize = 10048u;

// predefines
typedef hyperdimensional::hdbitset<uHdSize> HdBitset;
typedef std::shared_ptr<HdBitset> SP_HdBitset;
