#pragma once
#include <bitset>
#include <random>
#include <thread>
#include "hdrandom.hpp"

namespace hyperdimensional {

    template<unsigned uSize>
    class hdbitset :
        public std::bitset<uSize>
    {
    private:
        static thread_local hdrandom oHdRandom;
        const size_t csSize;

    public:
        /*
        @return a random bit using the static hdrandom engine
        */
        static unsigned rand();

        hdbitset();
        hdbitset(std::bitset<uSize> oCopyable);

        /*
        @return the number of bits which are set, based on bitset::count() but typecast to unsigned
        */
        unsigned count_u() const;

        /*
        Permutates the bitset, scrolling the bits by signed int iRotate
        Positive iRotate is rotl; negative iRotate is rotr
        */
        void permutate(int iRotate);

        /*
        Rotates the attribute to the left
        */
        void rotl();

        /*
        Rotates the attribute to the right
        */
        void rotr();

        /*
        @return the template parameter 
        */
        unsigned size_u() const;

        /*
        Randomizes the contents of the hdbitset
        Shuffling yields non-uniform bits(i.e. at least one is different from the others)
        */
        void shuffle();

        /*
        Randomizes the contents of the hdbitset
        Shuffling is limited to uShuffledWidth; all other bits are kept 0
        Shuffling yields non-uniform bits(i.e. at least one is different from the others)
        */
        void shuffle(unsigned uShuffledWidth);

        /*
        @return bitset::all() || bitset::none()
        */
        bool uniform() const;

    };


    template<unsigned uSize>
    thread_local hdrandom hdbitset<uSize>::oHdRandom;

    template<unsigned uSize>
    inline unsigned hdbitset<uSize>::rand()
    {
        return oHdRandom.rand();
    }


    template<unsigned uSize>
    inline hdbitset<uSize>::hdbitset() : std::bitset<uSize>(), csSize(static_cast<size_t>(uSize))
    {
    }


    template<unsigned uSize>
    inline hdbitset<uSize>::hdbitset(std::bitset<uSize> oCopyable) : std::bitset<uSize>(oCopyable), csSize(static_cast<size_t>(uSize))
    {
    }

    template<unsigned uSize>
    inline unsigned hdbitset<uSize>::count_u() const
    {
        return static_cast<unsigned>(std::bitset<uSize>::count());
    }


    template<unsigned uSize>
    inline void hdbitset<uSize>::rotl()
    {
        bool bMovableBit = std::bitset<uSize>::test(uSize - 1);
        std::bitset<uSize>::operator<<=(1);
        std::bitset<uSize>::set(0, bMovableBit);
        return;
    }


    template<unsigned uSize>
    inline void hdbitset<uSize>::rotr()
    {
        bool bMovableBit = std::bitset<uSize>::test(0);
        std::bitset<uSize>::operator>>=(1);
        std::bitset<uSize>::set(uSize - 1, bMovableBit);
        return;
    }


    template<unsigned uSize>
    inline void hdbitset<uSize>::permutate(int iRotate)
    {
        if (iRotate) {
            unsigned uRotate = abs(iRotate) % uSize;
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

    template<unsigned uSize>
    inline bool hdbitset<uSize>::uniform() const
    {
        return std::bitset<uSize>::none() || std::bitset<uSize>::all();
    }


    template<unsigned uSize>
    inline unsigned hdbitset<uSize>::size_u() const
    {
        return uSize;
    }


    template<unsigned uSize>
    inline void hdbitset<uSize>::shuffle()
    {
        // reset is not needed here, because each and every bit is set to a 0 or 1 value randomly
        do {
            for (size_t u = 0; u < csSize; u++) {
                std::bitset<uSize>::operator[](u) = hdbitset<uSize>::rand();
            }
        } while (uniform());
        return;
    }


    /*
    @todo do the type-cast at the object level
    */
    template<unsigned uSize>
    inline void hdbitset<uSize>::shuffle(unsigned uShuffledWidth)
    {
        std::bitset<uSize>::reset();
        size_t ullSize = static_cast<size_t>(uShuffledWidth);
        do {
            for (size_t u = 0; u < ullSize; u++) {
                std::bitset<uSize>::operator[](u) = hdbitset<uSize>::rand();
            }
        } while (uniform());
        return;
    }

}