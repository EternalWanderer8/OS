#include "src/GrammarRules/GrammarRulesProvider/CGrammarRulesProvider.h"
#include "src/LLParser/CLLParser.h"
#include <iostream>
#include <string>

struct Args
{
    std::string grammarRulesFileName;
    std::string inputFileName;
};

Args parseArgs(int argc, char** argv)
{
    if (argc != 3)
    {
        throw std::invalid_argument("Wrong arguments count specified. Should be: LL <grammar rules file> <input file>");
    }

    Args args;
    args.grammarRulesFileName = argv[1];
    args.inputFileName = argv[2];

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

int main(int argc, char** argv)
{
    try
    {
        Args args = parseArgs(argc, argv);

        std::ifstream input = getInput(args.inputFileName);

        std::string expression;

        std::getline(input, expression);

        CLLParser parser(CGrammarRulesProvider::getGrammarRules(args.grammarRulesFileName), std::move(expression));
        parser.parse();

        std::cout << "SUCCESS!" << std::endl;
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
    }
}