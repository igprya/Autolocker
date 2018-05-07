#include "stdafx.h"
#include "SecurityProvider.h"

namespace Security
{
	SecurityProvider::SecurityProvider(int detectionThreshold
		, int recognitionThreshold
		, int recognitionInterval
		, ISecurable* securable
		, IBaseLocker* lockProvider
		, Helpers::ILogger* logProvider)
	{
		this->detectionFailureThreshold = detectionThreshold;
		this->recognitionFailureCount = recognitionFailureThreshold;
		this->recognitionInterval = recognitionInterval;
		this->lockdownProvider = lockProvider;	
		this->securedObject = securable;
		this->lastRecognitionTime = std::time(nullptr);
		this->securityLogger = logProvider;

		SetSecurityState(SecurityState::SECURE);
	}

	SecurityProvider::~SecurityProvider()
	{
		delete lockdownProvider;
	}
	
	SecurityAction SecurityProvider::GetRequiredAction()
	{
		if (securityState == SecurityState::ALERT || securityState == SecurityState::LOCKDOWN) {
			return SecurityAction::RECOGNIZE;
		}

		if (GetRecognitionTimeGap() > recognitionInterval) {
			return SecurityAction::RECOGNIZE;
		}
		
		return SecurityAction::DETECT;
	}

	bool SecurityProvider::TryAuthorize(int& label, double& distance)
	{
		if (label == 1 && distance <= 100) {
			return true;
		}

		return false;
	}

	void SecurityProvider::HandleDetectionFailure()
	{
		IncCount(detectionFailureCount);

		if (detectionFailureCount > detectionFailureThreshold) {
			SetSecurityState(SecurityState::ALERT);
			DropCounter(detectionFailureCount);
		}
	}

	void SecurityProvider::HandleDetectionSuccess()
	{
		DropCounter(detectionFailureCount);

		if (GetRecognitionTimeGap() > recognitionInterval) {
			SetSecurityState(SecurityState::ALERT);
		}
	}

	void SecurityProvider::HandleAuthorizaitonFailure()
	{
		IncCount(recognitionFailureCount);

		if (recognitionFailureCount > recognitionFailureThreshold) {
			
			if (securityState != SecurityState::LOCKDOWN) {
				SetLockdown();
			}

			SetSecurityState(SecurityState::LOCKDOWN);
			DropCounter(recognitionFailureCount);
		}
	}

	void SecurityProvider::HandleAuthorizationSuccess()
	{
		DropCounter(recognitionFailureCount);

		if (securityState == SecurityState::LOCKDOWN) {
			ReleaseLockdown();
		}

		lastRecognitionTime = std::time(nullptr);
		SetSecurityState(SecurityState::SECURE);
	}

	void SecurityProvider::HandleMultilpleFaces(int faceCount)
	{
		std::stringstream ss;
		ss << "Multile faces (" << faceCount << ") detected";

		securityLogger->Log(ss.str());
	}

	void SecurityProvider::ForceLockdown()
	{
		SetLockdown();
		SetSecurityState(SecurityState::LOCKDOWN);
	}

	void SecurityProvider::SetSecurityState(SecurityState state)
	{
		if (state != securityState)	{
			securityState = state;

			if (securedObject) {
				securedObject->SecurityStateChanged(GetRequiredAction());
			}
		}
	}

	void SecurityProvider::SetLockdown()
	{
		lockdownProvider->Lock();
		securityLogger->Log("Locked down");
	}

	void SecurityProvider::ReleaseLockdown()
	{
		lockdownProvider->Unlock();
		securityLogger->Log("Lockdown released");
	}

	inline double SecurityProvider::GetRecognitionTimeGap()
	{
		return std::difftime(std::time(nullptr), lastRecognitionTime);
	}

	void SecurityProvider::IncCount(int& counter)
	{
		counter++;
	}

	void SecurityProvider::DropCounter(int& counter)
	{
		counter = 0;
	}
}