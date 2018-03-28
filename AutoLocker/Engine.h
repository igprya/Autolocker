#pragma once
#include <opencv2\core.hpp>
#include <ctime>

#include "Capturer.h"
#include "Detector.h"
#include "Recognizer.h"
#include "ExitCodes.h"
#include "StubLocker.h"

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

		int failedDetectionCount = 0;
		int failedDetectionsThreshold = 10;

		int recognitionInterval = 180;
		int failedRecognitionCount = 0;
		int failedRecognitionsThreshold = 10;

		Helpers::Keykeeper* keykeeper = NULL;
		
		cv::Mat* currentFrame = NULL;
		Processing::Capturer* capturer = NULL;
		Processing::Detector* detector = NULL;
		Processing::Recognizer* recognizer = NULL;
};

