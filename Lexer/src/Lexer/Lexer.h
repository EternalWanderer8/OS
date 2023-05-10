#pragma once
#include "Lexems/LexemType.h"
#include "Context/Position.h"
#include "Lexems/Lexems.h"
#include "Tokens/Token.h"
#include <iostream>
#include <sstream>
#include <vector>

class Lexer
{
public:
    explicit Lexer(std::istream& input)
        : input(input)
    {
    }

    void parse()
    {
        std::string line;
        while (std::getline(input, line))
        {
            currPosition.row++;
            std::istringstream iss(line);

            try
            {
                std::string content;

                while (iss >> content)
                {
                    currPosition.column = iss.tellg() == -1
                        ? line.size() - content.size()
                        : static_cast<int>(iss.tellg()) - content.size();

                    if (content.size() != 1 && content[content.size() - 1] == ';')
                    {
                        addToken(content.substr(0, content.size() - 1));
                        currPosition.column += content.size() - 1;
                        content = content.substr(content.size() - 1, 1);
                    }
                    addToken(content);
                }
            }
            catch (const std::logic_error& exception)
            {
            }
        }
    }

    void printTokensCard(std::ostream& output) const
    {
        for (const Token& token : tokens)
        {
            output << token.getTypeStr() << " (" << token.getContent() << ") [" << token.getPosition().row
                << ", " << token.getPosition().column << "]" << std::endl;
        }
    }

private:
    std::istream& input;
    Position currPosition;
    std::vector<Token> tokens;

    TokenType static getTokenType(const LexemType& lexemType)
    {
        switch (lexemType)
        {
            case LexemType::KEYWORD:
                return TokenType::KEYWORD;
            case LexemType::IDENTIFIER:
                return TokenType::IDENTIFIER;
            case LexemType::ASSIGNMENT:
                return TokenType::ASSIGNMENT;
            case LexemType::NUMBER:
                return TokenType::NUMBER;
            case LexemType::SEPARATOR:
                return TokenType::SEPARATOR;
            case LexemType::STRING:
                return TokenType::STRING;
            case LexemType::PLUS:
                return TokenType::PLUS;
            case LexemType::MINUS:
                return TokenType::MINUS;
            case LexemType::MULTIPLICATION:
                return TokenType::MULTIPLICATION;
            case LexemType::DIVISION:
                return TokenType::DIVISION;
            case LexemType::LOGICAL:
                return TokenType::LOGICAL;
            default:
                throw std::runtime_error("Error: Unexpected lexem");
        }
    }
    
    void addToken(const std::string& content)
    {
        LexemType lexemType = getLexemType(content);
        if (lexemType == LexemType::ERROR)
        {
            throw std::runtime_error("Error, unexpected token at (" + std::to_string(currPosition.row)
                                    + ", " + std::to_string(currPosition.column) + "); [..." + content + "...]");
        }
        if (lexemType == LexemType::COMMENT)
        {
            throw std::logic_error("Unnecessary token");
        }

        tokens.emplace_back(Token(content, getTokenType(lexemType), currPosition));
    }
};