#include "massexperiment.h"

massexperiment::massexperiment()
{
}

unsigned massexperiment::getSuccessCount() const
{
	return uSuccess;
}

unsigned long long massexperiment::getTickCount() const
{
	return uTickCount;
}

void massexperiment::setExperiment(std::shared_ptr<experiment> pExperimentParameter)
{
	pExperiment = pExperimentParameter;
}

void massexperiment::runMassExperiment(const unsigned long long ullRuncount)
{
	for (uTickCount = 0; uTickCount < ullRuncount; uTickCount++) {
		pExperiment->tick();
		uSuccess += pExperiment->getExperimentResult();
	}
}
