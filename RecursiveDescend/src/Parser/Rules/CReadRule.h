#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkRead(std::string& content)
{
    CStrings::ltrim(content);

    if (content.rfind(terminals::READ, 0) != 0)
    {
        throw std::runtime_error("Read: missing keyword");
    }
    content.erase(0, terminals::READ.size());
    CStrings::ltrim(content);

    if (content.rfind(terminals::OPENING_BRACE, 0) != 0)
    {
        throw std::runtime_error("Read: missing opening brace");
    }
    content.erase(0, 1);

    auto const bracePos = content.find(terminals::CLOSING_BRACE);
    if (bracePos == std::string::npos)
    {
        throw std::runtime_error("Read: missing closing brace");
    }

    std::string idList = content.substr(0, bracePos);
    if (!checkRule(non_terminals::IDLIST)(idList))
    {
        throw std::runtime_error("Read: invalid Id list");
    }
    content.erase(0, bracePos + 1);

    if (content.rfind(terminals::SEMICOLON, 0) != 0)
    {
        throw std::runtime_error("Read: missing semicolon");
    }
    content.erase(0, terminals::SEMICOLON.size());
    CStrings::ltrim(content);

    return content.empty();
}