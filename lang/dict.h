#pragma once
#include <vector>
#include <unordered_map>
#include "token.h"

class Dictionary {
public:
    static bool is_valid_metasequence(const std::string&);
    static bool is_builtin_function(const std::string&);
    // static bool is_keyword(const std::string&);
    static const std::unordered_map<std::string, Token::TokenType> MetaSequenceAssossiations;
    // static const std::vector<std::string> keywords;
    static const std::vector<std::string> functions;
};