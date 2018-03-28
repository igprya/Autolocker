#include "stdafx.h"
#include "FaceCapturer.h"

using namespace cv;

namespace Helpers
{
	FaceCapturer::FaceCapturer()
	{
		capturer = new Capturer();
		detector = new Detector();
	}

	FaceCapturer::~FaceCapturer()
	{
		delete capturer;
		delete detector;
	}

	int FaceCapturer::InitFaceCaputrer()
	{
		int capturerResult = capturer->InitCapture(DEFAULT_CAPTURE_DEVICE_INDEX);
		int detectorResult = detector->InitDetection(CASCADE_TEMPLATE_FILE_PATH);

		return ECODE_SUCCESS;
	}

	int FaceCapturer::Learn()
	{
		int imageCount = 5;
		int capturedImages = 0;

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		while (capturedImages < imageCount)
		{
			std::cout << "Press any key when you're ready to capure face";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			Mat frame = capturer->GetFrame();
			Mat face = detector->GetFaces(frame)[0];

			SaveImage(face);
			capturedImages++;
		}

		return ECODE_SUCCESS;
	}

	int FaceCapturer::SaveImage(Mat& imageMat)
	{
		std::string filePath;
		std::stringstream ss;
		ss << OWNER_FACES_FOLDER << time(nullptr) << ".jpg";
		filePath = ss.str();

		imwrite(filePath, imageMat);

		std::cout << "Face saved to " << filePath << std::endl;

		return ECODE_SUCCESS;
	}
}