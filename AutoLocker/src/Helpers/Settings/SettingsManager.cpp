#include "stdafx.h"
#include "SettingsManager.h"

namespace Helpers
{
	void SettingsManager::ReadSettings()
	{
		std::ifstream settingsFile(".settings");
		std::string line;
		int lineIndex = 0;

		if (settingsFile.is_open()) {
			while (getline(settingsFile, line)) 
			{
				switch (lineIndex)
				{
					case 0: settings.SetEngineRpm(atoi(line.c_str())); break;
					case 1: settings.SetDetectionFailureThreshold(atoi(line.c_str())); break;
					case 2: settings.SetRecognitionFailureThreshold(atoi(line.c_str())); break;
					case 3: settings.SetRecognitionInterval(atoi(line.c_str())); break;
					case 4: settings.SetConfidenceThreshold(atoi(line.c_str())); break;
					case 5: settings.SetDefaultCaptureDeviceIndex(atoi(line.c_str())); break;
					case 6: settings.SetFeedWindow(atoi(line.c_str())); break;
					case 7: settings.SetCascadeTemplateFilePath(line); break;
					case 8: settings.SetAuthorizedFacesPath(line); break;
					default: break;
				}

				lineIndex++;
			}

			settingsFile.close();
			settingsRead = true;
		}
		else
		{
			std::cout << "Unable to read settings file." << std::endl;
		}

	}


	void SettingsManager::WriteSettings()
	{
		std::ofstream settingsFile(".settings");
		if (settingsFile.is_open()) {
			settingsFile << settings.EngineRpm() << std::endl;
			settingsFile << settings.DetectionFailureThreshold() << std::endl;
			settingsFile << settings.RecognitionFailureThreshold() << std::endl;
			settingsFile << settings.RecognitionInterval() << std::endl;
			settingsFile << settings.ConfidenceThreshold() << std::endl;
			settingsFile << settings.DefaultCaptureDeviceIndex() << std::endl;
			settingsFile << settings.FeedWindow() << std::endl;
			settingsFile << settings.CascadeTemplateFilePath() << std::endl;
			settingsFile << settings.AuthorizedFacesPath() << std::endl;

			settingsFile.close();
		}
		else
		{
			std::cout << "Unable to write settings file." << std::endl;
		}
	}


	Settings SettingsManager::GetSettings()
	{
		if (!settingsRead)
			ReadSettings();

		return settings;
	}


	int SettingsManager::ResolveCommand(std::vector<std::string> parsedCommand)
	{
		std::vector<std::string> arguments;
		std::string command;

		if (parsedCommand.empty()) {
			std::cout << "No command. Type 'sm help' to see the list of available commands" << std::endl;
			return ECODE_NONE;
		}

		command = parsedCommand[0];
		
		if (parsedCommand.size() > 1) {
			arguments = Utils::RemoveFirstItem(parsedCommand);
		}

		if (command == helpCommand) {
			PrintHelp();
		}
		else if (command == setCommand) {
			SetParameter(arguments);
		} 
		else if (command == getCommand) {
			PrintParameter(arguments);
		}
		else if (command == writeCommand) {
			WriteSettings();
		}
		else if (command == readCommand) {
			ReadSettings();
		}
		else {
			std::cout << "Invalid command " << command << std::endl;
			return ECODE_FAILURE;
		}

		return ECODE_SUCCESS;
	}


	void SettingsManager::SetParameter(std::vector<std::string> command)
	{
		if (command.empty()) {
			return;
		}

		std::string parameterName = command[0];
		std::string parameterValue = command[1];

		if (parameterValue.empty()) {
			return;
		}

		if (parameterName == engineRpmParameter) {
			int value = atoi(parameterValue.c_str());
			settings.SetEngineRpm(value);
		}
		else if (parameterName == detectionFailureThresholdParameter) {
			int value = atoi(parameterValue.c_str());
			settings.SetDetectionFailureThreshold(value);
		}
		else if (parameterName == recognitionFailureThresholdParameter) {
			int value = atoi(parameterValue.c_str());
			settings.SetRecognitionFailureThreshold(value);
		}
		else if (parameterName == recognitionIntervalParameter) {
			int value = atoi(parameterValue.c_str());
			settings.SetRecognitionInterval(value);
		}
		else if (parameterName == confidenceThresholdParameter) {
			int value = atoi(parameterValue.c_str());
			settings.SetConfidenceThreshold(value);
		}
		else if (parameterName == captureDeviceIndexParameter) {
			int value = atoi(parameterValue.c_str());
			settings.SetDefaultCaptureDeviceIndex(value);
		}
		else if (parameterName == feedWindowParameter) {
			int value = atoi(parameterValue.c_str());
			settings.SetFeedWindow(value);
		}
		else if (parameterName == cascadeTemplatePathParameter) {
			settings.SetCascadeTemplateFilePath(parameterValue);
		}
		else if (parameterName == authorizedFacePathParameter) {
			settings.SetAuthorizedFacesPath(parameterValue);
		}
		else {
			std::cout << "Invalid parameter " << parameterName << std::endl;
		}
	}


	void SettingsManager::PrintParameter(std::vector<std::string> command)
	{
		if (command.empty()) {
			return;
		}

		std::string parameterName = command[0];

		if (parameterName == engineRpmParameter) {
			std::cout << settings.EngineRpm() << std::endl;
		}
		else if (parameterName == detectionFailureThresholdParameter) {
			std::cout << settings.DetectionFailureThreshold() << std::endl;
		}
		else if (parameterName == recognitionFailureThresholdParameter) {
			std::cout << settings.RecognitionFailureThreshold() << std::endl;
		}
		else if (parameterName == recognitionIntervalParameter) {
			std::cout << settings.RecognitionInterval() << std::endl;
		}
		else if (parameterName == confidenceThresholdParameter) {
			std::cout << settings.ConfidenceThreshold() << std::endl;
		}
		else if (parameterName == captureDeviceIndexParameter) {
			std::cout << settings.DefaultCaptureDeviceIndex() << std::endl;
		}
		else if (parameterName == feedWindowParameter) {
			std::cout << settings.FeedWindow() << std::endl;
		}
		else if (parameterName == cascadeTemplatePathParameter) {
			std::cout << settings.CascadeTemplateFilePath() << std::endl;
		}
		else if (parameterName == authorizedFacePathParameter) {
			std::cout << settings.AuthorizedFacesPath() << std::endl;
		}
		else {
			std::cout << "Invalid parameter " << parameterName << std::endl;
		}
	}


	void SettingsManager::PrintHelp()
	{
		std::cout << "Avaliable commands:" << std::endl;
		std::cout << helpCommand << std::endl;
		std::cout << setCommand << std::endl;
		std::cout << getCommand << std::endl;
		std::cout << readCommand << std::endl;
		std::cout << writeCommand << std::endl;
		std::cout << std::endl;
		std::cout << "set/get arguments:" << std::endl;
		std::cout << engineRpmParameter << std::endl;
		std::cout << detectionFailureThresholdParameter << std::endl;
		std::cout << recognitionFailureThresholdParameter << std::endl;
		std::cout << recognitionIntervalParameter << std::endl;
		std::cout << confidenceThresholdParameter << std::endl;
		std::cout << captureDeviceIndexParameter << std::endl;
		std::cout << feedWindowParameter << std::endl;
		std::cout << cascadeTemplatePathParameter << std::endl;
		std::cout << authorizedFacePathParameter << std::endl;
	}
}
