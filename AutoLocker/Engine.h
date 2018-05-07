#pragma once
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>

#include "Helpers\ILogger.h"
#include "Helpers\ConLogger.h"
#include "Security\IBaseLocker.h"
#include "Security\WinLocker.h"
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
		Engine(bool useDebug = false);
		~Engine();
		int Start();

	private:
		int InitEngine();
		int DetectFace();
		int RecognizeFace();

		void SecurityStateChanged(Security::SecurityAction action);
		void SetNextAction(engine_fptr action);

		void ShowUI(Mat& frame, std::vector<Rect>& faces);
		void DrawFaceFrames(Mat& frame, std::vector<Rect>& detectedFaces);
		
		engine_fptr nextEngineAction = nullptr;
		Security::SecurityProvider* securityProvider = nullptr;
		Processing::Capturer* capturer = nullptr;
		Processing::Detector* detector = nullptr;
		Processing::Recognizer* recognizer = nullptr;
		bool debugMode = false;
};

