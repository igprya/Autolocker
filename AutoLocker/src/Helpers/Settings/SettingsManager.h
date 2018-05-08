#pragma once

#include <iostream>
#include <fstream>

#include "Settings.h"

#include "..\Utils.h"

namespace Helpers
{

	class SettingsManager
	{
		public:
			void ReadSettings();
			void WriteSettings();
			Settings GetSettings();
			int ResolveCommand(std::vector<std::string> parsedCommand);

		private:
			void SetParameter(std::vector<std::string> command);
			void PrintParameter(std::vector<std::string> command);
			void PrintHelp();

			Settings settings;
			bool settingsRead = false;

			const std::string helpCommand = "help";
			const std::string setCommand = "set";
			const std::string getCommand = "get";
			const std::string readCommand = "read";
			const std::string writeCommand = "write";
			const std::string engineRpmParameter = "engine_rpm";
			const std::string detectionFailureThresholdParameter = "detectionfailure_threshold";
			const std::string recognitionFailureThresholdParameter = "recognitionfailure_threshold";
			const std::string recognitionIntervalParameter = "recognition_interval";
			const std::string confidenceThresholdParameter = "confidence_threshold";
			const std::string captureDeviceIndexParameter = "capturedevide_index";
			const std::string feedWindowParameter = "feedwindow";
			const std::string cascadeTemplatePathParameter = "cascadetemplate_path";
			const std::string authorizedFacePathParameter = "authorizedfaces_path";
	};
}
