#pragma once
#include <ctime>

#include <opencv2\core.hpp>

#include "ExitCodes.h"
#include "Capturer.h"
#include "Detector.h"
#include "Recognizer.h"
#include "StubLocker.h"

using namespace cv;

class Engine
{
	public:
		Engine();
		~Engine();
		int Run();

	private:
		int InitEngine();
		int InitProcessors();
		int EngineCycle();
		bool IsRecognitionRequired(time_t lastRecognition);
		int HandleDetectionFailure();
		int HandleRecognitionFailure();
		int DrawFaceFrames(Mat& frame, vector<Rect>& detectedFaces);

		int failedDetectionCount = 0;
		int failedDetectionsThreshold = 5;

		int recognitionInterval = 180;
		int failedRecognitionCount = 0;
		int failedRecognitionsThreshold = 10;

		Helpers::Keykeeper* keykeeper = NULL;
		
		Mat currentFrame;
		Processing::Capturer* capturer = NULL;
		Processing::Detector* detector = NULL;
		Processing::Recognizer* recognizer = NULL;
};

