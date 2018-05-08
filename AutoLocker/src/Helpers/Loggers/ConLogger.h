#pragma once
#include <ctime>
#include <chrono>
#include <string>

#include "ILogger.h"

namespace Helpers
{
	class ConLogger : public ILogger
	{
		public:
			virtual void Log(std::string msg);
	};
}

