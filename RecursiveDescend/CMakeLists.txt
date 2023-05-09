cmake_minimum_required(VERSION 3.23)
project(RecursiveDescend)

set(CMAKE_CXX_STANDARD 20)
set(MINGW Makefiles)

include_directories(src)
include_directories(src/Parser)

add_executable(RecursiveDescend
        src/Parser/Rules/RulesProvider/CRulesProvider.h
        src/Parser/Tokens/NonTerminals.h
        src/Parser/Tokens/Terminals.h
        src/Parser/Rules/CProgRule.h
        src/Parser/CParser.h
        src/Utils/CStrings.h
        RecursiveDescend.cpp
        src/Parser/Rules/CAssignRule.h
        src/Parser/Rules/CIdListRule.h
        src/Parser/Rules/CListstRule.h
        src/Parser/Rules/CWriteRule.h
        src/Parser/Rules/CTypeRule.h
        src/Parser/Rules/CReadRule.h
        src/Parser/Rules/CVarRule.h
        src/Parser/Rules/CExpRule.h
        src/Parser/Rules/CStRule.h
        src/Parser/Rules/CTRule.h
        src/Parser/Rules/CFRule.h
        src/Parser/Rules/RulesProvider/CRulesProvider.cpp)
