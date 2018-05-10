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
			ConLogger(LoggerVerbosityLevel verbosity = LoggerVerbosityLevel::LOG);
			virtual void Log(std::string msg);
			virtual void Warning(std::string msg);
			virtual void Error(std::string msg);
			virtual void Debug(std::string msg, std::string value = "");
		
		protected:
			std::string GetCurrentTime();
			LoggerVerbosityLevel verbosityLevel;
	};
}

