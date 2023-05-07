#pragma once
#include "../StateMachines/CStateMachineTransition.h"
#include "../StateMachines/CStateMachine.h"
#include <unordered_map>
#include <algorithm>
#include <vector>

class CStateMachineDeterminator
{
public:
    static CStateMachine determineStateMachine(const CStateMachine& machine)
    {
        std::unordered_map<std::string, CStateMachineTransition> ecloses = getEcloses(machine);

        for (auto& eclose : ecloses)
        {
            std::cout << eclose.first << ": ";

            for (auto& state : eclose.second.states) {
                std::cout << state << ", ";
            }

            if (eclose.second.signal.has_value() && eclose.second.signal.value() == FINAL_SIGNAL)
            {
                std::cout << "Final" << std::endl;
            }
            std::cout << std::endl;
        }

        return machine;
    }

private:
    static inline std::string FINAL_SIGNAL = "F";
    static inline std::string EMPTY_SYMBOL = "e";

    static void getEmptyTransitionState(
        const CStateMachine& machine,
        CStateMachineTransition& compoundTransition,
        const std::string& state
    ) {
        std::vector<std::string> alphabet = machine.getAlphabet();
        std::vector<std::string> states = machine.getStates();

        static int emptyTransitionsIndex = std::find(
            alphabet.begin(),
            alphabet.end(),
            EMPTY_SYMBOL
        ) - alphabet.begin();

        int stateIndex = std::find(states.begin(), states.end(), state) - states.begin();

        std::set<std::string> emptyTransitions = machine.getTransitions()[emptyTransitionsIndex][stateIndex].states;

        if (emptyTransitions.empty())
        {
            return;
        }

        for (const std::string& emptyTransition : emptyTransitions)
        {
            if (std::find(compoundTransition.states.begin(), compoundTransition.states.end(), emptyTransition) == compoundTransition.states.end())
            {
                compoundTransition.states.insert(emptyTransition);
                getEmptyTransitionState(machine, compoundTransition, emptyTransition);
            }
        }
    }

    static std::unordered_map<std::string, CStateMachineTransition> getEcloses(const CStateMachine& machine)
    {
        std::unordered_map<std::string, CStateMachineTransition> ecloses = {};
        std::vector<std::string> signals = machine.getSignals();
        std::vector<std::string> states = machine.getStates();
        std::vector<std::string> finalOriginalStates = {};

        for (int i = 0; i < signals.size(); ++i)
        {
            if (signals[i] == FINAL_SIGNAL)
            {
                finalOriginalStates.push_back(states[i]);
            }
        }

        for (const std::string& state : machine.getStates())
        {
            CStateMachineTransition compoundTransition;
            compoundTransition.states.insert(state);

            getEmptyTransitionState(machine, compoundTransition, state);

            if (compoundTransition.states.size() > 1)
            {
                auto transitionIterator = std::find_if(
                    compoundTransition.states.begin(),
                    compoundTransition.states.end(),
                    [&finalOriginalStates](const std::string& state)
                    {
                        return std::find(
                            finalOriginalStates.begin(),
                            finalOriginalStates.end(),
                            state
                        ) != finalOriginalStates.end();
                    }
                );

                if (transitionIterator != compoundTransition.states.end())
                {
                    compoundTransition.signal = FINAL_SIGNAL;
                }

                ecloses.insert(std::pair<std::string, CStateMachineTransition>(state, compoundTransition));
            }
        }

        return ecloses;
    }
};
