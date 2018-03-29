#pragma once
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>

#include "ExitCodes.h"
#include "Capturer.h"
#include "Detector.h"
#include "Recognizer.h"
#include "WinLocker.h"

using namespace cv;

class Engine
{
	public:
		Engine();
		~Engine();
		int Start();

	private:
		int InitEngine();
		int EngineCycle();
		bool IsRecognitionRequired(time_t lastRecognition);
		int HandleDetectionFailure();
		int HandleRecognitionFailure();
		int DrawFaceFrames(Mat& frame, std::vector<Rect>& detectedFaces);

		int failedDetectionCount = 0;
		int failedDetectionsThreshold = ENGINE_DETECTION_FAILURE_THRESHOLD;

		int failedRecognitionCount = 0;
		int recognitionInterval = ENGINE_RECOGNITION_INTERVAL;
		int failedRecognitionsThreshold = ENGINE_RECOGNITION_FAILURE_THRESHOLD;

		Helpers::Keykeeper* keykeeper = NULL;
		
		Mat currentFrame;
		Processing::Capturer* capturer = NULL;
		Processing::Detector* detector = NULL;
		Processing::Recognizer* recognizer = NULL;
};

