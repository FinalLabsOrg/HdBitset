#pragma once
/*
Number of threads that should be used by OpenMP.
*/
const unsigned cuThreads = 3;

/*
The number of attempts for the first experiment.
*/
const unsigned cuInitialAttemptCount = 10000;

/*
The previous attempt count is multiplied by cdNoMatchMultiplier for the next experiment
if the match count of the current experiment is not above cuNoMatchTreshold
*/
const unsigned cuNoMatchTreshold = 1;

/*
The previous attempt count is multiplied by cdNoMatchMultiplier for the next experiment
if the match count of the current experiment is not above cuNoMatchTreshold
*/
const double cdNoMatchMultiplier = 2.2;

/*
End time is dispayed for intervals longer than cuEndtimeNotification seconds
*/
const unsigned cuEndtimeNotification = 60 * 10;