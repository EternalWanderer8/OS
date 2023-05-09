#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <iostream>
#include <string>

bool checkProg(std::string& content)
{
    CStrings::ltrim(content);

    if (content.rfind(terminals::PROG, 0) == 0)
    {
        content.erase(0, terminals::PROG.size() + 1 + terminals::ID.size());

        if (checkRule(non_terminals::VAR)(content))
        {
            CStrings::ltrim(content);

            if (content.rfind(terminals::BEGIN, 0) == 0)
            {
                content.erase(0, terminals::BEGIN.size() + 1);
                if (checkRule(non_terminals::LISTST)(content))
                {
                    CStrings::ltrim(content);

                    if (content.rfind(terminals::END, 0) == 0)
                    {
                        content.erase(0, terminals::END.size());
                        CStrings::ltrim(content);
                        std::cout << content << std::endl;
                        return content.empty();
                    }
                }
            }
        }
    }
}