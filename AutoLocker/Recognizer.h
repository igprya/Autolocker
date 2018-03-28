#pragma once
#include <vector>
#include <opencv2/core.hpp>

#include "Timer.h"

using namespace cv;

namespace Processing
{
	class Recognizer : public Helpers::Timer
	{
		public:
			Recognizer();
			~Recognizer();
			int RecognizeFaces(std::vector<Mat*>* facesVector);
			int LearnFace(std::vector<Mat*> faces);
	};
}

