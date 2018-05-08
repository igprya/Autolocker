#pragma once

#include <iostream>

namespace Helpers
{
	struct Settings
	{
		public:
			int EngineRpm() { return engineRpm; };
			int DetectionFailureThreshold() { return detectionFailureThreshold; };
			int RecognitionFailureThreshold() { return recognitionFailureThreshold; };
			int RecognitionInterval() { return recognitionInterval; };
			int ConfidenceThreshold() { return confidenceThreshold; };
			int DefaultCaptureDeviceIndex() { return defaultCaptureDevideIndex; };
			bool FeedWindow() { return feedWindow; };
			std::string CascadeTemplateFilePath() { return cascadeTemplateFilePath; };
			std::string AuthorizedFacesPath() { return authorizedFacesPath; };

			void SetEngineRpm(int value) { engineRpm = value; };
			void SetDetectionFailureThreshold(int value) { detectionFailureThreshold = value; };
			void SetRecognitionFailureThreshold(int value) { recognitionFailureThreshold = value; };
			void SetRecognitionInterval(int value) { recognitionInterval = value; }
			void SetConfidenceThreshold(int value) { confidenceThreshold = value; }
			void SetDefaultCaptureDeviceIndex(int value) { defaultCaptureDevideIndex = value; };
			void SetFeedWindow(bool value) { feedWindow = value; };
			void SetCascadeTemplateFilePath(std::string path) { cascadeTemplateFilePath = path; };
			void SetAuthorizedFacesPath(std::string path) { authorizedFacesPath = path; };

		private:
			int engineRpm = 50;
			int detectionFailureThreshold = 10;
			int recognitionFailureThreshold = 10;
			int recognitionInterval = 10;
			int confidenceThreshold = 0;			
			int defaultCaptureDevideIndex = 0;
			bool feedWindow = false;
			std::string cascadeTemplateFilePath = "patterns/haarcascade_frontalface_alt.xml";
			std::string authorizedFacesPath = "authorized_faces/";		
	};
}