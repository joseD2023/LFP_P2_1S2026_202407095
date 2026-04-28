cmake_minimum_required(VERSION 4.1)
project(Proyecto2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto2 main.cpp
        Lexer/LexicalAnalyzer.h
        Lexer/LexicalAnalyzer.cpp
        Model/Token.h
        Model/Token.cpp
        Errores/LexicalError.h
        Errores/LexicalError.cpp)
