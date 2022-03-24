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

	unsigned long long uAttemptCount = cuInitialAttemptCount;
	std::shared_ptr<experiment> pSerialExperiment;

	std::cout << "Running experiments en masse using " << cuThreads << " threads. " << std::endl;

	for (unsigned uShuffleWidth = 1; uShuffleWidth < uHd10048; uShuffleWidth++) {

		std::shared_ptr<hdbitset<uHd10048>> pReferenceBitset = hdfactory<uHd10048>::random(uShuffleWidth);

		pSerialExperiment = std::make_shared<experiment>(uShuffleWidth);
		pSerialExperiment->setGuessableBitset(pReferenceBitset);

		double dOmpStartTime = omp_get_wtime();
		unsigned long long ullTotalTicksForControl = 0;
		unsigned uMatchCount = 0;

		std::cout << std::endl;
		std::cout << "Experiment #" << uShuffleWidth << ".";
		std::cout << std::endl;
		std::cout << "The first " << uShuffleWidth << " bytes are randomized and the rest are kept 0.";
		std::cout << std::endl;

#pragma omp parallel default(none), shared(uAttemptCount, pReferenceBitset, uShuffleWidth), reduction(+:ullTotalTicksForControl,uMatchCount), num_threads(cuThreads)
		{
			std::shared_ptr<experiment> pPrivateExperiment = std::make_shared<experiment>(uShuffleWidth);
			pPrivateExperiment->setGuessableBitset(pReferenceBitset);

			std::shared_ptr<massexperiment> pPrivateMassExperimentRunner = std::make_shared<massexperiment>();
			pPrivateMassExperimentRunner->setExperiment(pPrivateExperiment);

			pPrivateMassExperimentRunner->runMassExperiment(uAttemptCount);
			ullTotalTicksForControl = pPrivateMassExperimentRunner->getTickCount();
			uMatchCount += pPrivateMassExperimentRunner->getSuccessCount();
		}

		double dOmpEndTime = omp_get_wtime();
		double dActualDuration = dOmpEndTime - dOmpStartTime;

		std::cout << ullTotalTicksForControl << " trials yielded " << uMatchCount << " matches.";
		std::cout << std::endl;
		std::cout << "This took " << dActualDuration << " seconds";
		std::cout << std::endl;
		std::cout << "The observed thread count is " << ullTotalTicksForControl / uAttemptCount<< ".";
		std::cout << std::endl;

		if (uMatchCount<=cuNoMatchTreshold) {

			uAttemptCount = static_cast<unsigned>(uAttemptCount * cdNoMatchMultiplier);
			double dExpectedDuration = dActualDuration * cdNoMatchMultiplier;

			std::cout << "There were not enough matches and therefore the trial count is increased for the next experiment.";
			std::cout << std::endl;
			std::cout << "Therefore the next experiment is expected to last a bit over " << static_cast<unsigned>(dExpectedDuration) << " seconds.";
			std::cout << std::endl;

			if (dExpectedDuration > cuEndtimeNotification) {

				time_t result = time(NULL) + static_cast<time_t>(dExpectedDuration);
				char str[26];
				ctime_s(str, sizeof str, &result);

				std::cout << "This next experiment is expected to end at " << str;

			}

		}

	}

	return 0;

}