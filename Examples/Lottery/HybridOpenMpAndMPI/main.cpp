#include <memory>
#include <mpi.h>
#include <omp.h>
#include <time.h>
#include "hd.h"
#include "parameters.h"
#include "LotteryCout.hpp"
#include "LotteryMPI.hpp"
#include "MpMpiFinder.h"

/*
cd ~/projects/HdLotteryMpMpi/bin/x64/Release
mpirun -hostfile ~/hosts/host-all --map-by core -N 1 HdLotteryMpMpi.out

mpirun --map-by core -N 1 HdLotteryMpMpi.out
*/
using namespace hyperdimensional;
using namespace std;

MpMpiFinder oMpMpiFinder;
int iClusterMatchCount = 0;
int iClusterCheckpointCount = 0;
unsigned uClusterThreadCount = 0;
unsigned uMpiShuffleWidth = 0;
shared_ptr<hdbitset<uHd10048>> pGuessableBitset = 0;
unsigned uMatchCountOnNode = 0;
unsigned uThreadCountOnNode = 0;

void createGuessableBitset(const unsigned uShuffleWidth) {
	pGuessableBitset = LotteryMPI::isRoot() ? hdfactory<uHd10048>::random(uShuffleWidth) : hdfactory<uHd10048>::zero();
	pGuessableBitset = LotteryMPI::distribute(pGuessableBitset);
}

void search() {
	oMpMpiFinder.setShuffleWidth(uMpiShuffleWidth);
	oMpMpiFinder.setGuessableBitset(pGuessableBitset);
	oMpMpiFinder.search();
	oMpMpiFinder.readjust();
}

void doRootLoop() {

	for (uMpiShuffleWidth = cuInitialShuffleWidth; uMpiShuffleWidth < uHd10048; uMpiShuffleWidth++) {

		iClusterCheckpointCount=0;
		uClusterThreadCount = 0;
		iClusterMatchCount = 0;
		LotteryCout::coutLoopHead(uMpiShuffleWidth);
		MPI_Bcast(&uMpiShuffleWidth, 1, MPI_UNSIGNED_CHAR, ciHeadNode, MPI_COMM_WORLD);
		createGuessableBitset(uMpiShuffleWidth);
		double dStart = omp_get_wtime();
		do {

			iClusterCheckpointCount++;
			search();
			uMatchCountOnNode = oMpMpiFinder.getMatchCount();
			uThreadCountOnNode = oMpMpiFinder.getThreadCount();
			MPI_Reduce(&uThreadCountOnNode, &uClusterThreadCount, 1, MPI_INT, MPI_SUM, ciHeadNode, MPI_COMM_WORLD);
			MPI_Reduce(&uMatchCountOnNode, &iClusterMatchCount, 1, MPI_INT, MPI_SUM, ciHeadNode, MPI_COMM_WORLD);

		} while (!iClusterMatchCount);
		double dEnd = omp_get_wtime();

		LotteryCout::coutLoopFooter(LotteryMPI::getMyRank(), iClusterMatchCount, iClusterCheckpointCount, dEnd-dStart, uClusterThreadCount);

	}

}

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	if (LotteryMPI::isRoot()) {
		doRootLoop();
	}
	else {

		do {

			iClusterCheckpointCount = 0;
			uClusterThreadCount = 0;
			iClusterMatchCount = 0;
			MPI_Bcast(&uMpiShuffleWidth, 1, MPI_UNSIGNED_CHAR, ciHeadNode, MPI_COMM_WORLD);
			createGuessableBitset(uMpiShuffleWidth);
			search();
			uMatchCountOnNode = oMpMpiFinder.getMatchCount();
			uThreadCountOnNode = oMpMpiFinder.getThreadCount();
			MPI_Reduce(&uThreadCountOnNode, &uClusterThreadCount, 1, MPI_INT, MPI_SUM, ciHeadNode, MPI_COMM_WORLD);
			MPI_Reduce(&uMatchCountOnNode, &iClusterMatchCount, 1, MPI_INT, MPI_SUM, ciHeadNode, MPI_COMM_WORLD);

		} while (!iClusterMatchCount); // this is not the best end condition here; the best would be to receive an explicit signal from the root

	}

	MPI_Finalize();

	return EXIT_SUCCESS;

}