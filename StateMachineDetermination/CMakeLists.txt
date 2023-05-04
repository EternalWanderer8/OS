cmake_minimum_required(VERSION 3.23)
project(StateMachineDetermination)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(src/StateMachines)
include_directories(src/Utils)

add_executable(StateMachineDetermination
        src/StateMachines/CStateMachine.h
        src/StateMachines/CStateMachineTransition.h
        src/Utils/CSVParser.h
        StateMachineDetermination.cpp)
