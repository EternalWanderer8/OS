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
        CStateMachine::TransitionsTable transitions = machine.getTransitions();
        std::vector<std::string> alphabet = machine.getAlphabet();
        std::vector<std::string> signals = machine.getSignals();
        std::vector<std::string> states = machine.getStates();

        CStateMachine::TransitionsTable determinedTransitions(alphabet.size());
        std::vector<CStateMachineTransition> uniqueCompoundStates= {};
        std::string newStateChar = states[0][0] == 'S' ? "A" : "S";

        if (!ecloses.empty())
        {
            for (const auto& eclose : ecloses)
            {
                uniqueCompoundStates.push_back(eclose.second);
            }
        }
        else
        {
            CStateMachineTransition firstOriginalState;
            firstOriginalState.states.insert(states[0]);
            if (signals[0] == FINAL_SIGNAL)
            {
                firstOriginalState.signal = FINAL_SIGNAL;
            }
            uniqueCompoundStates.push_back(firstOriginalState);
        }

        for (int i = 0; i < uniqueCompoundStates.size(); ++i)
        {
            for (int inputSymbolIndex = 0; inputSymbolIndex < alphabet.size(); ++inputSymbolIndex)
            {
                if (alphabet[inputSymbolIndex] == EMPTY_SYMBOL)
                {
                    continue;
                }

                CStateMachineTransition compoundTransition;

                for (const std::string& state : uniqueCompoundStates[i].states)
                {
                    int stateIndex = std::find(
                        states.begin(),
                        states.end(),
                        state
                    ) - states.begin();

                    CStateMachineTransition originalTransition = transitions[inputSymbolIndex][stateIndex];

                    for (const std::string& originalTransitionState : originalTransition.states)
                    {
                        if (originalTransitionState.empty())
                        {
                            continue;
                        }

                        auto eclosesIterator = ecloses.find(originalTransitionState);

                        if (eclosesIterator != ecloses.end())
                        {
                            for (const std::string& ecloseState : eclosesIterator->second.states)
                            {
                                insertStateIntoCompoundTransition(compoundTransition, ecloseState, states, signals);
                            }
                        }
                        else
                        {
                            insertStateIntoCompoundTransition(compoundTransition, originalTransitionState, states, signals);
                        }
                    }
                }

                if (!compoundTransition.states.empty())
                {
                    auto uniqueStatesIterator = std::find(
                        uniqueCompoundStates.begin(),
                        uniqueCompoundStates.end(),
                        compoundTransition
                    );

                    if (uniqueStatesIterator == uniqueCompoundStates.end())
                    {
                        uniqueCompoundStates.push_back(compoundTransition);
                        compoundTransition.states = { newStateChar + std::to_string(uniqueCompoundStates.size())};
                    }
                    else
                    {
                        compoundTransition.states = { newStateChar + std::to_string(uniqueStatesIterator - uniqueCompoundStates.begin())};
                    }
                }

                determinedTransitions[inputSymbolIndex].push_back(compoundTransition);
            }
        }

        std::vector<std::string> newSignals = {};
        std::vector<std::string> newStates = {};

        for (int i = 0; i < uniqueCompoundStates.size(); ++i)
        {
            newStates.push_back(newStateChar + std::to_string(i));
            newSignals.push_back(uniqueCompoundStates[i].signal.has_value() ? FINAL_SIGNAL : "");
        }

        alphabet.erase(std::find(
            alphabet.begin(),
            alphabet.end(),
            EMPTY_SYMBOL
        ));

        writeDeterminedTransitionsCard(std::cout, uniqueCompoundStates, newStateChar);

        return {
            std::move(alphabet),
            std::move(newSignals),
            std::move(newStates),
            std::move(determinedTransitions)
        };
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
            if (!emptyTransition.empty() && std::find(
                compoundTransition.states.begin(),
                compoundTransition.states.end(),
                emptyTransition) == compoundTransition.states.end()
            ) {
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

        for (const std::string& state : states)
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

    static void insertStateIntoCompoundTransition(
        CStateMachineTransition& compoundTransition,
        const std::string& state,
        const std::vector<std::string>& states,
        const std::vector<std::string>& signals
    ) {
        if (std::find(
            compoundTransition.states.begin(),
            compoundTransition.states.end(),
            state) == compoundTransition.states.end()
        ) {
            compoundTransition.states.insert(state);
        }

        if (!compoundTransition.signal.has_value())
        {
            int originalTransitionStateIndex = std::find(
                states.begin(),
                states.end(),
                state
            ) - states.begin();

            if (signals[originalTransitionStateIndex] == FINAL_SIGNAL)
            {
                compoundTransition.signal = FINAL_SIGNAL;
            }
        }
    }

    static void writeDeterminedTransitionsCard(
        std::ostream& output,
        const std::vector<CStateMachineTransition>& compoundStates,
        const std::string& newStateChar
    ) {
        for (int i = 0; i < compoundStates.size(); ++i)
        {
            output << newStateChar << i << " =";
            for (const std::string& state : compoundStates[i].states)
            {
                output << " " + state;
            }
            output << std::endl;
        }
        output << std::endl;
    }
};
