// AutoLocker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

#include "Engine.h"
#include "Helpers\Utils.h"
#include "Helpers\Settings\SettingsManager.h"
#include "Helpers\FaceCapturer\FaceCapturer.h"

// Forward declarations
int StartLocker();
int TeachLocker();
int PromptOperation();
void HandleOperationResult(int);

// Global setting manager
Helpers::SettingsManager settingsManager;

int main(int argc, char** argv)
{
	std::cout << "AutoLocker" << std::endl;
	std::cout << "Supported commands: guard, sm, learn, exit" << std::endl;
	std::cout << std::endl;

	int operationResult = ECODE_NONE;

	while (operationResult != ECODE_TERMINATE) {
		operationResult = PromptOperation();
		HandleOperationResult(operationResult);
	}

	return 0;
}

int StartLocker()
{
	Engine* engine = new Engine();
	int engineResult = engine->Start(settingsManager.GetSettings());
	
	return engineResult;
}

int TeachLocker()
{
	Helpers::FaceCapturer* faceCapturer = new Helpers::FaceCapturer();
	faceCapturer->InitFaceCaputrer(settingsManager.GetSettings());
	faceCapturer->Learn();

	delete faceCapturer;

	return ECODE_SUCCESS;
}

int PromptOperation()
{
	std::string rawCommand;
	std::vector<std::string> parsedCommand;
	std::string command;
	int operationResult;

	getline(std::cin, rawCommand);

	parsedCommand = Helpers::Utils::SplitString(rawCommand);

	if (parsedCommand.empty()) {
		std::cout << "No command" << std::endl;
		return ECODE_FAILURE;
	}

	command = parsedCommand[0];
	
	if (command == "guard") {
		operationResult = StartLocker();
	}
	else if (command == "learn") {
		operationResult = TeachLocker();
	}
	else if (command == "sm") {
		operationResult = settingsManager.ResolveCommand(Helpers::Utils::RemoveFirstItem(parsedCommand));
	}
	else if (command == "exit") {
		operationResult = ECODE_TERMINATE;
	}
	else {
		std::cout << "Invalid command " << command << std::endl;
		operationResult = ECODE_NONE;
	}

	return operationResult;
}

void HandleOperationResult(int operationResult)
{
	if (operationResult == ECODE_NONE) {
		return;
	}		

	std::stringstream exitMessage;

	switch (operationResult)
	{
		case ECODE_SUCCESS: exitMessage << "Operation has completed successfully"; break;
		case ECODE_FAILURE: exitMessage << "Operation has encountered a problem and was terminated"; break;
		case ERROR_ENGINE_FAILED_INIT: exitMessage << "AutoLocker engine failed to initialize"; break;
		case ERROR_CAPTURER_FAILED_INIT: exitMessage << "Image capturer failed to initialize"; break;
		case ERROR_DETECTOR_FAILED_INIT: exitMessage << "Detector failed to initialize"; break;
		case ERROR_RECOGNIZER_FAILED_INIT: exitMessage << "Recognizer failed to initialize"; break;	
		case ERROR_CAPTURER_NOFRAME: exitMessage << "Failed to capture an image"; break;
		default: exitMessage << "Exit code " << operationResult << " (unknown)"; break;
	}

	std::cout << exitMessage.str() << std::endl;
}