#include "MpMpiFinder.h"
#include "omp.h"
#include "experiment.h"
#include "massexperiment.h"

unsigned MpMpiFinder::getSingleThreadMassExperimentResult()
{

	std::shared_ptr<experiment> pPrivateExperiment = std::make_shared<experiment>(uShuffleWidth);
	pPrivateExperiment->setGuessableBitset(pGuessableBitset);

	std::shared_ptr<massexperiment> pPrivateMassExperimentRunner = std::make_shared<massexperiment>();
	pPrivateMassExperimentRunner->setExperiment(pPrivateExperiment);

	pPrivateMassExperimentRunner->runMassExperiment(ullAttemptCount);
	return pPrivateMassExperimentRunner->getSuccessCount();

}

void MpMpiFinder::search()
{
	double dCheckpointStartTime, dCheckpointEndTime;
	dCheckpointStartTime = omp_get_wtime();
	uMatchCount = 0;
	uThreadCount = 0;
#pragma omp parallel default(none), reduction(+:uMatchCount,uThreadCount)
	{
		uMatchCount = getSingleThreadMassExperimentResult();
		uThreadCount = 1;
	}
	dCheckpointEndTime = omp_get_wtime();
	dActualCheckpointDuration = dCheckpointEndTime - dCheckpointStartTime;
}

void MpMpiFinder::setGuessableBitset(shared_ptr<hdbitset<uHd10048>> p)
{
	pGuessableBitset = p;
}

unsigned MpMpiFinder::getMatchCount() const
{
	return uMatchCount;
}

unsigned MpMpiFinder::getThreadCount() const
{
	return uThreadCount;
}

void MpMpiFinder::setShuffleWidth(unsigned uShuffleWidthToSet)
{
	uShuffleWidth = uShuffleWidthToSet;
}

void MpMpiFinder::readjust()
{
	ullAttemptCount = static_cast<unsigned>(static_cast<double>(ullAttemptCount) * dMaxCheckpointInterval / dActualCheckpointDuration);
}