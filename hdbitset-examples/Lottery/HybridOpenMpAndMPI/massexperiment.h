#pragma once
#include <memory>
#include "experiment.h"

class massexperiment
{
private:
	std::shared_ptr<experiment> pExperiment;
	unsigned uSuccess = 0;
	unsigned long long uTickCount = 0;

public:
	massexperiment();
	unsigned getSuccessCount() const;
	unsigned long long getTickCount() const;
	void setExperiment(std::shared_ptr<experiment> pExperimentParameter);
	void runMassExperiment(const unsigned long long ullRuncount);

};

