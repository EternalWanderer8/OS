#pragma once
#include <optional>
#include <string>
#include <set>

struct CStateMachineTransition
{
    std::set<std::string> states = {};
    std::optional<std::string> signal = std::nullopt;

    bool operator==(const CStateMachineTransition& transition) const
    {
        return (states == transition.states) && (signal == transition.signal);
    }
};