#pragma once

namespace Helpers 
{
	class IBaseLocker
	{
		public:
			virtual int Lock() = 0;
			virtual int Unlock() = 0;
			virtual bool IsLocked() = 0;

		protected:
			bool isLocked = false;
	};
}

