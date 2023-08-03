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
