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


	int FaceCapturer::InitFaceCaputrer(Helpers::Settings settings)
	{
		int capturerResult = capturer->InitCapture(settings.DefaultCaptureDeviceIndex());
		int detectorResult = detector->InitDetection(settings.CascadeTemplateFilePath());
		authorizedFacesFolder = settings.AuthorizedFacesPath();

		return ECODE_SUCCESS;
	}


	int FaceCapturer::Learn()
	{
		int captureInterations = 3;
		int imageCount = 5;
		int capturedImages = 0;

		std::cout << "AutoLocker will now consequentally capture five images from the camera and consider them authorized to use the computer" << std::endl;

		while (capturedImages < imageCount)
		{
			system("pause");
			int captureResult = CaptureFace();

			if (captureResult == ECODE_SUCCESS) {
				capturedImages++;
			}
		}

		std::cout << "Capturing completed" << std::endl;

		return ECODE_SUCCESS;
	}


	int FaceCapturer::CaptureFace()
	{
		Mat currentFrame;
		Mat firstFace;
		std::vector<Mat> faces;

		std::cout << "Capturing face..." << std::endl;
				
		currentFrame = capturer->GetFrame();
		faces = detector->GetFaces(currentFrame);

		if (faces.empty()) {
			std::cout << "Error: no faces found on captured image" << std::endl;
			return ECODE_FAILURE;
		}

		cv::imshow("Captured face", currentFrame);
		waitKey(50);

		std::string response;
		std::cout << "Is this face valid? [Y/N] ";
		std::getline(std::cin, response);

		if (toupper(response[0]) == 'Y') {
			SaveImage(faces[0]);
		}
		else {
			std::cout << "Skipping this image." << std::endl << std::endl;;
			return ECODE_FAILURE;
		}

		cv::destroyWindow("Captured face");

		return ECODE_SUCCESS;
	}


	int FaceCapturer::SaveImage(Mat& imageMat)
	{
		std::string filePath;
		std::stringstream ss;
		ss << authorizedFacesFolder << time(nullptr) << ".bmp";
		filePath = ss.str();

		imwrite(filePath, imageMat);

		std::cout << "Face saved to " << filePath << std::endl << std::endl;

		return ECODE_SUCCESS;
	}
}