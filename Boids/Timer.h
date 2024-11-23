#pragma once

#include <chrono>
#include <assert.h>

using namespace std::chrono;

class Timer {
private:
	double startInstant;
	double endInstant;
	bool running;

	void Reset();
	double GetNowInNs();

public:
	Timer();
	~Timer();

	void Start();
	void Stop();
	float GetDeltaTimeInSeconds();
	float GetDeltaTimeInMilliSeconds();
};