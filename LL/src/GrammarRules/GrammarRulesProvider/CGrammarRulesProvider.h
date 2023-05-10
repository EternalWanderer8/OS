#pragma once
#include "../CRule.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class CGrammarRulesProvider
{
public:
    using GrammarTable = std::vector<std::vector<std::string>>;

    static constexpr auto FIELD_VALUE_NULL = "NULL";
    static constexpr auto FIELD_VALUE_YES = "Yes";
    static constexpr auto FIELD_VALUE_NO = "No";
    static constexpr char CSV_SEPARATOR = ';';
    static constexpr int EMPTY_SYMBOL = 0;

    CRule::GrammarRules static getGrammarRules(const std::string& filename)
    {
        GrammarTable grammarTable = getGrammarTable(filename);

        CRule::GrammarRules grammarRules;

        for (const std::vector<std::string>& grammarLine : grammarTable)
        {
            std::stringstream numberStream(grammarLine[0]);
            size_t number;
            numberStream >> number;

            const auto symbol = grammarLine[1];

            const std::vector<char> leadingSymbols = parseLeadingSymbols(grammarLine[2]);

            bool shift = parseBoolField(grammarLine[3]);
            bool error = parseBoolField(grammarLine[4]);

            std::optional<std::size_t> pointer;
            if (grammarLine[5] != FIELD_VALUE_NULL)
            {
                pointer = parseNumericField(grammarLine[5]);
            }

            bool saveToStack = parseBoolField(grammarLine[6]);
            bool endParsing = parseBoolField(grammarLine[7]);

            CRule grammarRule;
            grammarRule.symbol = symbol,
            grammarRule.leadingSymbols = leadingSymbols,
            grammarRule.needToShift = shift,
            grammarRule.isError = error,
            grammarRule.pointer = pointer,
            grammarRule.needToStack = saveToStack,
            grammarRule.endParsing = endParsing;

            grammarRules[number] = grammarRule;
        }

        return grammarRules;
    }

private:
    GrammarTable static getGrammarTable(const std::string& filename)
    {
        std::ifstream input(filename);
        if (!input.is_open())
        {
            throw std::runtime_error("failed to open input file for reading");
        }

        GrammarTable grammarTable;
        std::string row;
        std::string value;

        while (std::getline(input, row))
        {
            std::istringstream iss(row);
            std::vector<std::string> rows;

            while (std::getline(iss, value, CSV_SEPARATOR))
            {
                rows.push_back(value);
            }

            grammarTable.push_back(rows);
        }

        return grammarTable;
    }

    std::vector<char> static parseLeadingSymbols(const std::string& field)
    {
        std::vector<char> result;
        for (auto&& currentSymbol : field)
        {
            if (currentSymbol == '0')
            {
                result.push_back(EMPTY_SYMBOL);
            }
            else if (currentSymbol != ',')
            {
                result.push_back(currentSymbol);
            }
        }

        return result;
    }

    bool static parseBoolField(const std::string& field)
    {
        if (field == FIELD_VALUE_YES)
        {
            return true;
        }
        if (field == FIELD_VALUE_NO)
        {
            return false;
        }

        throw std::runtime_error("Unknown field type");
    }

    size_t static parseNumericField(const std::string& field)
    {
        std::size_t result;

        std::stringstream stream(field);
        stream >> result;

        return result;
    }
};