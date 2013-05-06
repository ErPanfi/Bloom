//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include <Windows.h>

namespace mcg
{
	class Timer
	{
	public:
		Timer();

		void start();
		DWORD elapsedMSec();
		float elapsedSecF();

	private:
		DWORD mStartTimel;
	};
}
