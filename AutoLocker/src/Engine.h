#pragma once
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>

#include "Helpers\Settings\Settings.h"
#include "Helpers\Loggers\ILogger.h"
#include "Helpers\Loggers\ConLogger.h"
#include "Security\Lockers\IBaseLocker.h"
#include "Security\Lockers\WinLocker.h"
#include "Security\ISecurable.h"
#include "Security\SecurityProvider.h"
#include "Processors\Capturer.h"
#include "Processors\Detector.h"
#include "Processors\Recognizer.h"

using namespace cv;

class Engine;
typedef int(Engine::*engine_fptr)();

class Engine : public Security::ISecurable
{
	public:
		Engine();
		~Engine();
		int InitEngine(Helpers::Settings settings, Security::IBaseLocker* locker, Helpers::ILogger* engineLogger);
		int Start();

	private:		
		int DetectFace();
		int RecognizeFace();

		void OnSecurityStateChange(Security::SecurityAction requiredAction);
		void SetNextAction(engine_fptr action);

		void ShowUI(Mat& frame, std::vector<Rect>& faces);
		void DrawFaceFrames(Mat& frame, std::vector<Rect>& detectedFaces);
		
		engine_fptr nextEngineAction = nullptr;
		Helpers::ILogger* engineLogger = nullptr;
		Security::SecurityProvider* securityProvider = nullptr;
		Processing::Capturer* capturer = nullptr;
		Processing::Detector* detector = nullptr;
		Processing::Recognizer* recognizer = nullptr;
		Helpers::Settings settings;

		bool isInitialized = false;
};

