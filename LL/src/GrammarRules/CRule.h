#pragma once
#include <unordered_map>
#include <optional>
#include <string>
#include <vector>

struct CRule
{
    using GrammarRules = std::unordered_map<std::size_t, CRule>;

    std::string symbol;
    std::vector<char> leadingSymbols;
    bool needToShift;
    bool isError;
    std::optional<std::size_t> pointer;
    bool needToStack;
    bool endParsing;
};