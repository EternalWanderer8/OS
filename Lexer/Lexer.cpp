#include "src/Lexer/Lexer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

struct Args
{
	std::string inputFilename;
};

Args ParseArgs(int argc, char** argv)
{
	if (argc != 2)
	{
		throw std::invalid_argument("invalid parameters");
	}

    Args args;
    args.inputFilename = argv[1];

	return args;
}

int main(int argc, char** argv)
{
    try
    {
        Args const args = ParseArgs(argc, argv);
        std::ifstream input(args.inputFilename);

        if (!input.is_open())
        {
            throw std::runtime_error("Cannot open input file");
        }

        std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(input);
        lexer->parse();
        lexer->printTokensCard(std::cout);

        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}