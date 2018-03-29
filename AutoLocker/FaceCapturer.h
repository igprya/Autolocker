#pragma once
#include <iostream>
#include <thread>
#include <chrono>

#include <opencv2\core.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>

#include "Globals.h"
#include "ExitCodes.h"
#include "Capturer.h"
#include "Detector.h"

using namespace Processing;

namespace Helpers
{
	class FaceCapturer
	{
		public:
			FaceCapturer();
			~FaceCapturer();
			int InitFaceCaputrer();
			int Learn();

		private:
			int SaveImage(Mat& face);

			Capturer* capturer;
			Detector* detector;
	};
}

