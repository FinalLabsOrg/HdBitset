#pragma once

#include <bitset>
#include <memory>
#include "flhdc_hdbitset.hpp"
#include "flhdc_randengine.hpp"

namespace flhdc {

	template<const unsigned cuSize>
	class hdbitset_factory
	{
	private:
		typedef hdbitset<cuSize> hdbitset_t;
		typedef std::shared_ptr<hdbitset_t> p_hdbitset_t;

	public:

		/*
		@return a smart pointer to the copy of the attribute
		*/
		static p_hdbitset_t copy(p_hdbitset_t pCopyable);

		/*
		@return a smart pointer to a newly created all-one hdbitset<cuSize>
		*/
		static p_hdbitset_t one(p_randengine_t pRandomGenerator);

		/*
		@return a smart pointer to a newly created and shuffled hdbitset<cuSize>
		shuffling yields non-uniform bits(i.e. at least one is different from the others)
		*/
		static p_hdbitset_t random(p_randengine_t pRandomGenerator);

		/*
		@return a smart pointer to a newly created and shuffled hdbitset<cuSize>
		Shuffling is limited to uShuffledWidth; all other bits are kept 0
		Shuffling yields non-uniform bits(i.e. at least one is different from the others)
		*/
		static p_hdbitset_t random(unsigned uShuffledWidth, p_randengine_t pRandomGenerator);

		/*
		@return a smart pointer to a newly created default hdbitset<cuSize>
		*/
		static p_hdbitset_t raw(p_randengine_t pRandomGenerator);

		/*
		@return a smart pointer to a newly created all-zero hdbitset<cuSize>
		*/
		static p_hdbitset_t zero(p_randengine_t pRandomGenerator);
	};

	template<unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> hdbitset_factory<cuSize>::copy(p_hdbitset_t pCopyable)
	{
		return std::make_shared<hdbitset_t>(*pCopyable, pCopyable->get_randengine());
	}


	template<unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> hdbitset_factory<cuSize>::one(p_randengine_t pRandomGenerator)
	{
		p_hdbitset_t pGenerated = raw(pRandomGenerator);
		pGenerated->set();
		return pGenerated;
	}

	template<unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> hdbitset_factory<cuSize>::random(p_randengine_t pRandomGenerator)
	{
		p_hdbitset_t pGenerated = raw(pRandomGenerator);
		pGenerated->shuffle();
		return pGenerated;
	}

	template<unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> hdbitset_factory<cuSize>::random(unsigned uShuffledWidth, p_randengine_t pRandomGenerator)
	{
		p_hdbitset_t pGenerated = raw(pRandomGenerator);
		pGenerated->shuffle(uShuffledWidth);
		return pGenerated;
	}

	template<unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> hdbitset_factory<cuSize>::raw(p_randengine_t pRandomGenerator)
	{
		return std::make_shared<hdbitset<cuSize>>(pRandomGenerator);
	}

	template<unsigned cuSize>
	inline std::shared_ptr<hdbitset<cuSize>> hdbitset_factory<cuSize>::zero(p_randengine_t pRandomGenerator)
	{
		p_hdbitset_t pGenerated = raw(pRandomGenerator);
		pGenerated->reset();
		return pGenerated;
	}

}