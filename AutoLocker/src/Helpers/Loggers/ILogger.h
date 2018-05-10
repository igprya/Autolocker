#pragma once

namespace Helpers
{
	enum LoggerVerbosityLevel
	{ SILENT, ERROR, WARNING, LOG, DEBUG };

	class ILogger
	{
		public: 
			virtual void Error(std::string msg) = 0;
			virtual void Warning(std::string msg) = 0;
			virtual void Log(std::string msg) = 0;			
			virtual void Debug(std::string msg, std::string value = "") = 0;
	};
}