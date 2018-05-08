#pragma once
#include <iostream>
#include <thread>
#include <chrono>

#include <opencv2\core.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>

#include "..\Settings\Settings.h"
#include "..\..\Processors\Capturer.h"
#include "..\..\Processors\Detector.h"

using namespace Processing;

namespace Helpers
{
	class FaceCapturer
	{
		public:
			FaceCapturer();
			~FaceCapturer();
			int InitFaceCaputrer(Helpers::Settings settings);
			int Learn();

		private:
			int SaveImage(Mat& face);

			Capturer* capturer;
			Detector* detector;
			std::string authorizedFacesFolder;
	};
}

