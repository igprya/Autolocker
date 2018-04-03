#pragma once
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>

#include "Helpers\Security\SecurityProvider.h"
#include "Processors\Capturer.h"
#include "Processors\Detector.h"
#include "Processors\Recognizer.h"

using namespace cv;

class Engine
{
	public:
		Engine();
		~Engine();
		int Start();

	private:
		int InitEngine();
		int DetectFace();
		int RecognizeFace();

		void ShowUI(Mat& frame, std::vector<Rect>& faces);
		void DrawFaceFrames(Mat& frame, std::vector<Rect>& detectedFaces);

		Helpers::SecurityProvider* securityProvider = nullptr;
		Processing::Capturer* capturer = nullptr;
		Processing::Detector* detector = nullptr;
		Processing::Recognizer* recognizer = nullptr;
};

