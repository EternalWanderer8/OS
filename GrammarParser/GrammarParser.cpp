#include "src/StateMachineDeterminator/CStateMachineDeterminator.h"
#include "src/StateMachines/CStateMachine.h"
#include "GrammarParser/CGrammarParser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

struct Args
{
    CGrammarParser::GrammarSide grammarSide;
    std::string outputFileName;
    std::string inputFileName;
};

Args parseArgs(int argc, char** argv)
{
    std::string const LEFT_SIDE_GRAMMAR_STR = "left";
    std::string const RIGHT_SIDE_GRAMMAR_STR = "right";

    const std::map<std::string, CGrammarParser::GrammarSide> SIDE_MAP = {
        {RIGHT_SIDE_GRAMMAR_STR, CGrammarParser::GrammarSide::RIGHT},
        {LEFT_SIDE_GRAMMAR_STR, CGrammarParser::GrammarSide::LEFT}
    };

    if (argc != 4)
    {
        throw std::invalid_argument("Wrong arguments count specified. Should be:\n"
                                    "StateMachineMinimization left <input file> <output file> or\n"
                                    "StateMachineMinimization right <input file> <output file>");
    }

    Args args;

    auto it = SIDE_MAP.find(argv[1]);
    if (it == SIDE_MAP.end())
    {
        throw std::invalid_argument("Wrong state machine type specified");
    }
    args.grammarSide = it->second;
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
        std::ofstream output = getOutput(args.outputFileName);

        CStateMachine stateMachine = CGrammarParser::parseGrammar(input, args.grammarSide);

        CStateMachine determinedMachine = CStateMachineDeterminator::determineStateMachine(stateMachine, args.grammarSide);

        determinedMachine.save(output);

        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }
}
