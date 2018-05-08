#pragma once
#include "IBaseLocker.h"

#define NOMINMAX
#include <Windows.h>
#include <vector>
#include <thread>

namespace Security 
{
	class WinLocker : public IBaseLocker
	{
		public:
			int Lock();
			int Unlock();
			bool IsLocked();

		private:
			int SecureSceens();
			int CreateSecutiryWindowLock();
	};
}

