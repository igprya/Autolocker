#pragma once
#include <ctime>
#include <sstream>

#include "ISecurable.h"
#include "Lockers\IBaseLocker.h"
#include "..\Helpers\Settings\Settings.h"
#include "..\Helpers\Loggers\ILogger.h"

namespace Security
{
	class SecurityProvider
	{
		public:
			SecurityProvider(Helpers::Settings settings
				, ISecurable* securable
				, IBaseLocker* locker
				, Helpers::ILogger* logger);
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
			inline double GetRecognitionTimeGap();
			void SetLockdown();
			void ReleaseLockdown();			

			Helpers::Settings settings;
			ISecurable* securedObject = nullptr;
			SecurityState securityState = SecurityState::UNSECURED;			

			IBaseLocker* lockdownProvider = nullptr;			
			Helpers::ILogger* securityLogger = nullptr;

			time_t lastRecognitionTime = 0;
			int detectionFailureCount = 0;
			int recognitionFailureCount = 0;
	};
}
