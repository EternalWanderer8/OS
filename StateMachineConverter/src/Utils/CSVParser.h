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
        std::string transitionsLine;
        std::string transition;

        std::getline(input, transitionsLine);
        std::istringstream transitionsLineInput(transitionsLine);

        while (std::getline(transitionsLineInput, transition, separator))
        {
            result.push_back(transition);
        }

        return result;
    }
};