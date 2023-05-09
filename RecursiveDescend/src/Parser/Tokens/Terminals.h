#pragma once
#include <string>

namespace terminals
{
    const std::string WRITE = "WRITE";
    const std::string BEGIN = "BEGIN";
    const std::string PROG = "PROG";
    const std::string READ = "READ";
    const std::string END = "END";
    const std::string VAR = "VAR";

    const std::string STRING = "string";
    const std::string FLOAT = "float";
    const std::string BOOL = "bool";
    const std::string INT = "int";
    const std::string ID = "id";

    const std::string ASSIGMENT = ":=";
    const std::string SEMICOLON = ";";
    const char OPENING_BRACE = '(';
    const char CLOSING_BRACE = ')';
    const std::string COLON = ":";
    const std::string COMMA = ",";
    const char MULTIPLY = '*';
    const char MINUS = '-';
    const char PLUS = '+';
}