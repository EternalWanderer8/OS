#pragma once
#include <optional>
#include <string>

struct CStateMachineTransition
{
    std::string state;
    std::optional<std::string> signal;

    bool operator==(const CStateMachineTransition& transition) const
    {
        return (state == transition.state) && (signal == transition.signal);
    }
};