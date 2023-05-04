#include "src/StateMachines/CStateMachine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

struct Args
{
    std::string grammarSide;
    std::string outputFileName;
    std::string inputFileName;
};

Args parseArgs(int argc, char** argv)
{
    std::string const LEFT_SIDE_GRAMMAR_STR = "left";
    std::string const RIGHT_SIDE_GRAMMAR_STR = "right";

    if (argc != 4)
    {
        throw std::invalid_argument("Wrong arguments count specified. Should be:\n"
                                    "StateMachineMinimization mealy <input file> <output file> or\n"
                                    "StateMachineMinimization moore <input file> <output file>");
    }

    Args args;

    if (argv[1] != LEFT_SIDE_GRAMMAR_STR || argv[1] != RIGHT_SIDE_GRAMMAR_STR)
    {
        throw std::invalid_argument("Wrong state machine type specified");
    }
    args.grammarSide = argv[1];
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

        CStateMachine stateMachine(input, CStateMachine::Type::MOORE);

        stateMachine.save(std::cout);

        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }
}
