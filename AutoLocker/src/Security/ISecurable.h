#pragma once

namespace Security
{
	enum SecurityState
	{ UNSECURED, SECURE, ALERT, LOCKDOWN };

	enum SecurityAction
	{ DETECT, RECOGNIZE };

	// An interface a class should implement to be "securable" by the SecurityProvider
	class ISecurable
	{
		public:
			/* 
			   Called by SecurityProvider to communicate a security state change to
			   the secured class and demand it to react to the change
			*/
			virtual void OnSecurityStateChange(SecurityAction requiredAction) = 0;
	};
}