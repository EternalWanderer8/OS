cmake_minimum_required(VERSION 3.23)
project(StateMachineConverter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(MINGW Makefiles)

add_executable(StateMachineConverter
        StateMachineConverter.cpp
        src/StateMachines/CStateMachineTransition.h
        src/Converter/MealyMooreConverter.h
        src/Utils/CSVParser.h
        src/StateMachines/CStateMachine.h)