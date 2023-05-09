#pragma once
#include "./CStateMachineTransition.h"
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