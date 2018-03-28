#pragma once
#include <vector>
#include <filesystem>

#include <opencv2\core.hpp>
#include <opencv2\imgcodecs.hpp>

#include "ExitCodes.h"
#include "Timer.h"

using namespace std;
using namespace cv;

namespace Processing
{
	class Recognizer : public Helpers::Timer
	{
		public:
			Recognizer();
			~Recognizer();
			int InitRecognition();
			int RecognizeFace(Mat& face);
		private:
			vector<Mat> images;
			vector<int> labels;
	};
}

