#include "stdafx.h"
#include "StubLocker.h"
#include <Windows.h>

namespace Helpers 
{
	int StubLocker::Lock()
	{
		if (isLocked) {
			return ECODE_SUCCESS;
		}

		LockWorkStation();

		isLocked = true;

		return ECODE_SUCCESS;
	}

	int StubLocker::Unlock()
	{
		if (!isLocked) {
			return ECODE_SUCCESS;
		}

		isLocked = false;

		return ECODE_SUCCESS;
	}
}
