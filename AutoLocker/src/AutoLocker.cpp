// AutoLocker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

#include "Engine.h"
#include "Helpers\Utils.h"
#include "Helpers\Settings\SettingsManager.h"
#include "Helpers\FaceCapturer\FaceCapturer.h"
#include "Helpers\Loggers\ConLogger.h";
#include "Security\Lockers\WinLocker.h"

// Forward declarations
int StartLocker();
int TeachLocker();
int PromptOperation();
void HandleOperationResult(int);

int main(int argc, char** argv)
{
	std::cout << "AutoLocker v. 0.0.2.1" << std::endl;
	std::cout << "OpenCV library https://opencv.org/" << std::endl;
	std::cout << "Application icon by http://www.iconbeast.com/" << std::endl;
	std::cout << std::endl;

	std::cout << "WARNING: DO NOT USE AS AN ACTUAL SECURITY MEASURE. This application doesn't provide any security whatsoever and ";
	std::cout << "can be easily disabled by a person with basic computer knowledge. Author provides no guarantees regarding this application's ";
	std::cout << "perfomance/functionality and isn't liable for any damage that may occur. Use at your own risk!" << std::endl;
	std::cout << std::endl;

	std::cout << "Supported commands: guard, sm, learn, exit" << std::endl;

	int operationResult = ECODE_NONE;

	while (operationResult != ECODE_TERMINATE) {
		std::cout << "> ";
		operationResult = PromptOperation();
		HandleOperationResult(operationResult);
	}

	return 0;
}

int StartLocker()
{
	Helpers::SettingsManager settingsManager;
	Helpers::LoggerVerbosityLevel logVerbosity = (Helpers::LoggerVerbosityLevel)settingsManager.GetSettings().LogVerbosityLevel();
	Helpers::ILogger* logger = new Helpers::ConLogger(logVerbosity);
	Security::WinLocker* locker = new Security::WinLocker();

	Engine* engine = new Engine();
	int initResult = engine->InitEngine(settingsManager.GetSettings(), locker, logger);

	if (initResult != ECODE_SUCCESS) {
		return initResult;
	}

	int engineResult = engine->Start();
	
	delete engine;
	delete locker;
	delete logger;
	
	return engineResult;
}

int TeachLocker()
{
	Helpers::SettingsManager settingsManager;
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

	std::cin.clear();
	std::getline(std::cin, rawCommand);

	parsedCommand = Helpers::Utils::SplitString(rawCommand);

	if (parsedCommand.empty()) {
		return ECODE_NONE;
	}

	command = parsedCommand[0];
	
	if (command == "guard") {
		operationResult = StartLocker();
	}
	else if (command == "learn") {
		operationResult = TeachLocker();
	}
	else if (command == "sm") {
		Helpers::SettingsManager settingsManager;
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
		case ECODE_SUCCESS: exitMessage << "Command has completed successfully"; break;
		case ECODE_FAILURE: exitMessage << "Command has encountered a problem and was terminated"; break;
		case ERROR_ENGINE_FAILED_INIT: exitMessage << "Command terminated: engine failed to initialize"; break;
		case ERROR_CAPTURER_FAILED_INIT: exitMessage << "Command terminated: image capturer failed to initialize"; break;
		case ERROR_DETECTOR_FAILED_INIT: exitMessage << "Command terminated: detector failed to initialize"; break;
		case ERROR_RECOGNIZER_FAILED_INIT: exitMessage << "Command terminated: recognizer failed to initialize"; break;	
		case ERROR_CAPTURER_NOFRAME: exitMessage << "Command terminated: failed to capture an image"; break;
		default: exitMessage << "Exit code " << operationResult << " (unknown)"; break;
	}

	std::cout << exitMessage.str() << std::endl;
}