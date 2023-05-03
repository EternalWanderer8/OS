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

        return machine;
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
};