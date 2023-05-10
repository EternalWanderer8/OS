#pragma once
#include "../Context/Position.h"
#include "./TokenType.h"
#include <string>

class Token
{
public:
    Token(std::string content, TokenType type, Position position)
        : content(std::move(content))
        , type(type)
        , position(position)
    {
    }

    std::string getContent() const
    {
        return content;
    }

    TokenType getType() const
    {
        return type;
    }

    Position getPosition() const
    {
        return position;
    }

    std::string getTypeStr() const
    {
        switch (type)
        {
            case TokenType::KEYWORD:
                return "keyword";
            case TokenType::IDENTIFIER:
                return "identifier";
            case TokenType::ASSIGNMENT:
                return "assignment";
            case TokenType::NUMBER:
                return "number";
            case TokenType::SEPARATOR:
                return "separator";
            case TokenType::STRING:
                return "string";
            case TokenType::PLUS:
                return "plus";
            case TokenType::MINUS:
                return "minus";
            case TokenType::MULTIPLICATION:
                return "multiplication";
            case TokenType::DIVISION:
                return "division";
            case TokenType::LOGICAL:
                return "logical";
            default:
                return "Unexpected token";
        }
    }

private:
    std::string content;
    TokenType type;
    Position position;
};
