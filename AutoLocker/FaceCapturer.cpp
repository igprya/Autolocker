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
		int captureInterations = 3;
		int imageCount = 5;
		int capturedImages = 0;

		Mat currentFrame;
		Mat firstFace;
		std::vector<Mat> faces;

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		while (capturedImages < imageCount)
		{
			std::cout << "Press any key when you're ready to capure face";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			for (int i = 0; i < captureInterations; i++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				currentFrame = capturer->GetFrame();

				imshow("dbg-capturer", currentFrame);
				waitKey(1000);			

				faces = detector->GetFaces(currentFrame);

				if (faces.size() > 0)
					break;
			}

			if (faces.size() == 0)
				return ECODE_FAILURE;

			firstFace = faces[0];

			SaveImage(firstFace);
			capturedImages++;

			faces.clear();
		}

		return ECODE_SUCCESS;
	}

	int FaceCapturer::SaveImage(Mat& imageMat)
	{
		std::string filePath;
		std::stringstream ss;
		ss << OWNER_FACES_FOLDER << time(nullptr) << ".jpg";
		filePath = ss.str();

		resize(imageMat, imageMat, Size(150,150));

		imwrite(filePath, imageMat);

		std::cout << "Face saved to " << filePath << std::endl;

		return ECODE_SUCCESS;
	}
}