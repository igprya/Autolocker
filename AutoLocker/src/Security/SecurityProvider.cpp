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


	bool SecurityProvider::TryAuthorize(int& label, double& distance)
	{
		if (label == 1 && distance <= settings.ConfidenceThreshold()) {
			return true;
		}
		
		return false;
	}


	void SecurityProvider::HandleDetectionFailure()
	{
		detectionFailureCount++;

		if (detectionFailureCount > settings.DetectionFailureThreshold()) {
			SetSecurityState(SecurityState::ALERT);
		}
	}


	void SecurityProvider::HandleDetectionSuccess()
	{
		detectionFailureCount = 0;

		if (GetRecognitionTimeGap() > settings.RecognitionInterval()) {
			SetSecurityState(SecurityState::ALERT);
		}
	}


	void SecurityProvider::HandleAuthorizaitonFailure()
	{
		recognitionFailureCount++;

		if (recognitionFailureCount > settings.RecognitionFailureThreshold()) {
			
			if (securityState != SecurityState::LOCKDOWN) {
				SetSecurityState(SecurityState::LOCKDOWN);
				SetLockdown();
			}
		}
	}


	void SecurityProvider::HandleAuthorizationSuccess()
	{
		recognitionFailureCount = 0;
		lastRecognitionTime = std::time(nullptr);

		if (securityState == SecurityState::LOCKDOWN) {
			ReleaseLockdown();
		}		

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


	void SecurityProvider::SetSecurityState(SecurityState newState)
	{
		if (newState != securityState)	{
			securityState = newState;

			if (securedObject) {
				securedObject->SecurityStateChanged(GetRequiredAction());
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
			securityLogger->Log("Locked down, actual lockdown prevented by 'preventlockdown' setting");
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