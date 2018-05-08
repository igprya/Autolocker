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
	std::cout << "Supported commands: guard, sm, learn" << std::endl;
	std::cout << std::endl;

	while (true) {
		int operationResult = PromptOperation();
		HandleOperationResult(operationResult);
	}

	std::cout << "Press any key to exit...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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

	std::getline(std::cin, rawCommand);

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