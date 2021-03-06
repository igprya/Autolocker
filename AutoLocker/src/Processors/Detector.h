#pragma once
#include <vector>

#include <opencv2\core.hpp>
#include <opencv2\objdetect.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>

using namespace cv;

namespace Processing
{
	class Detector
	{
		public:
			~Detector();
			int InitDetection(std::string cascadePath);
			std::vector<Rect> GetFaceRects(Mat& frame);
			std::vector<Mat> GetFaces(Mat& frame);

		private:
			CascadeClassifier* classifier;
	};
}

