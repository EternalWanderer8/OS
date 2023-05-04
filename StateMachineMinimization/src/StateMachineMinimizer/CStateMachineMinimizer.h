#pragma once
#include "src/StateMachineMinimizer/EquivalentClass/CEquivalentClass.h"
#include "../StateMachines/CStateMachine.h"
#include <vector>
#include <string>

class CStateMachineMinimizer
{
public:
    static CStateMachine minimizeStateMachine(const CStateMachine& machine)
    {
        CStateMachine::TransitionsTable transitions = machine.getTransitions();
        std::vector<std::string> signals = machine.getSignals();
        std::vector<std::string> states = machine.getStates();

        std::vector<CEquivalentClass> prevEqClasses = {};

        for (int i = 0; i < states.size(); ++i)
        {
            std::vector<std::string> stateSignals = {};

            if (machine.getType() == CStateMachine::Type::MEALY)
            {
                for (const CStateMachine::TransitionsTableLine& transitionsLine : transitions)
                {
                    stateSignals.push_back(transitionsLine[i].signal.has_value()
                        ? transitionsLine[i].signal.value()
                        : ""
                    );
                }
            }
            else
            {
                stateSignals.push_back(signals[i]);
            }

            linkStateToEqClass(prevEqClasses, stateSignals, states[i]);
        }

        while (true)
        {
            std::vector<CEquivalentClass> currEqClasses = {};

            for (int i = 0; i < states.size(); ++i)
            {
                std::vector<std::string> prevEqClassesSignals = {};

                for (const CStateMachine::TransitionsTableLine& transitionsLine : transitions)
                {
                    std::string transition = transitionsLine[i].state;

                    int prevEqClassIndex = std::find_if(
                            prevEqClasses.begin(),
                            prevEqClasses.end(),
                            [&transition](const CEquivalentClass& prevEqClass)
                            {
                                return std::find_if(
                                        prevEqClass.ownStates.begin(),
                                        prevEqClass.ownStates.end(),
                                        [&transition](const std::string& state) {
                                            return state == transition;
                                        }
                                ) != prevEqClass.ownStates.end();
                            }
                    ) - prevEqClasses.begin();

                    prevEqClassesSignals.push_back(std::to_string(prevEqClassIndex));
                }

                linkStateToEqClass(currEqClasses, prevEqClassesSignals, states[i]);
            }

            if (isEqClassesIterationsEqual(prevEqClasses, currEqClasses))
            {
                break;
            }

            prevEqClasses = currEqClasses;
        }

        int i = 0;

        for (const CEquivalentClass& eqClass : prevEqClasses)
        {
            std::cout << "EqClass" << i << ':' << std::endl;

            std::cout << "signals: ";
            for (const std::string& signal : eqClass.signals)
            {
                std::cout << signal << ", ";
            }

            std::cout << std::endl << "ownStates: ";
            for (const std::string& state : eqClass.ownStates)
            {
                std::cout << state << ", ";
            }

            std::cout << std::endl;
            i++;
        }

        std::cout << std::endl;

        return constructMinimizedStateMachine(prevEqClasses, machine);
    }
private:
    static void linkStateToEqClass(
        std::vector<CEquivalentClass>& eqClasses,
        const std::vector<std::string>& newEqClassSignals,
        const std::string& state
    ) {
        auto eqClassesIterator = std::find_if(
            eqClasses.begin(),
            eqClasses.end(),
            [&newEqClassSignals](const CEquivalentClass& eqClass){
                return eqClass.signals == newEqClassSignals;
            }
        );

        if (eqClassesIterator == eqClasses.end())
        {
            CEquivalentClass eqClass;
            eqClass.signals = newEqClassSignals;
            eqClass.ownStates.insert(state);
            eqClasses.push_back(eqClass);
        }
        else
        {
            eqClassesIterator->ownStates.insert(state);
        }
    }

    static bool isEqClassesIterationsEqual(
        const std::vector<CEquivalentClass>& prevEqClasses,
        const std::vector<CEquivalentClass>& currEqClasses
    ) {
        if (currEqClasses.size() != prevEqClasses.size())
        {
            return false;
        }

        for (const CEquivalentClass& currEqClass : currEqClasses)
        {
            auto prevEqClassesIterator = std::find_if(
                prevEqClasses.begin(),
                prevEqClasses.end(),
                [&currEqClass](const CEquivalentClass& prevEqClass) {
                    return currEqClass.ownStates == prevEqClass.ownStates;
                }
            );

            if (prevEqClassesIterator == prevEqClasses.end())
            {
                return false;
            }
        }

        return true;
    }

    static CStateMachine constructMinimizedStateMachine(
            const std::vector<CEquivalentClass>& finalEqClasses,
            const CStateMachine& machine
    ) {
        CStateMachine::TransitionsTable transitions = machine.getTransitions();
        std::vector<std::string> signals = machine.getSignals();
        std::vector<std::string> states = machine.getStates();
        CStateMachine::Type machineType = machine.getType();

        std::string newStateChar = states[0].substr(0, 1) == "q" ? "S" : "q";

        CStateMachine::TransitionsTable newTransitions(transitions.size());
        std::vector<std::string> newSignals = {};
        std::vector<std::string> newStates = {};

        for (int i = 0; i < finalEqClasses.size(); ++i)
        {
            newStates.push_back(newStateChar + std::to_string(i));

            int oldStateIndex = std::find(
                states.begin(),
                states.end(),
                *finalEqClasses[i].ownStates.begin()
            ) - states.begin();

            if (machineType == CStateMachine::Type::MOORE)
            {
                newSignals.push_back(signals[oldStateIndex]);
            }

            for (int j = 0; j < transitions.size(); ++j)
            {
                std::string oldTransition = transitions[j][oldStateIndex].state;

                std::cout << "old state at line " << j << " column (state) " << oldStateIndex << " " << oldTransition << std::endl;

                int ownEqClassIndex = std::find_if(
                    finalEqClasses.begin(),
                    finalEqClasses.end(),
                    [&oldTransition](const CEquivalentClass& eqClass) {
                        return std::find_if(
                            eqClass.ownStates.begin(),
                            eqClass.ownStates.end(),
                            [&oldTransition](const std::string& state){
                                return state == oldTransition;
                            }
                        ) != eqClass.ownStates.end();
                    }
                ) - finalEqClasses.begin();

                std::cout << "state owned by " << ownEqClassIndex << " eqClass " << std::endl;

                CStateMachineTransition newTransition;
                newTransition.state = newStateChar + std::to_string(ownEqClassIndex);

                if (machineType == CStateMachine::Type::MEALY)
                {
                    newTransition.signal = transitions[j][oldStateIndex].signal.has_value()
                        ? transitions[j][oldStateIndex].signal.value()
                        : "";
                }
                else
                {
                    newTransition.signal = std::nullopt;
                }

                newTransitions[j].push_back(newTransition);
            }
        }

        return {
            machineType,
            std::move(machine.getAlphabet()),
            std::move(newSignals),
            std::move(newStates),
            std::move(newTransitions)
        };
    }
};