cmake_minimum_required(VERSION 3.23)
project(GrammarParser)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(src/StateMachines)
include_directories(src/Utils)

add_executable(GrammarParser
        src/StateMachineDeterminator/CStateMachineDeterminator.h
        src/StateMachines/CStateMachineTransition.h
        src/GrammarParser/CGrammarParser.h
        src/StateMachines/CStateMachine.h
        src/Utils/CStrings.h
        GrammarParser.cpp)
