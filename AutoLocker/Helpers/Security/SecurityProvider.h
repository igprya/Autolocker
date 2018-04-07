#pragma once
#include <ctime>

#include "ISecurable.h"
#include "BaseLocker.h"
#include "WinLocker.h"

namespace Helpers
{
	class SecurityProvider
	{
		public:
			SecurityProvider(int detectionThreshold
				, int recognitionThreshold
				, int recognitionInterval
				, ISecurable* securable);
			~SecurityProvider();
			bool TryAuthorize(int& label, double& distance);
			void HandleDetectionFailure();
			void HandleDetectionSuccess();
			void HandleRecognitionFailure();
			void HandleRecognitionSuccess();
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
			time_t lastRecognitionTime = 0;

			int detectionFailureCount = 0;
			int recognitionFailureCount = 0;

			SecurityState securityState = SecurityState::UNSECURED;
			BaseLocker* lockdownProvider = nullptr;
			ISecurable* securedObject = nullptr;
	};
}
