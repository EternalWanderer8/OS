#pragma once
#include <functional>
#include <string>

using RuleHandler = std::function<bool(std::string&)>;

RuleHandler checkRule(const std::string& symbol);