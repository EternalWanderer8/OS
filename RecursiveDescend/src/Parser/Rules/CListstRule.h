#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkListst(std::string& content)
{
    CStrings::ltrim(content);
    if (content.rfind(terminals::END, 0) == 0)
    {
        return true;
    }
    auto semicolonPos = content.find(terminals::SEMICOLON);
    if (semicolonPos != std::string::npos)
    {
        std::string st = content.substr(0, semicolonPos + 1);
        content.erase(0, st.size());
        CStrings::ltrim(content);
        if (checkRule(non_terminals::ST)(st))
        {
            if (!checkListst(content))
            {
                return false;
            }
            return true;
        }
    }
    return false;
}