#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkVar(std::string& content)
{
    CStrings::ltrim(content);

    if (content.rfind(terminals::VAR, 0) == 0)
    {
        content.erase(0, terminals::VAR.size() + 1);
        if (checkRule(non_terminals::IDLIST)(content))
        {
            CStrings::ltrim(content);
            if (content.rfind(terminals::COLON, 0) == 0)
            {
                content.erase(0, terminals::COLON.size() + 1);
                if (checkRule(non_terminals::TYPE)(content))
                {
                    CStrings::ltrim(content);
                    if (content.rfind(terminals::SEMICOLON, 0) == 0)
                    {
                        content.erase(0, terminals::SEMICOLON.size() + 1);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}