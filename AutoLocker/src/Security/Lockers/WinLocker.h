#pragma once
#define NOMINMAX
#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>

#include "IBaseLocker.h"

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
			HWND securityWindowHandle;
			std::mutex windowHandleMutex;			
	};
}

