#pragma once
#include <ctime>
#include <sstream>

#include "ISecurable.h"
#include "Lockers\IBaseLocker.h"

#include "..\Helpers\Loggers\ILogger.h"

namespace Security
{
	class SecurityProvider
	{
		public:
			SecurityProvider(int detectionThreshold
				, int recognitionThreshold
				, int recognitionInterval
				, int confidenceThreshold
				, ISecurable* securable
				, IBaseLocker* lockProvider
				, Helpers::ILogger* logProvider
				, bool preventLockdown = false);
			~SecurityProvider();
			bool TryAuthorize(int& label, double& distance);
			void HandleDetectionFailure();
			void HandleDetectionSuccess();
			void HandleAuthorizaitonFailure();
			void HandleAuthorizationSuccess();
			void HandleMultilpleFaces(int faceCount);
			void ForceLockdown();

		protected:			
			void SetSecurityState(SecurityState securityState);
			SecurityAction GetRequiredAction();
			void SetLockdown();
			void ReleaseLockdown();
		
			inline double GetRecognitionTimeGap();
			inline void IncCount(int& counter);
			inline void DropCounter(int& counter);

			int detectionFailureThreshold = 0;
			int recognitionFailureThreshold = 0;
			int recognitionInterval = 0;
			int confidenceThreshold = 0;
			time_t lastRecognitionTime = 0;

			int detectionFailureCount = 0;
			int recognitionFailureCount = 0;

			SecurityState securityState = SecurityState::UNSECURED;
			IBaseLocker* lockdownProvider = nullptr;
			ISecurable* securedObject = nullptr;
			Helpers::ILogger* securityLogger = nullptr;

			bool preventLockdown = false;
	};
}
