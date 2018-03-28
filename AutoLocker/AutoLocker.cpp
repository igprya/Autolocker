// AutoLocker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "ExitCodes.h"
#include "Engine.h"
#include "FaceCapturer.h"


void StartLocker();
void TeachLocker();

int main(int argc, char** argv)
{
	std::string mode;

	std::cout << "Mode: " << std::endl;
	std::cin >> mode;

	if (mode == "guard")
	{
		StartLocker();
	}
	else if (mode == "learn") {
		TeachLocker();
	}
	else {
		std::cout << "Unknown mode." << std::endl;
	}

	getchar();

	return 0;
}

void StartLocker()
{
	stringstream exitMessage;
	Engine* engine = new Engine();
	int engineResult = engine->Run();

	switch (engineResult)
	{
	case ECODE_SUCCESS: exitMessage << "ECODE_SUCCESS"; break;
	case ECODE_FAILURE: exitMessage << "ECODE_FAILURE"; break;
	case ERROR_ENGINE_FAILED_INIT: exitMessage << "ERROR_ENGINE_FAILED_INIT"; break;
	case ERROR_CAPTURER_FAILED_INIT: exitMessage << "ERROR_CAPTURER_FAILED_INIT"; break;
	case ERROR_DETECTOR_FAILED_INIT: exitMessage << "ERROR_DETECTOR_FAILED_INIT"; break;
	case ERROR_RECOGNIZER_FAILED_INIT: exitMessage << "ERROR_RECOGNIZER_FAILED_INIT"; break;
	default: exitMessage << "Exit code " << engineResult << " (unknown)"; break;
	}

	cout << exitMessage.str() << endl;
}

void TeachLocker()
{
	Helpers::FaceCapturer* faceCapturer = new Helpers::FaceCapturer();
	faceCapturer->InitFaceCaputrer();
	faceCapturer->Learn();
	
	delete faceCapturer;
}
