#include <vector>
#include <string>
#include <iostream>
#include "lexer.h"
#include "alphabet.h"
#include "dict.h"

Token Lexer::extract_number(const std::string& input, std::size_t& pos) {
    std::string number;
    auto len = input.length();
    for (auto c = input[pos]; pos < len && Alphabet::isdigit(c); number.push_back(c), c = input[++pos]);
    if (Alphabet::isdot(input[pos])) {
        number.push_back(input[pos++]);
        for (auto c = input[pos]; pos < len && Alphabet::isdigit(c); number.push_back(c), c = input[++pos]);
        return Token(Token::FLOAT, number);
    }
    return Token(Token::INT, number);
}

Token Lexer::extract_identifier(const std::string& input, std::size_t& pos) {
    std::string identifier;
    auto len = input.length();
    for (auto c = input[pos]; pos < len && Alphabet::isliteral(c); identifier.push_back(c), c = input[++pos]);

    Token token;
    if (Dictionary::is_builtin_function(identifier)) {
        token = Token(Token::BUILTIN_FUNC, identifier);
    }
    else if (auto it = Dictionary::MetaSequenceAssossiations.find(identifier); it != Dictionary::MetaSequenceAssossiations.end()) {
        token = Token(it->second, identifier);
    }
    else {
        token = Token(Token::IDENTIFIER, identifier);
    }

    return token;
}

Token Lexer::extract_string(const std::string& input, std::size_t& pos) {
    std::string identifier;
    auto len = input.length();
    int flag = 0;

    if (input[pos] == '"') {
        flag = 1;
    }
    else if (input[pos] == '\'') {
        flag = 2;
    }
    pos++;

    for (auto c = input[pos]; pos < len; c = input[++pos]) {
        if ((c == '"' && flag == 1) || (c == '\'' && flag == 2)) {
            pos++;
            return Token(Token::STRING, identifier);
        }
        identifier.push_back(c);
    }
    throw std::runtime_error("Not enough quote!");
}

Token Lexer::extract_metachar(const std::string& input, std::size_t& pos) {
    std::string metasequence;
    auto len = input.length();
    for (auto c = input[pos]; pos < len && Alphabet::ismetachar(c); c = input[pos]) {
        metasequence.push_back(c);


        if (!Dictionary::is_valid_metasequence(metasequence)) {
            metasequence.pop_back();
            break;
        }

        pos++;
    }
    return Token(Dictionary::MetaSequenceAssossiations.at(metasequence), metasequence);
}

void Lexer::skip_line(const std::string& input, std::size_t& pos) {
    auto len = input.length();
    for (auto c = input[pos]; pos < len || Alphabet::isnewline(c); c = input[++pos]);
}

std::vector<Token> Lexer::parse(const std::string& input) {
    std::vector<Token> tokens;
    std::size_t pos = 0, len = input.length();
    while (pos < len) {
        auto c = input[pos];
        if (Alphabet::isspace(c)) {
            pos++;
        }
        else if (Alphabet::isdigit(c)) {
            tokens.push_back(extract_number(input, pos));
        }
        else if (Alphabet::isliteral(c)) {
            tokens.push_back(extract_identifier(input, pos));
        }
        else if (Alphabet::ismetachar(c)) {
            tokens.push_back(extract_metachar(input, pos));
        }
        else if (Alphabet::isquote(c)) {
            tokens.push_back(extract_string(input, pos));
        }
        else if (Alphabet::iscomment(c)) {
            skip_line(input, pos);
        }
        else
            throw std::runtime_error("Invalid character");
    }
    tokens.push_back({ Token::END, "" });
    return tokens;
}