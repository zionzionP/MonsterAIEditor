#include "StopWatch.h"
#include "Windows.h"
#include "Winbase.h"


StopWatch::StopWatch()
	: timeElapsed_(0.0f)
	, startTime_(0L)
	, paused_(true)
{
}

/**
	@brief Start will start the stopwatch and register the current time-stamp
 */
void StopWatch::Start()
{
	paused_ = false;
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	startTime_ = time.QuadPart;
}

/**
	@brief Reset will reset the stopwatch so the next call to TimeElapsed will be the 
	time elapsed since the reset.
	@return the time elapsed since before the the reset was called.
 */
float StopWatch::Reset()
{
	float ret = 0.0f;
	LARGE_INTEGER time;
	LARGE_INTEGER freq;
	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&freq);
	ret = timeElapsed_ + static_cast<float>(time.QuadPart-startTime_)/static_cast<float>(freq.QuadPart);
	startTime_ = time.QuadPart;
	timeElapsed_ = 0.0f;
	return ret;
}

/**
	@brief Pause the time stopwatch.
 */
void StopWatch::Pause()
{
	LARGE_INTEGER nTime;
	LARGE_INTEGER freq;
	QueryPerformanceCounter(&nTime);
	QueryPerformanceFrequency(&freq);
	timeElapsed_ += static_cast<float>(nTime.QuadPart-startTime_)/static_cast<float>(freq.QuadPart);
	startTime_ = nTime.QuadPart;
	paused_ = true;
}

/**
	@brief Stops the stopwatch.
	@return the time elapsed.
 */
float StopWatch::Stop()
{
	Pause();
	return timeElapsed_;
}

/**
	@brief TimeElapsed will calculate the time elapsed since the stopwatch was started.
	@return elapsed time. If the stopwatch is not running, TimeElapsed will always return the same value.
 */
float StopWatch::TimeElapsed()
{
	if(paused_)	{
		return timeElapsed_;
	} else {
		LARGE_INTEGER nTime;
		LARGE_INTEGER freq;
		QueryPerformanceCounter(&nTime);
		QueryPerformanceFrequency(&freq);
		return timeElapsed_ + static_cast<float>(nTime.QuadPart-startTime_)/static_cast<float>(freq.QuadPart);
	}
}
