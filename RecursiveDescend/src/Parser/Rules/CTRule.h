#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkT(std::string& content)
{
    CStrings::ltrim(content);

    RuleHandler const fHandler = checkRule(non_terminals::F);
    if (!fHandler(content))
    {
        return false;
    }

    while (content.length() > 0)
    {
        if (content.at(0) != terminals::MULTIPLY)
        {
            break;
        }

        content = content.substr(1);
        if (!fHandler(content))
        {
            return false;
        }
    }

    return true;
}