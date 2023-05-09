#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkSt(std::string& content)
{
    CStrings::ltrim(content);

    if (content.rfind(non_terminals::READ, 0) == 0)
    {
        return checkRule(non_terminals::READ)(content);
    }
    CStrings::ltrim(content);

    if (content.rfind(non_terminals::WRITE, 0) == 0)
    {
        return checkRule(non_terminals::WRITE)(content);
    }
    CStrings::ltrim(content);

    if (content.find(terminals::ASSIGMENT) != std::string::npos)
    {
        return checkRule(non_terminals::ASSIGN)(content);
    }
    return false;
}