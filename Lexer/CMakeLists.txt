cmake_minimum_required(VERSION 3.23)
project(Lexer)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lexer
        Lexer.cpp
        src/Lexer/Lexems/LexemType.h
        src/Lexer/Tokens/TokenType.h
        src/Lexer/Lexems/Lexems.h
        src/Lexer/Tokens/Token.h
        src/Lexer/Context/Position.h src/Lexer/Lexer.h src/Lexer/Lexems/Lexems.cpp)

