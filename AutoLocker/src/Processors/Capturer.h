#pragma once
#include <ctime>

#include <opencv2\core.hpp>
#include <opencv2\videoio.hpp>

using namespace cv;

namespace Processing
{
	class Capturer
	{
		public:
			int InitCapture(int deviceIndex = 0);
			Mat GetFrame();

		private:
			VideoCapture capture;
	};
}

