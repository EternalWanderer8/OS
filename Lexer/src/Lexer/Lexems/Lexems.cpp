#include "LexemType.h"
#include "Lexems.h"
#include <functional>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

const std::vector<std::string> keywords = {
    "int", "float", "double", "bool", "false", "true", "auto",
    "for", "while", "if", "else", "switch", "case", "default",
    "const", "break", "continue", "return", "throw"
};

const std::vector<std::string> logicals = {
    ">", "<", "!=", "==", ">=",
    "<=", "&&", "||",
};

const std::vector<std::string> separators = {
    ";", "(", ")", "{", "}",
    "[", "]", "::", ":",
};

bool checkKeyword(const std::string& data)
{
    return std::find(keywords.begin(), keywords.end(),data) != keywords.end();
}

bool checkLogical(const std::string& data)
{
    return std::find(logicals.begin(), logicals.end(),data) != logicals.end();
}

bool checkSeparator(const std::string& data)
{
    return std::find(separators.begin(), separators.end(),data) != separators.end();
}

bool checkComment(const std::string& data)
{
    return data == "//";
}

bool checkIdentifier(const std::string& data)
{
    auto identifierIterator = std::find_if(data.begin() + 1, data.end(), [](char c) {
        return !(std::isalpha(c) || std::isdigit(c));
    });

    return std::isalpha(data[0]) && identifierIterator == data.end();
}

bool checkNumber(const std::string& data)
{
    bool isHexadecimal = false;
    bool isBinary = false;
    bool isFloat = false;
    bool isOctal = false;

    if (data.empty())
    {
        return false;
    }

    for (size_t index = 0; index < data.size(); index++)
    {
        if ((isBinary && std::isdigit(data[index]) && data[index] <= '1')
            || (isOctal && std::isdigit(data[index]) && data[index] <= '7')
            || (!isBinary && !isOctal && std::isdigit(data[index])))
        {
            continue;
        }

        if ((isHexadecimal && std::isalpha(data[index]) && (data[index] <= 'f' && data[index] >= 'a' || data[index] <= 'F' && data[index] >= 'A')))
        {
            continue;
        }

        if (data[index] == '.' && isFloat)
        {
            return false;
        }

        if (data[index] == '.' && !isFloat)
        {
            isFloat = true;
            continue;
        }

        if (data.size() > 2 && index == 1 && data[0] == '0' && (data[1] == 'x' || data[1] == 'b' || data[1] == 'e'))
        {
            isHexadecimal = data[1] == 'x';
            isBinary = data[1] == 'b';
            isOctal = data[1] == 'e';
            continue;
        }

        return false;
    }

    return true;
}

bool checkString(const std::string& data)
{
    return data[0] == '\"' && data[data.size() - 1] == '\"';
}

bool checkAssigment(const std::string& data)
{
    return data == "=";
}

bool checkDivision(const std::string& data)
{
    return data == "/";
}

bool checkMultiplication(const std::string& data)
{
    return data == "*";
}

bool checkMinus(const std::string& data)
{
    return data == "-";
}

bool checkPlus(const std::string& data)
{
    return data == "+";
}

using Handler = std::function<bool(const std::string&)>;

const std::map<LexemType, Handler> rules = {
        {LexemType::MULTIPLICATION, checkMultiplication},
        {LexemType::IDENTIFIER, checkIdentifier},
        {LexemType::ASSIGNMENT, checkAssigment},
        {LexemType::SEPARATOR, checkSeparator},
        {LexemType::DIVISION, checkDivision},
        {LexemType::KEYWORD, checkKeyword},
        {LexemType::LOGICAL, checkLogical},
        {LexemType::COMMENT, checkComment},
        {LexemType::NUMBER, checkNumber},
        {LexemType::STRING, checkString},
        {LexemType::MINUS, checkMinus},
        {LexemType::PLUS, checkPlus}
};

LexemType getLexemType(const std::string& content)
{
    for (const auto& rule : rules)
    {
        if (rule.second(content))
        {
            return rule.first;
        }
    }

    return LexemType::ERROR;
}