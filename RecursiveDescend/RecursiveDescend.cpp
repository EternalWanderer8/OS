#include "src/Parser/CParser.h"
#include <iostream>
#include <fstream>
#include <string>

struct Args
{
    std::string inputFileName;
};

Args ParseArgs(int argc, char** argv)
{
    if (argc != 2)
    {
        throw std::invalid_argument("Wrong arguments count specified. Should be: RecursiveDescend <input file>");
    }

    Args args;
    args.inputFileName = argv[1];

    return args;
}

int main(int argc, char** argv)
{
    try
    {
        Args args = ParseArgs(argc, argv);
        std::ifstream input(args.inputFileName);

        if (!input.is_open())
        {
            throw std::runtime_error("Failed to open file " + args.inputFileName + " for reading");
        }
        std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        CParser::parse(content);

        std::cout << "SUCCESS" << std::endl;
        return 1;
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}