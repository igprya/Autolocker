#pragma once
#include "Keykeeper.h"
#include "ExitCodes.h"

namespace Helpers 
{
	class WinLocker : public Keykeeper
	{
		public:
			int Lock();
			int Unlock();
	};
}

