#pragma once
#include <vector>
#include <filesystem>

#include <opencv2\core.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\face.hpp>

#include "ExitCodes.h"
#include "Timer.h"

using namespace cv;
using namespace face;

namespace Processing
{
	class Recognizer : public Helpers::Timer
	{
		public:
			Recognizer();
			~Recognizer();
			int InitRecognition(std::string facesDirectoryPath);
			int TrainModel();
			int TrainModel(std::vector<Mat>& cImages, std::vector<int> cLabels);
			int RecognizeFace(Mat& face, int& label, double& confidence);
			void GetLastRecognitionResults(int& lastLabel, double& lastConfidence);

		private:
			void SetLastRecognitionResults(int& label, double &confidence);

			std::vector<Mat> images;
			std::vector<int> labels;
			Ptr<FaceRecognizer> model;
			int lastRecognitionLablel = 0;
			double lastRecognitionConfidence = 150;
			
	};
}

