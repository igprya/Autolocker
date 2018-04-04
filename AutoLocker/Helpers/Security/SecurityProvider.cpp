#include "stdafx.h"
#include "SecurityProvider.h"

namespace Helpers
{
	SecurityProvider::SecurityProvider(int detectionThreshold, int recognitionThreshold, int recognitionInterval)
	{
		this->detectionFailureThreshold = detectionThreshold;
		this->recognitionFailureCount = recognitionFailureThreshold;
		this->recognitionInterval = recognitionInterval;
		this->lockdownProvider = new WinLocker();		
		this->securityState = SecurityState::SECURE;
	}

	SecurityProvider::~SecurityProvider()
	{
		delete lockdownProvider;
	}
	
	bool SecurityProvider::IsRecognitionRequired(time_t lastRecognitionTime)
	{
		if (securityState == SecurityState::ALERT || securityState == SecurityState::LOCKDOWN) {
			return true;
		}

		double timeDifference = std::difftime(std::time(nullptr), lastRecognitionTime);

		if (timeDifference > recognitionInterval) {
			return true;
		}
		
		return false;
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

		if (detectionFailureCount > detectionFailureThreshold) 
		{
			SetSecurityState(SecurityState::ALERT);
			DropCounter(detectionFailureCount);
		}
	}

	void SecurityProvider::HandleDetectionSuccess()
	{
		DropCounter(detectionFailureCount);
	}

	void SecurityProvider::HandleRecognitionFailure()
	{
		IncCount(recognitionFailureCount);

		if (recognitionFailureCount > recognitionFailureThreshold)
		{
			SetLockdown();
			SetSecurityState(SecurityState::LOCKDOWN);
			DropCounter(recognitionFailureCount);
		}
	}

	void SecurityProvider::HandleRecognitionSuccess()
	{
		DropCounter(recognitionFailureCount);

		if (securityState == SecurityState::LOCKDOWN) {
			ReleaseLockdown();
		}

		SetSecurityState(SecurityState::SECURE);
	}

	SecurityState SecurityProvider::GetSecurityState()
	{
		return securityState;
	}

	void SecurityProvider::ForceLockdown()
	{
		SetLockdown();
		SetSecurityState(SecurityState::LOCKDOWN);
	}

	void SecurityProvider::SetSecurityState(SecurityState state)
	{
		securityState = state;
	}

	void SecurityProvider::SetLockdown()
	{
		lockdownProvider->Lock();
	}

	void SecurityProvider::ReleaseLockdown()
	{
		lockdownProvider->Unlock();
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