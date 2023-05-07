#include "src/StateMachineDeterminator/CStateMachineDeterminator.h"
#include "src/StateMachines/CStateMachine.h"
#include <iostream>
#include <fstream>
#include <string>

struct Args
{
    std::string outputFileName;
    std::string inputFileName;
};

Args parseArgs(int argc, char** argv)
{
    if (argc != 3)
    {
        throw std::invalid_argument("Wrong arguments count specified. Should be: "
                                    "StateMachineDetermination <input file> <output file>");
    }

    Args args;

    args.inputFileName = argv[1];
    args.outputFileName = argv[2];

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

        CStateMachine stateMachine(input);

        stateMachine.save(std::cout);

        CStateMachine determinedMachine = CStateMachineDeterminator::determineStateMachine(stateMachine);

        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }
}
