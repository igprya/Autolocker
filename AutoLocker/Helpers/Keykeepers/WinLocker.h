#pragma once
#include "Keykeeper.h"

namespace Helpers 
{
	class WinLocker : public Keykeeper
	{
		public:
			int Lock();
			int Unlock();
	};
}

