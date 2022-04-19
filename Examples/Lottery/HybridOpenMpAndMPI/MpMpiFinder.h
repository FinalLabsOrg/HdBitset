#pragma once
#include "hd.h"
#include "parameters.h"

using namespace hyperdimensional;
using namespace std;

class MpMpiFinder
{
private:
	const double dMaxCheckpointInterval = cdMaxCheckpointInterval;
	double dActualCheckpointDuration = 0.0;
	shared_ptr<hdbitset<uHd10048>> pGuessableBitset = 0;
	unsigned uMatchCount = 0;
	unsigned long long ullAttemptCount = cuOmpInitialAttemptCount;
	double dSearchDuration = 0.0;
	unsigned uShuffleWidth = 0;
	unsigned uThreadCount = 0;
	unsigned getSingleThreadMassExperimentResult();
public:
	void search();
	void setGuessableBitset(shared_ptr<hdbitset<uHd10048>> p);
	unsigned getMatchCount() const;
	unsigned getThreadCount() const;
	void setShuffleWidth(unsigned uShuffleWidthToSet);
	void readjust();
};

