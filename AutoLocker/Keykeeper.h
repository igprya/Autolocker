#pragma once

namespace Helpers {
	class Keykeeper
	{
		public:
			virtual int Lock() = 0;
			virtual int Unlock() = 0;

		protected:
			bool isLocked = false;
	};
}

