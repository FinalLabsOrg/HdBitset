#pragma once
#include <bitset>
#include <random>
#include <thread>
#include "randengine.hpp"

namespace flhdc {

    template<const unsigned cuSize>
    class hdbitset :
        public std::bitset<cuSize>
    {
    private:
        typedef std::bitset<cuSize> bitset_t;
        const p_randengine_t cpHdRandom;
        const size_t csSize;

    public:
        hdbitset(const p_randengine_t pRandomGenerator);
        hdbitset(const bitset_t oCopyable, const p_randengine_t pRandomGenerator);

        /*
        @return the number of bits which are set, based on bitset::count() but typecast to unsigned
        */
        unsigned count_u() const;

        /*
        @return a pointer to the random generator
        */
        p_randengine_t get_randengine() const;

        /*
        Rotates the attribute to the left
        */
        void rotl();

        /*
        Rotates the attribute to the right
        */
        void rotr();

        /*
        Scrolls the bits by signed int iRotate in order to permutate the bitset.
        Positive iRotate is rotl; negative iRotate is rotr
        */
        void scroll(int iRotate);

        /*
        @return the template parameter 
        */
        constexpr unsigned size_u() const;

        /*
        Randomizes the contents of the hdbitset
        Shuffling yields non-uniform bits(i.e. at least one is different from the others)
        */
        void shuffle();

        /*
        Randomizes the contents of the hdbitset
        Shuffling is limited to uShuffledWidth; all other bits are kept 0
        Shuffling yields non-uniform bits (i.e. at least one is different from the others)
        */
        void shuffle(unsigned uShuffledWidth);

        /*
        @return bitset::none() || bitset::all()
        */
        bool uniform() const;

    };

    template<const unsigned cuSize>
    inline hdbitset<cuSize>::hdbitset(const p_randengine_t pRandomGenerator) 
        : bitset_t(), cpHdRandom(pRandomGenerator), csSize(static_cast<size_t>(cuSize))
    {
    }


    template<const unsigned cuSize>
    inline hdbitset<cuSize>::hdbitset(const bitset_t oCopyable, const p_randengine_t pRandomGenerator)
        : bitset_t(oCopyable), cpHdRandom(pRandomGenerator), csSize(static_cast<size_t>(cuSize))
    {
    }

    template<const unsigned cuSize>
    inline unsigned hdbitset<cuSize>::count_u() const
    {
        return static_cast<unsigned>(bitset_t::count());
    }

    template<const unsigned cuSize>
    inline p_randengine_t hdbitset<cuSize>::get_randengine() const
    {
        return cpHdRandom;
    }


    template<const unsigned cuSize>
    inline void hdbitset<cuSize>::rotl()
    {
        bool bMovableBit = bitset_t::test(cuSize - 1);
        bitset_t::operator<<=(1);
        bitset_t::set(0, bMovableBit);
        return;
    }


    template<const unsigned cuSize>
    inline void hdbitset<cuSize>::rotr()
    {
        bool bMovableBit = bitset_t::test(0);
        bitset_t::operator>>=(1);
        bitset_t::set(cuSize - 1, bMovableBit);
        return;
    }


    template<const unsigned cuSize>
    inline void hdbitset<cuSize>::scroll(int iRotate)
    {
        if (iRotate) {
            unsigned uRotate = abs(iRotate) % cuSize;
            if (iRotate > 0) {
                for (unsigned u = 0; u < uRotate; u++) {
                    rotl();
                }
            }
            else {
                for (unsigned u = 0; u < uRotate; u++) {
                    rotr();
                }
            }
        }
        return;
    }

    template<const unsigned cuSize>
    inline bool hdbitset<cuSize>::uniform() const
    {
        return bitset_t::none() || bitset_t::all();
    }


    template<const unsigned cuSize>
    inline constexpr unsigned hdbitset<cuSize>::size_u() const
    {
        return cuSize;
    }


    template<const unsigned cuSize>
    inline void hdbitset<cuSize>::shuffle()
    {
        // reset is not needed here, because each and every bit is set to a 0 or 1 value randomly
        do {
            for (size_t u = 0; u < csSize; u++) {
                bitset_t::operator[](u) = cpHdRandom->rand_bin();
            }
        } while (uniform());
        return;
    }


    /*
    @todo do the type-cast at the object level
    */
    template<const unsigned cuSize>
    inline void hdbitset<cuSize>::shuffle(unsigned uShuffledWidth)
    {
        bitset_t::reset();
        size_t ullSize = static_cast<size_t>(uShuffledWidth);
        do {
            for (size_t u = 0; u < ullSize; u++) {
                bitset_t::operator[](u) = cpHdRandom->rand_bin();
            }
        } while (uniform());
        return;
    }

}