#include "Timer.h"

Timer::Timer() :mDeltaTime(0.0f), mBaseTime({0}), mPreviousTime({0}), mCurrentTime({0}) {
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency(&countsPerSecond);
	mSecondsPerCount = 1.0 / static_cast<double>(countsPerSecond.QuadPart);
}

void Timer::Reset() {
	QueryPerformanceCounter(&mBaseTime);
	mPreviousTime = mBaseTime;
}

void Timer::Tick() {
	QueryPerformanceCounter(&mCurrentTime);
	mDeltaTime = (mCurrentTime.QuadPart - mPreviousTime.QuadPart) * mSecondsPerCount;
	mPreviousTime = mCurrentTime; 
}

float Timer::GetDeltaTime() const {
	return mDeltaTime;
}

float Timer::GetTotalTime() const {
	return ((mCurrentTime.QuadPart - mBaseTime.QuadPart) * mSecondsPerCount);
}