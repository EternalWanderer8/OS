cmake_minimum_required(VERSION 3.23)
project(StateMachineMinimization)

set(CMAKE_CXX_STANDARD 17)
set(MINGW Makefiles)

include_directories(.)
include_directories(src)
include_directories(src/StateMachines)
include_directories(src/Utils)

add_executable(StateMachineMinimization
        src/StateMachines/CStateMachine.h
        src/StateMachines/CStateMachineTransition.h
        src/Utils/CSVParser.h StateMachineMinimization.cpp src/StateMachineMinimizer/CStateMachineMinimizer.h src/StateMachineMinimizer/EquivalentClass/CEquivalentClass.h)
