#pragma once
#include "Rules/RulesProvider/CRulesProvider.h"
#include "Tokens/NonTerminals.h"

struct CParser
{
    static void parse(std::string& data)
    {
        RuleHandler const handler = checkRule(non_terminals::PROG);
        if (!handler(data))
        {
            throw std::runtime_error("Invalid Program keyword");
        }
    }
};