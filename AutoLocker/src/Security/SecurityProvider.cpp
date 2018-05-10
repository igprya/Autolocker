#include "stdafx.h"
#include "SecurityProvider.h"

namespace Security
{
	SecurityProvider::SecurityProvider(Helpers::Settings settings
		, ISecurable* securable
		, IBaseLocker* locker
		, Helpers::ILogger* logger)
	{
		this->settings = settings;
		this->securedObject = securable;
		this->lockdownProvider = locker;	
		this->securityLogger = logger;

		this->lastRecognitionTime = std::time(nullptr);
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
		
		return SecurityAction::DETECT;
	}


	bool SecurityProvider::TryRecognize(int& label, double& distance)
	{
		if (label == 1 && distance <= settings.ConfidenceThreshold()) {
			return true;
		}
		
		securityLogger->Debug("Beyond recognition threshold", std::to_string(distance));
		return false;
	}


	void SecurityProvider::HandleDetectionFailure()
	{
		detectionFailureCount++;

		if (detectionFailureCount > settings.DetectionFailureThreshold()) {
			SetSecurityState(SecurityState::ALERT);
		}

		securityLogger->Debug("Detection failure", std::to_string(detectionFailureCount));
	}


	void SecurityProvider::HandleDetectionSuccess()
	{
		detectionFailureCount = 0;

		if (GetRecognitionTimeGap() > settings.RecognitionInterval()) {
			SetSecurityState(SecurityState::ALERT);
		}

		securityLogger->Debug("Detection success");
	}


	void SecurityProvider::HandleRecognitionFailure()
	{
		recognitionFailureCount++;

		if (recognitionFailureCount > settings.RecognitionFailureThreshold()) {
			
			if (securityState != SecurityState::LOCKDOWN) {
				SetSecurityState(SecurityState::LOCKDOWN);
				SetLockdown();
			}
		}

		securityLogger->Debug("Recognition failure", std::to_string(recognitionFailureCount));
	}


	void SecurityProvider::HandleRecognitionSuccess()
	{
		recognitionFailureCount = 0;
		lastRecognitionTime = std::time(nullptr);

		if (securityState == SecurityState::LOCKDOWN) {
			ReleaseLockdown();
		}		

		SetSecurityState(SecurityState::SECURE);

		securityLogger->Debug("Recognition success");
	}


	void SecurityProvider::HandleMultilpleFaces(int faceCount)
	{
		std::stringstream ss;
		ss << "Multile faces (" << faceCount << ") detected";
		securityLogger->Warning(ss.str());
	}


	void SecurityProvider::ForceLockdown()
	{
		SetLockdown();
		SetSecurityState(SecurityState::LOCKDOWN);
	}


	void SecurityProvider::SetSecurityState(SecurityState newState)
	{
		if (newState != securityState)	{
			securityState = newState;

			if (securedObject) {
				securedObject->OnSecurityStateChange(GetRequiredAction());
			}
		}
	}


	void SecurityProvider::SetLockdown()
	{
		if (!settings.PreventLockdown()) {
			lockdownProvider->Lock();
			securityLogger->Log("Locked down");
		}
		else {
			securityLogger->Warning("Locked down, actual lockdown prevented by 'preventlockdown' setting");
		}
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
}