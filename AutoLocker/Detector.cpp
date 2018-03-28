#include "stdafx.h"
#include "Detector.h"

using namespace std;
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

	vector<Rect> Detector::GetFaceRects(Mat& frame)
	{
		vector<Rect> detectedFaces;
		Mat frameGray;

		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		equalizeHist(frameGray, frameGray);

		classifier->detectMultiScale(frameGray, detectedFaces, 1.2, 2, 0 | CASCADE_SCALE_IMAGE, Size(30,30));

		return detectedFaces;
	}

	vector<Mat> Detector::GetFaces(Mat& frame)
	{
		vector<Rect> faceRects = this->GetFaceRects(frame);
		vector<Mat> faceMats;
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
			resize(croppedFrame, resizedFrame, Size(128, 128), 0, 0, INTER_LINEAR);
			cvtColor(croppedFrame, croppedFrame, COLOR_BGR2GRAY);

			faceMats.push_back(croppedFrame);
		}

		return faceMats;
	}
}