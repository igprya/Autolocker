#include "stdafx.h"
#include "SettingsManager.h"

namespace Helpers
{
	SettingsManager::SettingsManager()
	{
		ReadSettings();
	}


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
					case 7: settings.SetPreventLockdown(atoi(line.c_str())); break;
					case 8: settings.SetCascadeTemplateFilePath(line); break;
					case 9: settings.SetAuthorizedFacesPath(line); break;
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
			settingsFile << settings.PreventLockdown() << std::endl;
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
		int operationResult = ECODE_NONE;
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
			operationResult = SetParameter(arguments);
		} 
		else if (command == getCommand) {
			operationResult = PrintParameter(arguments);
		}
		else if (command == listCommand) {
			ListSettings();
		}
		else {
			std::cout << "Invalid command " << command << std::endl;
			operationResult = ECODE_FAILURE;
		}

		return operationResult;
	}


	int SettingsManager::SetParameter(std::vector<std::string> command)
	{
		if (command.empty()) {
			return ECODE_FAILURE;
		}

		int operationResult = ECODE_NONE;
		std::string parameterName = command[0];
		std::string parameterValue = command[1];

		if (parameterValue.empty()) {
			return ECODE_FAILURE;
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
		else if (parameterName == preventLockdownParameter) {
			int value = atoi(parameterValue.c_str());
			settings.SetPreventLockdown(value);
		}
		else if (parameterName == cascadeTemplatePathParameter) {
			settings.SetCascadeTemplateFilePath(parameterValue);
		}
		else if (parameterName == authorizedFacePathParameter) {
			settings.SetAuthorizedFacesPath(parameterValue);
		}
		else {
			std::cout << "Invalid parameter " << parameterName << std::endl;
			operationResult = ECODE_FAILURE;
		}

		WriteSettings();
		
		return operationResult;
	}


	int SettingsManager::PrintParameter(std::vector<std::string> command)
	{
		if (command.empty()) {
			return ECODE_FAILURE;
		}

		if (!settingsRead) {
			ReadSettings();
		}

		int operationResult = ECODE_NONE;
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
		else if (parameterName == preventLockdownParameter) {
			std::cout << settings.PreventLockdown() << std::endl;
		}
		else if (parameterName == cascadeTemplatePathParameter) {
			std::cout << settings.CascadeTemplateFilePath() << std::endl;
		}
		else if (parameterName == authorizedFacePathParameter) {
			std::cout << settings.AuthorizedFacesPath() << std::endl;
		}
		else {
			std::cout << "Invalid parameter " << parameterName << std::endl;
			operationResult = ECODE_FAILURE;
		}

		return operationResult;
	}


	void SettingsManager::PrintHelp()
	{
		std::cout << "Avaliable commands:" << std::endl;
		std::cout << helpCommand << std::endl;
		std::cout << setCommand << std::endl;
		std::cout << getCommand << std::endl;
		std::cout << listCommand << std::endl;
		std::cout << std::endl;
		std::cout << "set,get command arguments:" << std::endl;
		std::cout << "[int]  " << engineRpmParameter << std::endl;
		std::cout << "[int]  " << detectionFailureThresholdParameter << std::endl;
		std::cout << "[int]  " << recognitionFailureThresholdParameter << std::endl;
		std::cout << "[int]  " << recognitionIntervalParameter << std::endl;
		std::cout << "[int]  " << confidenceThresholdParameter << std::endl;
		std::cout << "[int]  " << captureDeviceIndexParameter << std::endl;
		std::cout << "[bool] " << feedWindowParameter << std::endl;
		std::cout << "[bool] " << preventLockdownParameter << std::endl;
		std::cout << "[str]  " << cascadeTemplatePathParameter << std::endl;
		std::cout << "[str]  " << authorizedFacePathParameter << std::endl;
	}


	void SettingsManager::ListSettings()
	{
		std::cout << engineRpmParameter << "\t\t\t"					<< settings.EngineRpm() << std::endl;
		std::cout << detectionFailureThresholdParameter << "\t"		<< settings.DetectionFailureThreshold() << std::endl;
		std::cout << recognitionFailureThresholdParameter << "\t"	<< settings.RecognitionFailureThreshold() << std::endl;
		std::cout << recognitionIntervalParameter << "\t\t"			<< settings.RecognitionInterval() << std::endl;
		std::cout << confidenceThresholdParameter << "\t\t"			<< settings.ConfidenceThreshold() << std::endl;
		std::cout << captureDeviceIndexParameter << "\t\t"			<< settings.DefaultCaptureDeviceIndex() << std::endl;
		std::cout << feedWindowParameter << "\t\t\t"				<< settings.FeedWindow() << std::endl;
		std::cout << preventLockdownParameter << "\t\t\t"				<< settings.PreventLockdown() << std::endl;
		std::cout << cascadeTemplatePathParameter << "\t\t"			<< settings.CascadeTemplateFilePath() << std::endl;
		std::cout << authorizedFacePathParameter << "\t\t"			<< settings.AuthorizedFacesPath() << std::endl;
	}
}
