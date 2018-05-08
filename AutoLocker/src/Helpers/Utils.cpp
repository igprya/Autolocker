#include "stdafx.h"
#include "Utils.h"

namespace Helpers
{
	std::vector<std::string> Utils::SplitString(std::string s)
	{
		std::stringstream ss(s);
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string> vstrings(begin, end);

		return vstrings;
	}


	std::vector<std::string> Utils::RemoveFirstItem(std::vector<std::string> input)
	{
		std::vector<std::string>::const_iterator second = input.begin() + 1;
		std::vector<std::string>::const_iterator last = input.end();
		return std::vector<std::string>(second, last);
	}
}
