// AutoLocker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <opencv2\imgcodecs.hpp>

#include "Engine.h"
#include "Helpers\FaceCapturer\FaceCapturer.h"
#include "Processors\Recognizer.h"

int PromptOperation();
void HandleOperationResult(int);
int StartLocker();
int TeachLocker();

int TestRecognition();

int main(int argc, char** argv)
{
	int operationResult = PromptOperation();
	HandleOperationResult(operationResult);

	std::cout << "Press any key to exit...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return 0;
}

int StartLocker()
{
	Engine* engine = new Engine();
	int engineResult = engine->Start();
	
	return engineResult;
}

int PromptOperation()
{
	std::string mode;
	int operationResult;

	std::cout << "Mode: ";
	std::cin >> mode;

	if (mode == "guard")
	{
		operationResult = StartLocker();
	}
	else if (mode == "learn") {
		operationResult = TeachLocker();
	}
	else if (mode == "debug") {
		operationResult = TestRecognition();
	}
	else {
		std::cout << "Unknown mode." << std::endl;
		operationResult = ECODE_FAILURE;
	}

	return operationResult;
}


void HandleOperationResult(int operationResult)
{
	std::stringstream exitMessage;

	switch (operationResult)
	{
		case ECODE_SUCCESS: exitMessage << "ECODE_SUCCESS"; break;
		case ECODE_FAILURE: exitMessage << "ECODE_FAILURE"; break;
		case ERROR_ENGINE_FAILED_INIT: exitMessage << "ERROR_ENGINE_FAILED_INIT"; break;
		case ERROR_CAPTURER_FAILED_INIT: exitMessage << "ERROR_CAPTURER_FAILED_INIT"; break;
		case ERROR_DETECTOR_FAILED_INIT: exitMessage << "ERROR_DETECTOR_FAILED_INIT"; break;
		case ERROR_RECOGNIZER_FAILED_INIT: exitMessage << "ERROR_RECOGNIZER_FAILED_INIT"; break;	
		case ERROR_CAPTURER_NOFRAME: exitMessage << "ERROR_CAPTURER_NOFRAME"; break;
		default: exitMessage << "Exit code " << operationResult << " (unknown)"; break;
	}

	std::cout << exitMessage.str() << std::endl;
}

static void read_csv(const std::string& filename, std::vector<Mat>& images, std::vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file) {
		std::string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	std::string line, path, classlabel;
	while (getline(file, line)) {
		std::stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, IMREAD_GRAYSCALE));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

int TeachLocker()
{
	Helpers::FaceCapturer* faceCapturer = new Helpers::FaceCapturer();
	faceCapturer->InitFaceCaputrer();
	faceCapturer->Learn();

	delete faceCapturer;

	return ECODE_SUCCESS;
}

int TestRecognition()
{
	std::vector<Mat> images;
	std::vector<int> labels;

	read_csv("D:/Temp/att_faces/facelist.txt", images, labels);
	Mat testSample = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	images.pop_back();
	labels.pop_back();

	Processing::Recognizer* rec = new Processing::Recognizer();
	rec->TrainModel(images, labels);

	int label;
	double confidence;

	rec->RecognizeFace(testSample, label, confidence);

	std::cout << "Recognized " << label << " with confidence " << confidence << std::endl;

	return ECODE_SUCCESS;
}