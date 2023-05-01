#pragma once
#include "../StateMachines/CStateMachine.h"
#include <algorithm>
#include <utility>
#include <map>

class MealyMooreConverter
{
public:
    enum class Mode
    {
        MEALY_TO_MOORE,
        MOORE_TO_MEALY
    };

    static inline const std::map<Mode, CStateMachine::Type> CONVENTION_TYPE_MAP = {
        { MealyMooreConverter::Mode::MEALY_TO_MOORE, CStateMachine::Type::MEALY },
        { MealyMooreConverter::Mode::MOORE_TO_MEALY, CStateMachine::Type::MOORE }
    };

    static CStateMachine convertStateMachine(const CStateMachine& machine, Mode mode)
    {
        return mode == Mode::MOORE_TO_MEALY
            ? MealyMooreConverter::convertMooreToMealy(machine)
            : MealyMooreConverter::convertMealyToMoore(machine);
    }

private:
    static CStateMachine convertMooreToMealy(const CStateMachine& machine)
    {
        std::vector<std::string> signals = machine.getSignals();
        std::vector<std::string> states = machine.getStates();

        CStateMachine::TransitionsTable convertedTransitions = {};
        std::vector<std::string> convertedStates = {};

        char newStateChar = states[0][0] == 'q' ? 'S' : 'q';

        for (std::string& state : states)
        {
            convertedStates.push_back(newStateChar + state.erase(0, 1));
        }

        for (const CStateMachine::TransitionsTableLine& transitionsLine : machine.getTransitions())
        {
            CStateMachine::TransitionsTableLine convertedTransitionsLine = {};

            for (int i = 0; i < transitionsLine.size(); ++i)
            {
                CStateMachineTransition transition = transitionsLine[i];

                CStateMachineTransition newTransition;
                newTransition.state = newStateChar + transition.state.erase(0, 1);
                newTransition.signal = signals[i];

                convertedTransitionsLine.push_back(newTransition);
            }

            convertedTransitions.push_back(convertedTransitionsLine);
        }

        return {
            CStateMachine::Type::MEALY,
            std::move(machine.getAlphabet()),
            {},
            std::move(convertedStates),
            std::move(convertedTransitions)
        };
    }

    static CStateMachine convertMealyToMoore(const CStateMachine& machine)
    {
        CStateMachine::TransitionsTable transitions = machine.getTransitions();
        std::vector<std::string> states = machine.getStates();

        std::string newStateChar = states[0][0] == 'q' ? "S" : "q";

        std::vector<CStateMachineTransition> markedTransitions = {};

        for (const CStateMachine::TransitionsTableLine& transitionsLine : transitions)
        {
            for (const CStateMachineTransition& transition : transitionsLine)
            {
                if (std::find(markedTransitions.begin(), markedTransitions.end(), transition) == markedTransitions.end())
                {
                    markedTransitions.push_back(transition);
                }
            }
        }

        CStateMachine::TransitionsTable convertedTransitions(transitions.size());
        std::vector<std::string> convertedSignals = {};
        std::vector<std::string> convertedStates = {};

        for (int i = 0; i < markedTransitions.size(); ++i)
        {
            convertedSignals.push_back(markedTransitions[i].signal.has_value()
                ? markedTransitions[i].signal.value()
                : ""
            );

            convertedStates.push_back(newStateChar + std::to_string(i));

            int stateIndex = std::find(states.begin(), states.end(), markedTransitions[i].state) - states.begin();

            for (int j = 0; j < transitions.size(); ++j)
            {
                CStateMachineTransition convertedTransition;
                int markedStateIndex = std::find(
                    markedTransitions.begin(),
                    markedTransitions.end(),
                    transitions[j][stateIndex]
                ) - markedTransitions.begin();

                convertedTransition.state = newStateChar + std::to_string(markedStateIndex);
                convertedTransition.signal = std::nullopt;

                convertedTransitions[j].push_back(convertedTransition);
            }
        }

        return {
            CStateMachine::Type::MOORE,
            std::move(machine.getAlphabet()),
            std::move(convertedSignals),
            std::move(convertedStates),
            std::move(convertedTransitions)
        };
    }
};