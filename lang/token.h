#pragma once
#include <string>

struct Token {
    enum TokenType {
        COLON, // :
        INT, FLOAT, STRING, BOOL,
        IDENTIFIER, // variable, function and etc
        BUILTIN_FUNC, // 
        // KEYWORD,
        PLUS, MINUS, MULTIPLY, DIVIDE, FLOORDIV, MODULO, POWER,
        LPAREN, RPAREN, // ( )
        LSQUARE, RSQUARE, // [ ]
        LCURLY, RCURLY, // { }
        COMMA, DOT, // , .
        EQUALS, // =
        // PLUSEQUALS, MINUSEQUALS, MULTEQUALS, DIVEQUALS, FLOORDIVEQUALS, MODULOEQUALS, POWEREQUALS, EQUALITY, INEQUALITY,
        DOUBLEQUALS,
        // LESS, GREATER, LESSEQUAL, GREATEREQUAL,
        COMPARISON,
        AND, OR, NOT, IN,
        IF, ELIF, ELSE,
        WHILE, FOR,
        DEF, CLASS,
        NEWLINE, TAB,
        RAISE,
        INVALID,
        JUMP,
        END
    };

    TokenType type;
    std::string value;

    Token() : value("") {}
    Token(const TokenType& type, const std::string& value) : type(type), value(value) {}
};