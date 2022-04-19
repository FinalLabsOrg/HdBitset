#include <iostream>
#include <memory>
#include <omp.h>
#include <time.h>
#include "experiment.h"
#include "hd.h"
#include "massexperiment.h"
#include "parameters.h"

using namespace hyperdimensional;

int main() {

	unsigned uMatchCount, uCheckpointCount;
	double dOmpStartTime, dOmpEndTime, dActualDuration, dActualCheckpointInterval;
	unsigned long long ullAttemptCount;
	std::shared_ptr<experiment> pSerialExperiment;

	std::cout << "Running experiments en masse using " << cuThreads << " threads. " << std::endl;

	for (unsigned uShuffleWidth = 1; uShuffleWidth < uHd10048; uShuffleWidth++) {

		ullAttemptCount = cuInitialAttemptCount;
		dActualDuration = 0.0;
		uCheckpointCount = 0;

		std::shared_ptr<hdbitset<uHd10048>> pReferenceBitset = hdfactory<uHd10048>::random(uShuffleWidth);

		std::cout << std::endl;
		std::cout << "Experiment #" << uShuffleWidth << ".";
		std::cout << std::endl;
		std::cout << "The first " << uShuffleWidth << " bytes are randomized and the rest are kept 0.";
		std::cout << std::endl;

		do {

			uMatchCount = 0;
			unsigned uThreadCountControl = 0;
			dOmpStartTime = omp_get_wtime();

			#pragma omp parallel default(none), shared(ullAttemptCount, pReferenceBitset, uShuffleWidth), reduction(+:uMatchCount,uThreadCountControl), num_threads(cuThreads)
			{
				std::shared_ptr<experiment> pPrivateExperiment = std::make_shared<experiment>(uShuffleWidth);
				pPrivateExperiment->setGuessableBitset(pReferenceBitset);

				std::shared_ptr<massexperiment> pPrivateMassExperimentRunner = std::make_shared<massexperiment>();
				pPrivateMassExperimentRunner->setExperiment(pPrivateExperiment);

				pPrivateMassExperimentRunner->runMassExperiment(ullAttemptCount);
				uMatchCount += pPrivateMassExperimentRunner->getSuccessCount();
				uThreadCountControl = 1;
			}

			dOmpEndTime = omp_get_wtime();
			dActualCheckpointInterval = dOmpEndTime - dOmpStartTime;
			dActualDuration += dActualCheckpointInterval;

			if (uThreadCountControl != cuThreads) {
				std::cout << "Observed thread count is " << uThreadCountControl << " vs. the expected " << cuThreads << ".";
				std::cout << "Exiting.";
				exit(1);
			}

			if (dActualCheckpointInterval * cdCheckpointEasing < cdMaxCheckpointInterval) {
				ullAttemptCount = static_cast<unsigned>(static_cast<double>(ullAttemptCount) * cdCheckpointEasing);
			}

			uCheckpointCount++;

		} while (!uMatchCount);

		std::cout << "The experiments yielded " << uMatchCount << " matches with " << uCheckpointCount << " checkpoints, in " << dActualDuration << " seconds";
		std::cout << std::endl;

	}

	return 0;

}