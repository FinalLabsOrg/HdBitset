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
End time is dispayed for intervals longer than cuEndtimeNotification seconds
*/
const unsigned cuEndtimeNotification = 60 * 10;

/*
Factor for the gradual increase of the checkpoint interval
*/
const double cdCheckpointEasing = 1.25;

/*
Maximum allowed target checkpoint interval in seconds
*/
const double cdMaxCheckpointInterval = 90.0;