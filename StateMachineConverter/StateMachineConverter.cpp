#include "src/Converter/MealyMooreConverter.h"
#include "src/StateMachines/CStateMachine.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

struct Args
{
    MealyMooreConverter::Mode mode;
    std::string outputFileName;
    std::string inputFileName;
};

Args parseArgs(int argc, char** argv)
{
    std::string const MEALY_TO_MOORE_STR = "mealy-to-moore";
    std::string const MOORE_TO_MEALY_STR = "moore-to-mealy";
    std::map<std::string, MealyMooreConverter::Mode> const MODE_MAP = {
        { MEALY_TO_MOORE_STR, MealyMooreConverter::Mode::MEALY_TO_MOORE },
        { MOORE_TO_MEALY_STR, MealyMooreConverter::Mode::MOORE_TO_MEALY },
    };

    if (argc != 4)
    {
        throw std::invalid_argument("Wrong arguments count specified. Should be:\n"
                                    "StateMachineConverter mealy-to-moore <input file> <output file> or\n"
                                    "StateMachineConverter moore-to-mealy <input file> <output file>");
    }

    Args args;
    std::string mode = argv[1];
    auto it = MODE_MAP.find(mode);
    if (it == MODE_MAP.end())
    {
        throw std::invalid_argument("Wrong mode specified");
    }
    args.mode = it->second;
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

        auto it = MealyMooreConverter::CONVENTION_TYPE_MAP.find(args.mode);
        if (it == MealyMooreConverter::CONVENTION_TYPE_MAP.end())
        {
            throw std::invalid_argument("Wrong mode specified");
        }

        CStateMachine::Type type = it->second;
        CStateMachine stateMachine(input, type);

        CStateMachine convertedMachine = MealyMooreConverter::convertStateMachine(stateMachine, args.mode);

        std::ofstream output = getOutput(args.outputFileName);

        convertedMachine.save(output);

        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }
}