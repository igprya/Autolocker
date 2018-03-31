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

		LockWorkStation();

		isLocked = true;

		return ECODE_SUCCESS;
	}

	int WinLocker::Unlock()
	{
		if (!isLocked) {
			return ECODE_SUCCESS;
		}

		isLocked = false;

		return ECODE_SUCCESS;
	}
}
