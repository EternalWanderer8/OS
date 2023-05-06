#pragma once
#include "./CStateMachineTransition.h"
#include "../Utils/CSVParser.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

class CStateMachine
{
public:
    using TransitionsTableLine = std::vector<CStateMachineTransition>;
    using TransitionsTable = std::vector<TransitionsTableLine>;

    explicit CStateMachine(std::istream& input)
    {
        std::vector<std::string> rawTransitionsHeader = CSVParser::ParseLine(input);
        rawTransitionsHeader.erase(rawTransitionsHeader.begin());
        signals = rawTransitionsHeader;

        rawTransitionsHeader = CSVParser::ParseLine(input);
        rawTransitionsHeader.erase(rawTransitionsHeader.begin());
        states = rawTransitionsHeader;

        std::vector<std::string> rawTransitionsLine = CSVParser::ParseLine(input);
        while (!rawTransitionsLine.empty())
        {
            alphabet.push_back(rawTransitionsLine[0]);
            rawTransitionsLine.erase(rawTransitionsLine.begin());
            TransitionsTableLine transitionsLine = {};

            for (const std::string& compoundTransition: rawTransitionsLine)
            {
                std::istringstream cts(compoundTransition);
                std::set<std::string> transitionStates = {};
                std::string state;

                while (std::getline(cts, state, STATE_SEPARATOR))
                {
                    transitionStates.insert(state);
                }

                CStateMachineTransition parsedTransition;
                parsedTransition.states = transitionStates;

                transitionsLine.push_back(parsedTransition);
            }

            transitions.push_back(transitionsLine);

            rawTransitionsLine = CSVParser::ParseLine(input);
        }
    }

    CStateMachine(
        std::vector<std::string>&& alphabet,
        std::vector<std::string>&& signals,
        std::vector<std::string>&& states,
        TransitionsTable&& transitions
    )
        : states(states)
        , signals(signals)
        , alphabet(alphabet)
        , transitions(transitions)
    {
    }

    void save(std::ostream& output) const
    {
        if (!signals.empty())
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
                output << ELEMENT_SEPARATOR;
                for (const std::string& state : transition.states)
                {
                    if (state != *transition.states.begin())
                    {
                        output << STATE_SEPARATOR;
                    }
                    output << state;
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
    static constexpr char STATE_SEPARATOR = ',';

    TransitionsTable transitions = {};
    std::vector<std::string> states = {};
    std::vector<std::string> signals = {};
    std::vector<std::string> alphabet = {};
};