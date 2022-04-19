#pragma once
#include "hd.h"
#include "ticking.hpp"
#include "parameters.h"


using namespace hyperdimensional;

class experiment : public ticking
{
private:
	bool bExperimentResult;
	std::shared_ptr<hdbitset<uHd10048>> pGuessHelper;
	std::shared_ptr<hdbitset<uHd10048>> pReference;
	const unsigned uShuffleWidth;
public:
	experiment(unsigned uShuffleWidthToSet = uHd10048);
	void setGuessableBitset(const std::shared_ptr<hdbitset<uHd10048>> pGuessable);
	bool getExperimentResult() const;
	void tick();
};

