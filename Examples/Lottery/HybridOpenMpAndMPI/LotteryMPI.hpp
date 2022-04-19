#pragma once
#include <memory>
#include <mpi.h>
#include "hd.h"

using namespace std;
using namespace hyperdimensional;

class LotteryMPI
{
public:

    template<unsigned uSize>
    static void decode(unsigned char* pTransfer, shared_ptr < hdbitset<uSize> > pTarget);

    template<unsigned uSize>
    static shared_ptr < hdbitset<uSize> > distribute(shared_ptr < hdbitset<uSize> > pOrigin);

    template<unsigned uSize>
    static void encode(unsigned char* pTransfer, shared_ptr < hdbitset<uSize> > pOrigin);

    static int getMyRank();

    static bool isRoot();

    static void test();

};

template<unsigned uSize>
inline shared_ptr<hdbitset<uSize>> LotteryMPI::distribute(shared_ptr<hdbitset<uSize>> pOrigin)
{
    unsigned char aTransfer[uHd10048];
    if (isRoot()) {
        encode(aTransfer, pOrigin);
    }
    MPI_Bcast(&aTransfer, uHd10048, MPI_UNSIGNED_CHAR, ciHeadNode, MPI_COMM_WORLD);
    if (!isRoot()) {
        shared_ptr<hdbitset<uSize>> pTarget = hdfactory<uSize>::zero();
        decode(aTransfer, pTarget);
        return pTarget;
    }
    else {
        return pOrigin;
    }
}

template<unsigned uSize>
inline void LotteryMPI::decode(unsigned char* pTransfer, shared_ptr<hdbitset<uSize>> pTarget)
{
    for (unsigned u = 0; u < uSize; u++) {
        pTarget->operator[](u) = pTransfer[u];
    }
}

template<unsigned uSize>
inline void LotteryMPI::encode(unsigned char* pTransfer, shared_ptr<hdbitset<uSize>> pOrigin)
{
    for (unsigned u = 0; u < uSize; u++) {
        pTransfer[u] = pOrigin->operator[](u);
    }
}

inline int LotteryMPI::getMyRank()
{
    int iMyRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);
    return iMyRank;
}

inline bool LotteryMPI::isRoot()
{
    return getMyRank() == ciHeadNode;
}

void LotteryMPI::test()
{
	unsigned char aTransfer[uHd10048];
	shared_ptr<hdbitset<uHd10048>> pSource = hdfactory<uHd10048>::random();
	shared_ptr<hdbitset<uHd10048>> pTarget = hdfactory<uHd10048>::zero();
	LotteryMPI::encode(aTransfer, pSource);
	LotteryMPI::decode(aTransfer, pTarget);
	if (!hdops<uHd10048>::eq(pSource, pTarget)) {
		cout << "MPI data transfer test failed." << endl;
        exit(0);
    }
}