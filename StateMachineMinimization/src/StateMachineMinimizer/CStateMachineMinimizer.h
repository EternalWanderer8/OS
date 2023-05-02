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
        std::vector<CEquivalentClass> currEqClasses = {};

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

            auto prevEqClassesIterator = std::find_if(
                prevEqClasses.begin(),
                prevEqClasses.end(),
                [&stateSignals](const CEquivalentClass& eqClass){
                    return eqClass.signals == stateSignals;
                }
            );

            if (prevEqClassesIterator == prevEqClasses.end())
            {
                CEquivalentClass eqClass;
                eqClass.signals = stateSignals;
                eqClass.ownStates.insert(states[i]);
                prevEqClasses.push_back(eqClass);
            }
            else
            {
                prevEqClassesIterator->ownStates.insert(states[i]);
            }
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

        return machine;
    }
};