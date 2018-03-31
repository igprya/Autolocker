#include "stdafx.h"
#include "WinLocker.h"
#include <Windows.h>

namespace Helpers 
{
	int WinLocker::Lock()
	{
		if (isLocked) {
			return ECODE_SUCCESS;
		}

		//SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 1);

		printf("Locked");

		isLocked = true;

		return ECODE_SUCCESS;
	}

	int WinLocker::Unlock()
	{
		if (!isLocked) {
			return ECODE_SUCCESS;
		}

		//SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, -1);

		printf("Unlocked");

		isLocked = false;

		return ECODE_SUCCESS;
	}

	bool WinLocker::IsLocked()
	{
		return isLocked;
	}
}
