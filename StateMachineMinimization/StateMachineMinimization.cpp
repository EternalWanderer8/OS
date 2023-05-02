#include "src/StateMachines/CStateMachine.h"
#include "src/StateMachineMinimizer/CStateMachineMinimizer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

struct Args
{
    CStateMachine::Type type;
    std::string outputFileName;
    std::string inputFileName;
};

Args parseArgs(int argc, char** argv)
{
    std::string const MEALY_TYPE_STR = "mealy";
    std::string const MOORE_TYPE_STR = "moore";
    std::map<std::string, CStateMachine::Type> const TYPE_MAP = {
        { MEALY_TYPE_STR, CStateMachine::Type::MEALY },
        { MOORE_TYPE_STR, CStateMachine::Type::MOORE },
    };

    if (argc != 4)
    {
        throw std::invalid_argument("Wrong arguments count specified. Should be:\n"
                                    "StateMachineMinimization mealy <input file> <output file> or\n"
                                    "StateMachineMinimization moore <input file> <output file>");
    }

    Args args;
    std::string type = argv[1];
    auto it = TYPE_MAP.find(type);
    if (it == TYPE_MAP.end())
    {
        throw std::invalid_argument("Wrong state machine type specified");
    }
    args.type = it->second;
    args.inputFileName = argv[2];
    args.outputFileName = argv[3];

    return args;
}

std::ifstream getInput(const std::string& inputFileName)
{
    std::ifstream input;
    input.open(inputFileName);
    if (!input.is_open())
    {
        throw std::runtime_error("Failed to open file " + inputFileName + " for reading");
    }

    input.peek();

    if (input.eof())
    {
        throw std::runtime_error("Empty file provided");
    }

    return input;
}

std::ofstream getOutput(const std::string& outputFileName)
{
    std::ofstream output;
    output.open(outputFileName);
    if (!output.is_open())
    {
        throw std::runtime_error("Failed to open file " + outputFileName + " for writing");
    }

    return output;
}

int main(int argc, char** argv)
{
    try
    {
        Args args = parseArgs(argc, argv);

        std::ifstream input = getInput(args.inputFileName);

        CStateMachine stateMachine(input, args.type);

        stateMachine.save(std::cout);

        CStateMachineMinimizer::minimizeStateMachine(stateMachine);

        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }
}