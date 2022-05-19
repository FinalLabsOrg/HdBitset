#pragma once
#include "flhdc_randengine.hpp"

namespace flhdc {

	class randengine_factory
	{
	public:
		static p_randengine_t factory(unsigned uMpiThreadId, unsigned uOmpThreadId, unsigned uCustomSeed);
	};

	inline p_randengine_t randengine_factory::factory(unsigned uMpiThreadId, unsigned uOmpThreadId, unsigned uCustomSeed)
	{
		return std::make_shared<randengine>(uMpiThreadId, uOmpThreadId, uCustomSeed);
	}

}