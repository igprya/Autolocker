#pragma once
#include <stdafx.h>

namespace Helpers
{
	class ILogger
	{
		public: 
			virtual void Log(std::string msg) = 0;
	};
}