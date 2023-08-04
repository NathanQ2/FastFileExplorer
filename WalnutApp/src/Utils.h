#pragma once

#include <string>
#include <vector>

namespace FastFileExplorer
{
	std::vector<std::string> SplitString(const std::string s, char delim);
	std::string ParseSize(std::uintmax_t size);
	template <typename T>
	std::string to_string_with_precision(const T a_value, const int n = 6);
}