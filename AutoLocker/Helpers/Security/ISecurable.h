#pragma once

namespace Helpers
{
	enum SecurityState
	{ UNSECURED, SECURE, ALERT, LOCKDOWN };

	enum SecurityAction
	{ DETECT, RECOGNIZE };

	class ISecurable
	{
		public:
			virtual void SecurityStateChanged(SecurityAction action) = 0;
	};
}