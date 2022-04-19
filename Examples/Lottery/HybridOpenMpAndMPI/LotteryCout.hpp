#pragma once

#include <iostream>

class LotteryCout
{
public:
	static void coutLoopFooter(int iMyRank, unsigned uMatchCount, unsigned uCheckpointCount, double dActualDuration, unsigned uObservedThreadCount);
	static void coutLoopHead(unsigned uShuffleWidth);
	static void coutMainHead(unsigned uThread);
	};

inline void LotteryCout::coutLoopFooter(int iMyRank, unsigned uMatchCount, unsigned uCheckpointCount, double dActualDuration, unsigned uObservedThreadCount)
{
	double dAverageThreadCount = static_cast<double>(uObservedThreadCount) / static_cast<double>(uCheckpointCount);
	std::cout << "Rank: " << iMyRank << std::endl;
	std::cout << "The experiments yielded " << uMatchCount << " matches with " << uCheckpointCount << " checkpoints, in " << dActualDuration << " seconds, using " << dAverageThreadCount << " (observed; MPI & OMP) threads.";
	std::cout << std::endl;
}

inline void LotteryCout::coutLoopHead(unsigned uShuffleWidth)
{
	std::cout << std::endl;
	std::cout << "Experiment #" << uShuffleWidth << ".";
	std::cout << std::endl;
	std::cout << "The first " << uShuffleWidth << " bytes are randomized and the rest are kept 0.";
	std::cout << std::endl;
}