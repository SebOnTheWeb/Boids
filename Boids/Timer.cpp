#pragma once

#include "Timer.h"

Timer::Timer() {
	this->Reset();
}

Timer::~Timer() {

}

void Timer::Reset() {
	this->startInstant = 0.0;
	this->endInstant = 0.0;
	bool running = false;
}

double Timer::GetNowInNs() {
	return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

void Timer::Start() {
	Reset();
	this->startInstant = GetNowInNs();
	running = true;
}

void Timer::Stop() {
	assert(running);

	this->endInstant = GetNowInNs();
	this->running = false;
}

float Timer::GetDeltaTimeInSeconds() {
	assert(!running);

	return (endInstant - startInstant) / 1000000000.0f;
}

float Timer::GetDeltaTimeInMilliSeconds() {
	assert(!running);

	return (endInstant - startInstant) / 1000000.0f;
}