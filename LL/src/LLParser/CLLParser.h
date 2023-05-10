#pragma once
#include "../GrammarRules/CRule.h"
#include <algorithm>
#include <iostream>
#include <stack>

class CLLParser
{
public:
    CLLParser(CRule::GrammarRules&& grammarRules, std::string&& expression)
        : grammarRules(std::move(grammarRules))
        , expression(std::move(expression))
    {
    }

    void parse()
    {
        std::size_t inputPos = 0;
        std::size_t currentRulePos = 1;
        CRule currentRule = grammarRules.at(currentRulePos);

        std::stack<std::size_t> stack;

        while (!currentRule.endParsing || !stack.empty())
        {
            if (!charMatches(getExpressionChar(expression, inputPos), currentRule.leadingSymbols))
            {
                if (currentRule.isError)
                {
                    throw std::runtime_error("Invalid expression");
                }
                else
                {
                    currentRule = grammarRules.at(++currentRulePos);
                }

                continue;
            }

            if (currentRule.needToShift)
            {
                ++inputPos;
            }

            if (currentRule.needToStack)
            {
                stack.push(currentRulePos + 1);
            }

            if (currentRule.pointer.has_value())
            {
                currentRulePos = currentRule.pointer.value();
                currentRule = grammarRules.at(currentRulePos);
            }
            else if (!stack.empty())
            {
                currentRulePos = stack.top();
                currentRule = grammarRules.at(currentRulePos);
                stack.pop();
            }
            else
            {
                throw std::runtime_error("Invalid expression");
            }
        }
    }

private:
    static constexpr int EMPTY_SYMBOL = 0;
    CRule::GrammarRules grammarRules;
    std::string expression;

    char static getExpressionChar(const std::string& input, std::size_t position)
    {
        if (position == input.length())
        {
            return 0;
        }

        return input.at(position);
    }

    bool static charMatches(char ch, const std::vector<char>& leadingSymbols)
    {
        return std::any_of(
            leadingSymbols.begin(),
            leadingSymbols.end(),
            [ch](char symbol) {
                return ch == symbol || symbol == EMPTY_SYMBOL;
            }
        );
    }
};