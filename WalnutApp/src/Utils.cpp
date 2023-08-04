#include "Utils.h"

#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> FastFileExplorer::SplitString(const std::string s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

std::string FastFileExplorer::ParseSize(std::uintmax_t size)
{
	// convert the bytes that the size is to a string with a number and the correct size unit
	double newSize = 0.0f;
	std::string endLetter = "b";


	if (size < 1000)
	{
		newSize = size;
	}
	if (size >= 1000)
	{
		newSize = size / 1000.0f;
		endLetter = "kb";
	}
	if (size >= 1e+6)
	{
		newSize = size / 1e+6;
		endLetter = "mb";
	}
	if (size >= 1e+9)
	{
		newSize = size / 1e+9;
		endLetter = "gb";
	}
	if (size >= 1e+12)
	{
		newSize = size / 1e+12;
		endLetter = "tb";
	}
	if (size >= 1e+15)
	{
		newSize = size / 1e+15;
		endLetter = "pb";
	}


	return to_string_with_precision(newSize, 2) + endLetter;
}

template <typename T>
std::string FastFileExplorer::to_string_with_precision(const T a_value, const int n)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return std::move(out).str();
}