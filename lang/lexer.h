#pragma once
#include <vector>
#include <iostream>
#include "token.h"

class Lexer {
public:
    static std::vector<Token> parse(const std::string&);
private:
    static Token extract_number(const std::string&, std::size_t&);
    static Token extract_identifier(const std::string&, std::size_t&);
    static Token extract_string(const std::string&, std::size_t&);
    static Token extract_metachar(const std::string&, std::size_t&);
    static void skip_line(const std::string&, std::size_t&);
};