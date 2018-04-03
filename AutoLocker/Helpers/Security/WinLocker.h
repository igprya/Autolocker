#pragma once
#include "BaseLocker.h"

namespace Helpers 
{
	class WinLocker : public BaseLocker
	{
		public:
			int Lock();
			int Unlock();
			bool IsLocked();

	};
}

