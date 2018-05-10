#include "stdafx.h"
#include "ConLogger.h"

namespace Helpers
{
	ConLogger::ConLogger(LoggerVerbosityLevel verbosity)
	{
		verbosityLevel = verbosity;
	}

	void ConLogger::Error(std::string msg)
	{
		if (verbosityLevel < LoggerVerbosityLevel::ERROR) {
			return;
		}

		std::cout << "[ERR ] " << GetCurrentTime() << " " << msg << std::endl;
	}


	void ConLogger::Warning(std::string msg)
	{
		if (verbosityLevel < LoggerVerbosityLevel::WARNING) {
			return;
		}

		std::cout << "[WARN] " << GetCurrentTime() << " " << msg << std::endl;
	}

	void ConLogger::Log(std::string msg)
	{
		if (verbosityLevel < LoggerVerbosityLevel::LOG) {
			return;
		}

		std::cout << "[INFO] " << GetCurrentTime() << " " << msg << std::endl;
	}



	void ConLogger::Debug(std::string msg, std::string value)
	{
		if (verbosityLevel < LoggerVerbosityLevel::DEBUG) {
			return;
		}

		std::cout << "[DEBG] " << GetCurrentTime() << " " << msg << " " << value << std::endl;
	}


	std::string ConLogger::GetCurrentTime()
	{
		char buf[100] = { 0 };
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::strftime(buf, sizeof(buf), "%X %x", std::localtime(&now));
		return std::string(buf);
	}
}