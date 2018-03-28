#include "stdafx.h"
#include "Detector.h"

using namespace cv;

namespace Processing
{
	Detector::Detector()
	{
	}

	Detector::~Detector()
	{
	}

	int Detector::InitDetection(std::string cascadePath)
	{
		classifier = new CascadeClassifier(cascadePath);

		return ECODE_SUCCESS;
	}

	std::vector<Rect>* Detector::GetFaces(Mat* frame)
	{
		std::vector<Rect>* detectedFaces = new std::vector<Rect>();
		Mat frameGray;

		cvtColor(*frame, frameGray, COLOR_BGR2GRAY);
		equalizeHist(frameGray, frameGray);

		classifier->detectMultiScale(frameGray, *detectedFaces, 1.5, 3, 0 | CASCADE_SCALE_IMAGE, Size(40,40));

		return detectedFaces;
	}
}