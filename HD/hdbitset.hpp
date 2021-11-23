#pragma once
#include <bitset>

namespace hyperdimensional {

    template<unsigned uSize>
    class hdbitset :
        public std::bitset<uSize>
    {
    public:

        hdbitset();
        hdbitset(std::bitset<uSize> oCopyable);

        /*
        bitset::count() typecast to unsigned
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
        returns the template parameter 
        this is also how bitset::size() is implemented
        */
        unsigned size_u() const;

        /*
        Randomizes the contents of the hdbitset
        */
        void shuffle();

        /*
        bitset::all() || bitset::none()
        */
        bool uniform() const;

    };


    template<unsigned uSize>
    inline hdbitset<uSize>::hdbitset() : std::bitset<uSize>()
    {
    }


    template<unsigned uSize>
    inline hdbitset<uSize>::hdbitset(std::bitset<uSize> oCopyable) : std::bitset<uSize>(oCopyable)
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
    }


    template<unsigned uSize>
    inline void hdbitset<uSize>::rotr()
    {
        bool bMovableBit = std::bitset<uSize>::test(0);
        std::bitset<uSize>::operator>>=(1);
        std::bitset<uSize>::set(uSize - 1, bMovableBit);
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
    }


    template<unsigned uSize>
    inline bool hdbitset<uSize>::uniform() const
    {
        return std::bitset<uSize>::all() || std::bitset<uSize>::none();
    }


    template<unsigned uSize>
    inline unsigned hdbitset<uSize>::size_u() const
    {
        return uSize;
    }


    template<unsigned uSize>
    inline void hdbitset<uSize>::shuffle()
    {
        std::bitset<uSize>::reset();
        do {
            for (unsigned u = 0; u < uSize; u++) {
                if (rand() % 2)
                    std::bitset<uSize>::flip(u);
            }
        } while (uniform());

    }

}