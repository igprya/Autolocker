#pragma once
#include <ctime>

#include "BaseLocker.h"
#include "WinLocker.h"

namespace Helpers
{
	enum SecurityState
	{ SECURE, ALERT, LOCKDOWN };

	class SecurityProvider
	{
		public:
			SecurityProvider(int detectionThreshold
				, int recognitionThreshold
				, int recognitionInterval);

			~SecurityProvider();

			bool IsRecognitionRequired(time_t lastRecognitionTime);
			bool TryAuthorize(int& label, double& distance);
			void HandleDetectionFailure();
			void HandleDetectionSuccess();
			void HandleRecognitionFailure();
			void HandleRecognitionSuccess();		
			SecurityState GetSecurityState();
			void ForceLockdown();

		protected:			
			void SetSecurityState(SecurityState securityState);
			void SetLockdown();
			void ReleaseLockdown();
		
			inline void IncCount(int& counter);
			inline void DropCounter(int& counter);

			int detectionFailureThreshold;
			int recognitionFailureThreshold;
			int recognitionInterval;

			int detectionFailureCount = 0;
			int recognitionFailureCount = 0;

			SecurityState securityState;
			BaseLocker* lockdownProvider;
	};
}
