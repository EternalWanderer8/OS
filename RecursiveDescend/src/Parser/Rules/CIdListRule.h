#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkIdlist(std::string& content)
{
    CStrings::ltrim(content);
    if (content.rfind(terminals::ID, 0) == 0)
    {
        content.erase(0, terminals::ID.size());
        CStrings::ltrim(content);

        if (content.rfind(terminals::COMMA, 0) == 0)
        {
            content.erase(0, terminals::COMMA.size());
            if (!checkIdlist(content))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}