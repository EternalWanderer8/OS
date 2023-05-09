#pragma once
#include "../StateMachines/CStateMachineTransition.h"
#include "../StateMachines/CStateMachine.h"
#include "../Utils/CStrings.h"
#include <iostream>
#include <sstream>

class CGrammarParser
{
public:
    enum class GrammarSide
    {
        RIGHT,
        LEFT
    };

    static CStateMachine parseGrammar(std::istream& input, GrammarSide grammarSide)
    {
        std::string grammarLine;
        std::vector<std::string> states = {};
        std::vector<std::string> signals = {};
        std::vector<std::string> alphabet = {};
        CStateMachine::TransitionsTable transitions = {};

        while (std::getline(input, grammarLine))
        {
            std::istringstream grammarRule(grammarLine);
            parseGrammarRule(grammarRule, grammarSide, states, signals, alphabet, transitions);
        }

        return {
            std::move(alphabet),
            std::move(signals),
            std::move(states),
            std::move(transitions)
        };
    }

private:
    static constexpr char GRAMMAR_TRANSITION_SEPARATOR = '|';
    static inline std::string FINAL_SIGNAL = "F";
    static inline std::string EDGE_STATE = "H";

    static void parseGrammarRule(
        std::istream& grammarRule,
        GrammarSide grammarSide,
        std::vector<std::string>& states,
        std::vector<std::string>& signals,
        std::vector<std::string>& alphabet,
        CStateMachine::TransitionsTable& transitions
    ) {
        std::string fromState;
        std::string tempTrash;

        grammarRule >> fromState;
        updateStates(fromState, states, signals, alphabet, transitions, grammarSide);

        grammarRule >> tempTrash;

        std::string transition;
        while (std::getline(grammarRule, transition, GRAMMAR_TRANSITION_SEPARATOR))
        {
            CStrings::trim(transition);

            std::string toState;
            std::string inputChar;

            if (transition.size() == 1)
            {
                toState = EDGE_STATE;
                inputChar = transition;
            }
            else
            {
                if (grammarSide == GrammarSide::LEFT)
                {
                    toState = transition.substr(0, 1);
                    inputChar = transition.substr(1, transition.length() - 1);
                }
                else
                {
                    inputChar = transition.substr(0, 1);
                    toState = transition.substr(1, transition.length() - 1);
                }
            }

            updateStates(toState, states, signals, alphabet, transitions, grammarSide);
            updateAlphabet(inputChar, states, alphabet, transitions);

            if (grammarSide == GrammarSide::LEFT)
            {
                updateTransitions(toState, fromState, inputChar, states, alphabet, transitions);
            }
            else
            {
                updateTransitions(fromState, toState, inputChar, states, alphabet, transitions);
            }
        }
    }

    static void updateStates(
        const std::string& state,
        std::vector<std::string>& states,
        std::vector<std::string>& signals,
        std::vector<std::string>& alphabet,
        CStateMachine::TransitionsTable& transitions,
        GrammarSide grammarSide
    ) {
        if (std::find(states.begin(), states.end(), state) == states.end())
        {
            if ((grammarSide == GrammarSide::LEFT && states.empty())
                || (grammarSide == GrammarSide::RIGHT && state == EDGE_STATE))
            {
                signals.emplace_back(FINAL_SIGNAL);
            }
            else
            {
                signals.emplace_back("");
            }

            states.push_back(state);

            for (CStateMachine::TransitionsTableLine& transitionsLine : transitions)
            {
                CStateMachineTransition transition;
                transitionsLine.push_back(transition);
            }
        }
    }

    static void updateTransitions(
        const std::string& fromState,
        const std::string& toState,
        const std::string& inputChar,
        const std::vector<std::string>& states,
        const std::vector<std::string>& alphabet,
        CStateMachine::TransitionsTable& transitions
    ) {
        int stateIndex = std::find(states.begin(), states.end(), fromState) - states.begin();
        int inputCharIndex = std::find(alphabet.begin(), alphabet.end(), inputChar) - alphabet.begin();

        CStateMachineTransition& transition = transitions[inputCharIndex][stateIndex];

        if (std::find(transition.states.begin(), transition.states.end(), toState) == transition.states.end())
        {
            transition.states.insert(toState);
        }
    }

    static void updateAlphabet(
        const std::string& inputChar,
        std::vector<std::string>& states,
        std::vector<std::string>& alphabet,
        CStateMachine::TransitionsTable& transitions
    ) {
        if (std::find(alphabet.begin(), alphabet.end(), inputChar) == alphabet.end())
        {
            alphabet.push_back(inputChar);

            transitions.push_back(std::vector<CStateMachineTransition>(states.size()));
        }
    }
};