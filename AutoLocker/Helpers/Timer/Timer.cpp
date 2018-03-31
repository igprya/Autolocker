#include "stdafx.h"
#include "Timer.h"

namespace Helpers {
	Timer::Timer()
	{
		operationTime = std::time(nullptr);
	}
	
	time_t Timer::GetOperationTime()
	{
		return operationTime;
	}

	void Timer::SetOperationTime()
	{
		operationTime = std::time(nullptr);
	}
}
