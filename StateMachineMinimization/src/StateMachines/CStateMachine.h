#pragma once
#include "./CStateMachineTransition.h"
#include "../Utils/CSVParser.h"
#include <iostream>
#include <utility>
#include <vector>

class CStateMachine
{
public:
    using TransitionsTableLine = std::vector<CStateMachineTransition>;
    using TransitionsTable = std::vector<TransitionsTableLine>;

    enum class Type
    {
        MEALY,
        MOORE
    };

    CStateMachine(std::istream& input, Type type)
    {
        this->type = type;
        std::vector<std::string> rawTransitionsHeader = CSVParser::ParseLine(input);
        rawTransitionsHeader.erase(rawTransitionsHeader.begin());

        if (type == Type::MOORE)
        {
            signals = rawTransitionsHeader;

            rawTransitionsHeader = CSVParser::ParseLine(input);
            rawTransitionsHeader.erase(rawTransitionsHeader.begin());
        }

        states = rawTransitionsHeader;

        std::vector<std::string> rawTransitionsLine = CSVParser::ParseLine(input);
        while (!rawTransitionsLine.empty())
        {
            alphabet.push_back(rawTransitionsLine[0]);
            rawTransitionsLine.erase(rawTransitionsLine.begin());
            TransitionsTableLine transitionsLine = {};

            for (const std::string& transition: rawTransitionsLine)
            {
                CStateMachineTransition parsedTransition;

                if (type == Type::MEALY && !transition.empty())
                {
                    size_t signalSeparatorPos = transition.find(SIGNAL_SEPARATOR);

                    parsedTransition.state = transition.substr(0, signalSeparatorPos);
                    parsedTransition.signal = transition.substr(signalSeparatorPos + 1, transition.length());
                }
                else
                {
                    parsedTransition.signal = std::nullopt;
                    parsedTransition.state = transition;
                }

                transitionsLine.push_back(parsedTransition);
            }

            transitions.push_back(transitionsLine);

            rawTransitionsLine = CSVParser::ParseLine(input);
        }
    }

    CStateMachine(
        Type type,
        std::vector<std::string>&& alphabet,
        std::vector<std::string>&& signals,
        std::vector<std::string>&& states,
        TransitionsTable&& transitions
    )
        : type(type)
        , states(states)
        , signals(signals)
        , alphabet(alphabet)
        , transitions(transitions)
    {
    }

    void save(std::ostream& output) const
    {
        if (type == Type::MOORE)
        {
            for (const std::string& signal : signals)
            {
                output << ELEMENT_SEPARATOR << signal;
            }

            output << std::endl;
        }

        for (const std::string& state : states)
        {
            output << ELEMENT_SEPARATOR << state;
        }

        output << std::endl;

        for (int i = 0; i < alphabet.size(); ++i)
        {
            output << alphabet[i];

            for (const CStateMachineTransition& transition : transitions[i])
            {
                output << ELEMENT_SEPARATOR << transition.state;

                if (transition.signal.has_value())
                {
                    output << SIGNAL_SEPARATOR << transition.signal.value();
                }
            }

            output << std::endl;
        }
    }

    std::vector<std::string> getAlphabet() const
    {
        return alphabet;
    }

    std::vector<std::string> getSignals() const
    {
        return signals;
    }

    std::vector<std::string> getStates() const
    {
        return states;
    }

    TransitionsTable getTransitions() const
    {
        return transitions;
    }

private:
    static constexpr char ELEMENT_SEPARATOR = ';';
    static constexpr char SIGNAL_SEPARATOR = '/';

    Type type;
    TransitionsTable transitions = {};
    std::vector<std::string> states = {};
    std::vector<std::string> signals = {};
    std::vector<std::string> alphabet = {};
};