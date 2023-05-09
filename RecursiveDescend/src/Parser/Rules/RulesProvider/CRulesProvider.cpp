#include "../../Tokens/NonTerminals.h"
#include "CRulesProvider.h"
#include "../CAssignRule.h"
#include "../CIdListRule.h"
#include "../CListstRule.h"
#include "../CWriteRule.h"
#include "../CTypeRule.h"
#include "../CReadRule.h"
#include "../CProgRule.h"
#include "../CVarRule.h"
#include "../CExpRule.h"
#include "../CStRule.h"
#include "../CTRule.h"
#include "../CFRule.h"
#include <stdexcept>
#include <string>
#include <map>

using RuleHandler = std::function<bool(std::string&)>;

static inline std::map<std::string, RuleHandler> rules = {
    { non_terminals::ASSIGN, checkAssign },
    { non_terminals::IDLIST, checkIdlist },
    { non_terminals::LISTST, checkListst },
    { non_terminals::WRITE, checkWrite },
    { non_terminals::TYPE, checkType },
    { non_terminals::PROG, checkProg },
    { non_terminals::READ, checkRead },
    { non_terminals::VAR, checkVar },
    { non_terminals::EXP, checkExp },
    { non_terminals::ST, checkSt },
    { non_terminals::T, checkT },
    { non_terminals::F, checkF }
};

RuleHandler checkRule(const std::string& symbol)
{
    auto it = rules.find(symbol);
    if (it == rules.end())
    {
        throw std::runtime_error("No rule specified for symbol " + symbol);
    }

    return it->second;
}