#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool isNumOrId(std::string& content)
{
    std::string identifier;
    std::size_t i = 0;

    if (content.at(0) == terminals::MINUS)
    {
        content = content.substr(1);
    }

    while (i < content.length() && std::isalpha(content.at(i)))
    {
        identifier += content.at(i++);
    }

    if (identifier == "num" || identifier == "id")
    {
        content = content.substr(identifier.length());
        return true;
    }

    return false;
}

bool checkF(std::string& content)
{
    CStrings::ltrim(content);

    if (content.at(0) == terminals::MINUS && content.at(1) == terminals::OPENING_BRACE)
    {
        content = content.substr(1);
    }

    if (content.at(0) == terminals::MINUS)
    {
        content = content.substr(1);
        RuleHandler const expHandler = checkRule(non_terminals::F);
        return expHandler(content);
    }

    if (content.at(0) == terminals::OPENING_BRACE)
    {
        RuleHandler const expHandler = checkRule(non_terminals::EXP);

        content = content.substr(1);
        if (!expHandler(content))
        {
            return false;
        }

        if (!content.empty() && content.at(0) == terminals::CLOSING_BRACE)
        {
            content = content.substr(1);
            return true;
        }
        else
        {
            return false;
        }
    }

    return isNumOrId(content);
}