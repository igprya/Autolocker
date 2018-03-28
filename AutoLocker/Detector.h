#pragma once
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#include "ExitCodes.h"
#include "Timer.h"

using namespace cv;

namespace Processing
{
	class Detector : public Helpers::Timer
	{
		public:
			Detector();
			~Detector();
			int InitDetection(std::string cascadePath);
			std::vector<Rect>* GetFaces(Mat* frame);

		private:
			CascadeClassifier* classifier;
	};
}

