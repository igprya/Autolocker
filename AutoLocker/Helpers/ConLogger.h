#pragma once
#include <ctime>
#include <chrono>

#include "ILogger.h"

namespace Helpers
{
	class ConLogger : public ILogger
	{
		public:
			virtual void Log(std::string msg);
	};
}

