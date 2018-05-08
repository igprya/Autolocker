#include "stdafx.h"
#include "ConLogger.h"

namespace Helpers
{
	void ConLogger::Log(std::string msg)
	{
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		char buf[100] = { 0 };
		std::strftime(buf, sizeof(buf), "%X %x", std::localtime(&now));
		std::cout << buf << " " << msg << std::endl;
	}
}