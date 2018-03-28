#pragma once

#include <ctime>

namespace Helpers {
	class Timer
	{
		public:
			Timer();
			time_t GetOperationTime();

		protected:
			void SetOperationTime();

		private:
			time_t operationTime;
	};
}

