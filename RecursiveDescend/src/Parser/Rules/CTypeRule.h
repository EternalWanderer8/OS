#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkType(std::string& content)
{
    CStrings::ltrim(content);

    if (content.rfind(terminals::INT, 0) == 0)
    {
        content.erase(0, terminals::INT.size());
        return true;
    }
    CStrings::ltrim(content);

    if (content.rfind(terminals::FLOAT, 0) == 0)
    {
        content.erase(0, terminals::FLOAT.size());
        return true;
    }
    CStrings::ltrim(content);

    if (content.rfind(terminals::BOOL, 0) == 0)
    {
        content.erase(0, terminals::BOOL.size());
        return true;
    }
    CStrings::ltrim(content);

    if (content.rfind(terminals::STRING, 0) == 0)
    {
        content.erase(0, terminals::STRING.size());
        return true;
    }

    return false;
}