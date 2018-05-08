#pragma once

#include <iterator>
#include <sstream>
#include <vector>

namespace Helpers
{
	class Utils
	{
		public:
			static std::vector<std::string> SplitString(std::string s);
			static std::vector<std::string> RemoveFirstItem(std::vector<std::string> input);
	};
}

