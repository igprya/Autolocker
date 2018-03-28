#include "stdafx.h"
#include "Engine.h"

#include <thread>
#include <chrono>
#include <opencv2\highgui.hpp>


using namespace cv;

Engine::Engine()
{
	capturer = new Processing::Capturer();
	detector = new Processing::Detector();
	recognizer = new Processing::Recognizer();
}

Engine::~Engine()
{
	capturer->~Capturer();
	detector->~Detector();
	recognizer->~Recognizer();
}

int Engine::Run()
{
	int engInitResult = InitEngine();

	if (engInitResult < ECODE_SUCCESS) {
		keykeeper->Lock();
		return engInitResult;
	}

	int procInitResult = InitProcessors();

	if (procInitResult < ECODE_SUCCESS) {
		keykeeper->Lock();
		return procInitResult;
	}

	while (true) {
		int cycleResult = EngineCycle();		

		if (cycleResult < ECODE_SUCCESS) {
			keykeeper->Lock();

			return cycleResult;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return ECODE_SUCCESS;
}

int Engine::InitEngine()
{
	keykeeper = new Helpers::StubLocker();

	return ECODE_SUCCESS;
}

int Engine::InitProcessors()
{
	bool capturerRunning = false;
	bool detectorRunning = false;
	bool recognizerRunning = false;

	capturerRunning = this->capturer->InitCapture(0);
	detectorRunning = this->detector->InitDetection("C:/haarcascade_frontalface_alt.xml");
	recognizerRunning = true;//this->recognizer->InitRecognition();

	if (!capturerRunning) {
		return ERROR_CAPTURER_FAILED_INIT;
	}

	if (!detectorRunning) {
		return ERROR_DETECTOR_FAILED_INIT;
	}

	if (!recognizerRunning) {
		return ERROR_RECOGNIZER_FAILED_INIT;
	}

	return ECODE_SUCCESS;
}

int Engine::EngineCycle()
{
	int handleResult = ECODE_SUCCESS;

	currentFrame = capturer->GetFrame();

	if (!currentFrame) {
		return ERROR_CAPTURER_NOFRAME;
	}

	std::vector<Rect>* faces = detector->GetFaces(currentFrame);

	if (faces->size() == 0) 
	{
		handleResult = HandleDetectionFailure();
		if (handleResult < ECODE_SUCCESS) {
			return handleResult;
		}
	}
	else {
		failedDetectionCount = 0;
	}

	DrawFaceFrames(*currentFrame, *faces);
	imshow("dbg", *currentFrame);
	waitKey(5);

	/*
	if (IsRecognitionRequired(recognizer->GetOperationTime())) {
		int recognitionResult = recognizer->RecognizeFaces(faces);

		if (recognitionResult < ECODE_SUCCESS) 
		{
			handleResult = HandleRecognitionFailure();
			if (handleResult < ECODE_SUCCESS) {
				return handleResult;
			}
		}
	}*/

	return ECODE_SUCCESS;
}

bool Engine::IsRecognitionRequired(time_t lastRecognition)
{
	double timeDifference = std::difftime(lastRecognition, std::time(nullptr));

	if (timeDifference > recognitionInterval) {
		return true;
	}
	else {
		return false;
	}
}

int Engine::HandleDetectionFailure()
{
	failedDetectionCount++;

	if (failedDetectionCount > failedDetectionsThreshold) {
		failedDetectionCount = 0;

		return keykeeper->Lock();
	}

	return ECODE_SUCCESS;
}

int Engine::HandleRecognitionFailure()
{
	failedRecognitionCount++;

	if (failedRecognitionCount > failedRecognitionsThreshold) {
		failedRecognitionCount = 0;

		return keykeeper->Lock();
	}

	return ECODE_SUCCESS;
}

int Engine::DrawFaceFrames(Mat& frame, std::vector<Rect>& detectedFaces)
{
	size_t currentIndex = 0;
	int currentArea = 0;

	for (currentIndex = 0; currentIndex < detectedFaces.size(); currentIndex++) 
	{
		Point pt1 = Point(detectedFaces[currentIndex].x, detectedFaces[currentIndex].y);
		Point pt2 = Point(detectedFaces[currentIndex].x + detectedFaces[currentIndex].height, detectedFaces[currentIndex].y + detectedFaces[currentIndex].width);

		rectangle(frame, pt1, pt2, 8);
	}

	return ECODE_SUCCESS;
}
