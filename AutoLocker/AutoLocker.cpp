// AutoLocker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "ExitCodes.h"
#include "Engine.h"
#include "FaceCapturer.h"

int PromptOperation();
void HandleOperationResult(int);
int StartLocker();
int TeachLocker();

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
	int engineResult = engine->Run();
	
	return engineResult;
}

int TeachLocker()
{
	Helpers::FaceCapturer* faceCapturer = new Helpers::FaceCapturer();
	faceCapturer->InitFaceCaputrer();
	faceCapturer->Learn();
	
	delete faceCapturer;

	return ECODE_SUCCESS;
}

void HandleOperationResult(int operationResult)
{
	stringstream exitMessage;

	switch (operationResult)
	{
		case ECODE_SUCCESS: exitMessage << "ECODE_SUCCESS"; break;
		case ECODE_FAILURE: exitMessage << "ECODE_FAILURE"; break;
		case ECODE_ABORT: exitMessage << "ECODE_ABORT"; break;
		case ERROR_ENGINE_FAILED_INIT: exitMessage << "ERROR_ENGINE_FAILED_INIT"; break;
		case ERROR_CAPTURER_FAILED_INIT: exitMessage << "ERROR_CAPTURER_FAILED_INIT"; break;
		case ERROR_DETECTOR_FAILED_INIT: exitMessage << "ERROR_DETECTOR_FAILED_INIT"; break;
		case ERROR_RECOGNIZER_FAILED_INIT: exitMessage << "ERROR_RECOGNIZER_FAILED_INIT"; break;		
		default: exitMessage << "Exit code " << operationResult << " (unknown)"; break;
	}

	cout << exitMessage.str() << endl;
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
	else {
		std::cout << "Unknown mode." << std::endl;
		operationResult = ECODE_ABORT;
	}

	return operationResult;
}
