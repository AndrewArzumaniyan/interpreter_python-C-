#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "Token.h"
#include "tree.h"

class Syntaxer {
public:
    static std::vector<std::shared_ptr<Node>> parse(const std::vector<Token>&);
private:
    static std::shared_ptr<Func_decl> parse_func_decl(const std::vector<Token>&, std::size_t&, const std::size_t&);
    static ClassBlockContainer parse_class_block(const std::vector<Token>&, std::size_t&, const std::size_t&);
    static std::shared_ptr<Class_decl> parse_class_decl(const std::vector<Token>&, std::size_t&, const std::size_t&);
private:
    static std::shared_ptr<Expression> parse_binary_expression(const std::vector<Token>&, std::size_t&, const int& = 0);
    static std::shared_ptr<Expression> parse_simple_expression(const std::vector<Token>&, std::size_t&);
    static void expect_token(const Token::TokenType&, const std::vector<Token>&, std::size_t&);
    static std::shared_ptr<Expression> parse_expression(const std::vector<Token>&, std::size_t&);
    static std::vector<std::shared_ptr<Expression>> parse_interior(const std::vector<Token>&, std::size_t&);

    static const std::unordered_map<std::string, int> precedences;
private:
    static BlockContainer parse_block(const std::vector<Token>&, std::size_t&, const std::size_t&);
    static std::shared_ptr<Jump> parse_jump(const std::vector<Token>&, std::size_t&);
    static std::shared_ptr<Conditional> parse_conditional(const std::vector<Token>&, std::size_t&, const std::size_t&);
    static std::shared_ptr<Loop> parse_loop(const std::vector<Token>&, std::size_t&, const std::size_t&);
    static std::shared_ptr<Statement> parse_statement(const std::vector<Token>&, std::size_t&, const std::size_t&);
};