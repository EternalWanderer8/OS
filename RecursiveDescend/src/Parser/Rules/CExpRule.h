#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkExp(std::string& content)
{
    CStrings::ltrim(content);

    RuleHandler const tHandler = checkRule(non_terminals::T);
    if (!tHandler(content))
    {
        return false;
    }

    while (content.length() > 0)
    {
        if (content.at(0) != terminals::PLUS)
        {
            break;
        }

        content = content.substr(1);
        if (!tHandler(content))
        {
            return false;
        }
    }

    return true;
}