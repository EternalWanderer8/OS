#pragma once
#include "RulesProvider/CRulesProvider.h"
#include "../Tokens/NonTerminals.h"
#include "../Tokens/Terminals.h"
#include "../Utils/CStrings.h"
#include <string>

bool checkWrite(std::string& content)
{
    CStrings::ltrim(content);
    if (content.rfind(terminals::WRITE, 0) != 0)
    {
        throw std::runtime_error("Cannot find `WRITE` in write");
    }
    content.erase(0, terminals::WRITE.size());
    CStrings::ltrim(content);

    if (content.rfind(terminals::OPENING_BRACE, 0) != 0)
    {
        throw std::runtime_error("Cannot find opening brace in write");
    }
    content.erase(0, 1);

    auto const bracePos = content.find(terminals::CLOSING_BRACE);
    if (bracePos == std::string::npos)
    {
        throw std::runtime_error("Cannot find closing brace in write");
    }

    std::string idList = content.substr(0, bracePos);
    if (!checkRule(non_terminals::IDLIST)(idList))
    {
        throw std::runtime_error("Id list is not correct in write");
    }
    content.erase(0, bracePos + 1);
    CStrings::ltrim(content);

    if (content.rfind(terminals::SEMICOLON, 0) != 0)
    {
        throw std::runtime_error("Cannot find semicolon in write");
    }
    content.erase(0, terminals::SEMICOLON.size());
    CStrings::ltrim(content);

    return content.empty();
}