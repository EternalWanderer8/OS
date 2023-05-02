#pragma once
#include <vector>
#include <string>
#include <set>

struct CEquivalentClass
{
    std::vector<std::string> signals = {};
    std::set<std::string> ownStates = {};
};