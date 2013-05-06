//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "timer.h"

using namespace mcg;

Timer::Timer()
	: mStartTimel(0)
{}

void Timer::start()
{
	mStartTimel = GetTickCount();
}

DWORD Timer::elapsedMSec()
{
	return GetTickCount() - mStartTimel;
}

float Timer::elapsedSecF()
{
	return elapsedMSec() / 1000.0f;
}
