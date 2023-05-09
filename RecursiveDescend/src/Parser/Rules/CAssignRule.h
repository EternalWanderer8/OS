#pragma once
#include "./RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <stdexcept>
#include <string>

bool checkAssign(std::string& content)
{
    CStrings::ltrim(content);

    if (content.rfind(terminals::ID, 0) != 0)
    {
        throw std::runtime_error("Assignment: missing identifier");
    }
    content.erase(0, terminals::ID.size());
    CStrings::ltrim(content);

    if (content.rfind(terminals::ASSIGMENT, 0) != 0)
    {
        throw std::runtime_error("Assignment: missing assignment symbol");
    }
    content.erase(0, terminals::ASSIGMENT.size());
    CStrings::ltrim(content);

    auto const semicolonPos = content.find(terminals::SEMICOLON);
    if (semicolonPos == std::string::npos)
    {
        throw std::runtime_error("Assignment: missing semicolon");
    }

    std::string exp = content.substr(0, semicolonPos);
    if (!checkRule(non_terminals::EXP)(exp))
    {
        throw std::runtime_error("Assignment: invalid exp");
    }
    content.erase(0, semicolonPos + 1);
    CStrings::ltrim(content);

    return content.empty();
}