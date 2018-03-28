#pragma once
#include "Keykeeper.h"
#include "ExitCodes.h"

namespace Helpers 
{
	class StubLocker : public Keykeeper
	{
		public:
			int Lock();
			int Unlock();
	};
}

