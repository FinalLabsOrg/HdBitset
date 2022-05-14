#include "experiment.h"
#include "hd.h"
#include <iostream>
using namespace hyperdimensional;

experiment::experiment(unsigned uShuffleWidthToSet) : ticking(), uShuffleWidth(uShuffleWidthToSet)
{
    pGuessHelper = hdfactory<uHd10048>::zero();
}

void experiment::setGuessableBitset(const std::shared_ptr<hdbitset<uHd10048>> pGuessable)
{
    pReference = pGuessable;
}

bool experiment::getExperimentResult() const
{
    return bExperimentResult;
}

void experiment::tick()
{
    pGuessHelper->shuffle(uShuffleWidth);
    bExperimentResult = hdops<uHd10048>::eq(pReference, pGuessHelper);
}
