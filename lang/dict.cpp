#include "dict.h"


bool Dictionary::is_valid_metasequence(const std::string& token) {
    return MetaSequenceAssossiations.find(token) != MetaSequenceAssossiations.end();
}

bool Dictionary::is_builtin_function(const std::string& token) {
    return std::find(functions.begin(), functions.end(), token) != functions.end();
}

// bool Lexer::Dictionary::is_keyword(const std::string& token) {
//     return std::find(keywords.begin(), keywords.end(), token) != keywords.end();
// }

const std::unordered_map<std::string, Token::TokenType> Dictionary::MetaSequenceAssossiations = {
    {"+", Token::PLUS},
    {"-", Token::MINUS},
    {"*", Token::MULTIPLY},
    {"/", Token::DIVIDE},
    {"//", Token::FLOORDIV},
    {"%", Token::MODULO},
    {"**", Token::POWER},
    {",", Token::COMMA},
    {"(", Token::LPAREN},
    {")", Token::RPAREN},
    {"[", Token::LSQUARE},
    {"]", Token::RSQUARE},
    {"{", Token::LCURLY},
    {"}", Token::RCURLY},
    {":", Token::COLON},
    {"=", Token::EQUALS},
    {"+=", Token::DOUBLEQUALS},
    {"-=", Token::DOUBLEQUALS},
    {"*=", Token::DOUBLEQUALS},
    {"/=", Token::DOUBLEQUALS},
    {"//=", Token::DOUBLEQUALS},
    {"%=", Token::DOUBLEQUALS},
    {"**=", Token::DOUBLEQUALS},
    {"==", Token::COMPARISON},
    {"!=", Token::COMPARISON},
    {"<", Token::COMPARISON},
    {">", Token::COMPARISON},
    {"<=", Token::COMPARISON},
    {">=", Token::COMPARISON},
    {"\n", Token::NEWLINE},
    {"\t", Token::TAB},
    {"if", Token::IF},
    {"elif", Token::ELIF},
    {"else", Token::ELSE},
    {"and", Token::AND},
    {"or", Token::OR},
    {"not", Token::NOT},
    {"class", Token::CLASS},
    {"def", Token::DEF},
    {"in", Token::IN},
    {"raise", Token::RAISE},
    {"while", Token::WHILE},
    {"for", Token::FOR},
    {"return", Token::JUMP},
    {"continue", Token::JUMP},
    {"break", Token::JUMP},
    {"True", Token::BOOL},
    {"False", Token::BOOL},
    {":", Token::COLON},
};

const std::vector<std::string> Dictionary::functions = {
    "pow",
    "exp",
    "log",
    "sin",
    "cos",
    "print",
    "input",
    "int",
    "float",
    "str",
    "len",
    "range",
    "fact",
};


// const std::vector<std::string> Dictionary::keywords = {
//     "False",
//     "None",
//     "True",
//     "and",
//     "or",
//     "in",
//     "def",
//     "del",
//     "if",
//     "elif",
//     "else",
//     "pass",
//     "not",
//     "for",
//     "while",
//     "break",
//     "continue",
// };