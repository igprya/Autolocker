#pragma once
#include <vector>
#include <opencv2/core.hpp>

#include "Timer.h"

using namespace cv;

namespace Processing
{
	class Detector : public Helpers::Timer
	{
		public:
			Detector();
			~Detector();
			std::vector<Mat*>* GetFaces(Mat* frame);
	};
}

