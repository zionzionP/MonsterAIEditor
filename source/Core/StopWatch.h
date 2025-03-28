#pragma once

/**
	@brief A lightweight time measurement class to assist the benchmarking
	of AI related things. Can be used for tracking events at a resolution
	of less than 1 microsecond.
 */
class StopWatch
{
public:
	StopWatch();
	void Start();
	float Reset();
	void Pause();
	float Stop();
	float TimeElapsed();

protected:
	long long startTime_;
	float timeElapsed_;
	bool paused_;
};
