#pragma once
#include <ctime>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include "ExitCodes.h"
#include "Timer.h"

namespace Processing
{
	class Capturer : public Helpers::Timer
	{
		public:
			Capturer();
			~Capturer();
			int InitCapture(int deviceIndex);
			cv::Mat* GetFrame();

		private:
			cv::VideoCapture capture;
	};
}

