#include "stdafx.h"
#include "Detector.h"

using namespace cv;

namespace Processing
{
	Detector::~Detector()
	{
		delete classifier;
	}

	int Detector::InitDetection(std::string cascadePath)
	{
		classifier = new CascadeClassifier(cascadePath);
		return ECODE_SUCCESS;
	}

	std::vector<Rect> Detector::GetFaceRects(Mat& frame)
	{
		std::vector<Rect> detectedFaces;
		Mat frameGray;

		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		equalizeHist(frameGray, frameGray);

		classifier->detectMultiScale(frameGray, detectedFaces, 1.2, 2, 0 | CASCADE_SCALE_IMAGE, Size(100,100));

		SetOperationTime();

		return detectedFaces;
	}

	std::vector<Mat> Detector::GetFaces(Mat& frame)
	{
		std::vector<Rect> faceRects = this->GetFaceRects(frame);
		std::vector<Mat> faceMats;
		Mat croppedFrame;
		Mat resizedFrame;

		for (int i = 0; i < faceRects.size(); i++)
		{
			Rect currentFace;

			currentFace.x = faceRects[i].x;
			currentFace.y = faceRects[i].y;
			currentFace.width = faceRects[i].width;
			currentFace.height = faceRects[i].height;

			croppedFrame = frame(currentFace);
			resize(croppedFrame, resizedFrame, Size(600, 600), 0, 0, INTER_LINEAR);
			cvtColor(resizedFrame, resizedFrame, COLOR_BGR2GRAY);

			faceMats.push_back(resizedFrame);
		}

		return faceMats;
	}
}