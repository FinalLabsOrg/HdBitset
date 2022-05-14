#pragma once
/*
The shuffle width of the first experiment
*/
constexpr unsigned cuInitialShuffleWidth = 20;

/*
The number of attempts for the first iteration of the first experiment.
*/
constexpr unsigned cuOmpInitialAttemptCount = 10000000;

/*
Maximum allowed target checkpoint interval in seconds
*/
constexpr double cdMaxCheckpointInterval = 90.0; // 90.0;

/*
MPI ID of the Head Node
*/
constexpr int ciHeadNode = 0;