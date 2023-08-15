#pragma once

#include <Windows.h>

class Timer {
public:
	Timer();
	void Reset();
	void Tick();
	float GetDeltaTime() const;
	float GetTotalTime() const;
private:
	float mSecondsPerCount;
	float mDeltaTime;

	LARGE_INTEGER mBaseTime;
	LARGE_INTEGER mPreviousTime;
	LARGE_INTEGER mCurrentTime;
};