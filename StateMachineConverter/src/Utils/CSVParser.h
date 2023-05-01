#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class CSVParser
{
public:
    std::vector<std::string> static ParseLine(std::istream& input, char separator = ';')
    {
        std::vector<std::string> result;
        std::string valuesLine;
        std::string value;

        std::getline(input, valuesLine);
        std::istringstream valuesLineInput(valuesLine);

        while (std::getline(valuesLineInput, value, separator))
        {
            result.push_back(value);
        }

        return result;
    }
};