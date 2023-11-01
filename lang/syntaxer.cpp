#include <stdexcept>
#include "syntaxer.h"

std::vector<std::shared_ptr<Node>> Syntaxer::parse(const std::vector<Token>& tokens) {
    std::vector<std::shared_ptr<Node>> nodes;
    std::size_t pos = 0;
    std::size_t level = 0;

    while (tokens[pos].type != Token::END) {
        if (tokens[pos].type == Token::DEF) {
            level += 1;
            nodes.push_back(parse_func_decl(tokens, pos, level));
        }
        else if (tokens[pos].type == Token::CLASS) {
            level += 1;
            nodes.push_back(parse_class_decl(tokens, pos, level));
        }
        else {
            if (tokens[pos].type == Token::NEWLINE) {
                pos++;
                if (tokens[pos].type == Token::END)
                    break;
            }
            nodes.push_back(parse_statement(tokens, pos, level));
            if (tokens[pos].type == Token::NEWLINE) {
                pos++;
                if (tokens[pos].type == Token::END)
                    break;
            }
        }
    }

    return nodes;
}

std::shared_ptr<Statement> Syntaxer::parse_statement(const std::vector<Token>& tokens, std::size_t& pos, const std::size_t& level) {
    if (tokens[pos].type == Token::IF) {
        //level += 1;
        return parse_conditional(tokens, pos, level+1);
    }
    else if (tokens[pos].type == Token::FOR || tokens[pos].type == Token::WHILE) {
        //level += 1;
        return parse_loop(tokens, pos, level+1);
    }
    else if (tokens[pos].type == Token::JUMP) {
        return parse_jump(tokens, pos);
    }
    return parse_expression(tokens, pos);
}

std::shared_ptr<Conditional> Syntaxer::parse_conditional(const std::vector<Token>& tokens, std::size_t& pos, const std::size_t& level) {
    std::vector<std::shared_ptr<Expression>> expr;
    std::vector<BlockContainer> ifblocks;
    BlockContainer elseblock;

    if (tokens[pos].type == Token::IF) {
        pos++;
        expr.push_back(parse_expression(tokens, pos));

        if (tokens[pos].type == Token::COLON && tokens[pos + 1].type == Token::NEWLINE) {
            pos += 2;
            ifblocks.push_back(parse_block  (tokens, pos, level));
        }
        else {
            throw std::runtime_error("74 Syntaxer::parse_conditional(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
        }
    }

    if (tokens[pos].type == Token::END) {
        return std::make_shared<Conditional>(expr, ifblocks, elseblock);
    }

    pos++;  

    int elifflag = 0;
    int elseflag = 0;

    while (tokens[pos].type == Token::ELIF) {
        elifflag = 1;
        pos++;
        expr.push_back(parse_expression(tokens, pos));

        if (tokens[pos].type == Token::COLON && tokens[pos + 1].type == Token::NEWLINE) {
            pos += 2;
            ifblocks.push_back(parse_block(tokens, pos, level));
        }
        else {
            throw std::runtime_error("86 Syntaxer::parse_conditional(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
        }
    }


    if (elifflag) {
        if (tokens[pos].type == Token::END) {
            return std::make_shared<Conditional>(expr, ifblocks, elseblock);
        }
        pos++;
    }

    if (tokens[pos].type == Token::ELSE) {
        elseflag = 1;
        pos++;
        if (tokens[pos].type == Token::COLON && tokens[pos + 1].type == Token::NEWLINE) {
            pos += 2;
            elseblock = parse_block(tokens, pos, level);
        }
        else {
            throw std::runtime_error("96 Syntaxer::parse_conditional(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
        }
    }
        
    if (!(elifflag || elseflag))
        pos -= 2;

    //level -= 1;
    return std::make_shared<Conditional>(expr, ifblocks, elseblock);
}


std::shared_ptr<Expression> Syntaxer::parse_expression(const std::vector<Token>& tokens, std::size_t& pos) {
    auto token_type = tokens[pos].type;
    auto node = parse_binary_expression(tokens, pos);


    if (tokens[pos].type != Token::COLON && tokens[pos].type != Token::NEWLINE && tokens[pos].type != Token::TAB && tokens[pos].type != Token::END) {
        throw std::runtime_error("Expected : or newline. Token got: " + tokens[pos].value + " . pos = " + std::to_string(pos));
    }

    return node;
}


std::shared_ptr<Loop> Syntaxer::parse_loop(const std::vector<Token>& tokens, std::size_t& pos, const std::size_t& level) {
    if (tokens[pos].type == Token::WHILE) {
        pos++;
        auto expr = parse_expression(tokens, pos);
        BlockContainer block;

        if (tokens[pos].type == Token::COLON && tokens[pos + 1].type == Token::NEWLINE) {
            pos += 2;
            block = parse_block(tokens, pos, level);
        }
        else {
            throw std::runtime_error("124 Syntaxer::parse_loop(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
        }

        return std::make_shared<While>(expr, block);
    }

    pos++;
    auto expr = parse_expression(tokens, pos);
    BlockContainer block;

    if (tokens[pos].type == Token::COLON && tokens[pos + 1].type == Token::NEWLINE) {
        pos += 2;
        block = parse_block(tokens, pos, level);
    }
    else {
        throw std::runtime_error("138 Syntaxer::parse_loop(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
    }

    //level -= 1;
    return std::make_shared<For>(expr, block);
}


BlockContainer Syntaxer::parse_block(const std::vector<Token>& tokens, std::size_t& pos, const std::size_t& level) {
    Block block;
    std::size_t block_level = level;

    if (tokens[pos].type == Token::TAB) {
        while (1) {
            for (int i = 1; i < level; i++, pos++);
            if (tokens[pos].type != Token::TAB && tokens[pos].type != Token::END) {
                break;
            }

            pos++;
            if (tokens[pos].type == Token::DEF) {
                //level += 1;
                block_level = level;
                block.push_back(parse_func_decl(tokens, pos, level));
                //level -= 1;
                pos--;
            }
            else if (tokens[pos].type == Token::CLASS) {
                //level += 1;
                block_level = level;
                block.push_back(parse_class_decl(tokens, pos, level));
                //level -= 1;
                pos--;
            }
            else {
                //block_level = level;
                //block.push_back(parse_statement(tokens, pos, block_level));
                block.push_back(parse_statement(tokens, pos, level));
                if (tokens[pos].type == Token::COLON) {
                    pos++;
                }
            }
            if (tokens[pos].type == Token::END) {
                return BlockContainer(block, block_level);
            }
            if (tokens[pos].type != Token::NEWLINE) {
                throw std::runtime_error("Syntaxer::parse_block(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
            }

            pos++;
        }
    }
    else {
        throw std::runtime_error("Syntaxer::parse_block(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
    }

    pos--;
    //level--;
    return BlockContainer(block, block_level);
}

ClassBlockContainer Syntaxer::parse_class_block(const std::vector<Token>& tokens, std::size_t& pos, const std::size_t& level) {
    ClassBlock block;
    std::size_t block_level;

    if (tokens[pos].type == Token::TAB) {
        while (tokens[pos].type == Token::TAB && tokens[pos].type != Token::END) {
            for (int i = 1; i < level; i++, pos++);
            pos++;
            if (tokens[pos].type == Token::DEF) {
                //level += 1;
                block_level = level;
                block.push_back(parse_func_decl(tokens, pos, level));
                pos--;
            }
            else {
                throw std::runtime_error("Syntaxer::parse_class_block(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
            }
            if (tokens[pos].type == Token::END) {
                return ClassBlockContainer(block, block_level);
            }
            if (tokens[pos].type != Token::NEWLINE) {
                throw std::runtime_error("Syntaxer::parse_block(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
            }
            pos++;
        }
    }
    else {
        throw std::runtime_error("Syntaxer::parse_block(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
    }

    pos--;
    return ClassBlockContainer(block, block_level);
}

std::shared_ptr<Jump> Syntaxer::parse_jump(const std::vector<Token>& tokens, std::size_t& pos) {
    if (tokens[pos].value == "break") {
        pos++;
        return std::make_shared<Break>(parse_expression(tokens, pos));
    }
    else if (tokens[pos].value == "continue") {
        pos++;
        return std::make_shared<Continue>();
    }

    pos++;
    if (tokens[pos].type == Token::NEWLINE || tokens[pos].type == Token::END) {
        return std::make_shared<Return>();
    }

    //level -= 1;
    return std::make_shared<Return>(parse_expression(tokens, pos));
}

std::shared_ptr<Func_decl> Syntaxer::parse_func_decl(const std::vector<Token>& tokens, std::size_t& pos, const std::size_t& level) {
    std::string value;
    std::vector<std::string> args;
    BlockContainer block;

    if (tokens[pos].type == Token::DEF) {
        pos++;

        if (tokens[pos].type == Token::IDENTIFIER) {
            value = tokens[pos].value;
            pos++;

            if (tokens[pos].type == Token::LPAREN) {
                pos ++;

                while (tokens[pos].type == Token::IDENTIFIER) {
                    args.push_back(tokens[pos++].value);
                    if (tokens[pos].type == Token::RPAREN) {
                        break;
                    }
                    pos++;
                }

                if (tokens[pos].type != Token::RPAREN) {
                    throw std::runtime_error("Syntaxer::parse_class_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
                }

                if (tokens[pos].type != Token::COLON)
                    pos++;
            }
            else {
                throw std::runtime_error("195 Syntaxer::func_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
            }
            if (tokens[pos].type != Token::COLON) 
                pos++;
            if (tokens[pos].type == Token::COLON && tokens[pos + 1].type == Token::NEWLINE) {
                pos += 2;

                block = parse_block(tokens, pos, level);
            }
            else {
                throw std::runtime_error("203 Syntaxer::func_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
            }
        }
        else {
            throw std::runtime_error("206 Syntaxer::func_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
        }
    }
    else {
        throw std::runtime_error("209 Syntaxer::func_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
    }

    if (tokens[pos].type != Token::END)
        pos++;
    return std::make_shared<Func_decl>(value, args, block);
}

std::shared_ptr<Class_decl> Syntaxer::parse_class_decl(const std::vector<Token>& tokens, std::size_t& pos, const std::size_t& level) {
    std::string value;
    std::vector<std::string> args;
    ClassBlockContainer block;

    if (tokens[pos].type == Token::CLASS) {
        pos++;
        if (tokens[pos].type == Token::IDENTIFIER) {
            value = tokens[pos].value;
            pos++;

            if (tokens[pos].type == Token::LPAREN) {
                pos++;
                
                while (tokens[pos].type == Token::IDENTIFIER) {
                    args.push_back(tokens[pos++].value);
                    if (tokens[pos].type == Token::RPAREN) {
                        break;
                    }
                    pos++;
                }

                if (tokens[pos].type != Token::RPAREN) {
                    throw std::runtime_error("Syntaxer::parse_class_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
                }

                if (tokens[pos].type != Token::COLON) 
                    pos++;

                if (tokens[pos].type == Token::COLON && tokens[pos + 1].type == Token::NEWLINE) {
                    pos += 2;
                    
                    block = parse_class_block(tokens, pos, level);
                }
                else {
                    throw std::runtime_error("Syntaxer::parse_class_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
                }
            }
            else {
                throw std::runtime_error("Syntaxer::parse_class_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
            }
        }
        else {
            throw std::runtime_error("Syntaxer::parse_class_decl(): Invalid token: " + tokens[pos].value + " (pos: " + std::to_string(pos) + ")");
        }
    }

    if (tokens[pos].type != Token::END)
        pos++;
    return std::make_shared<Class_decl>(value, args, block);
}


const std::unordered_map<std::string, int> Syntaxer::precedences = {
    {"=", 1},
    {"+=", 2}, {"-=", 2},
    {"*=", 3}, {"/=",3 }, {"//=", 3}, {"**=", 3}, {"%=", 3},
    {"or", 4},
    {"and", 5},
    {"==", 6}, {"!=", 6},
    {"<", 7}, {"<=", 7}, {">", 7}, {">=", 7},
    {"+", 8}, {"-", 8},
    {"*", 9}, {"/", 9}, {"%", 9}, {"//", 9},
    {"**", 10},
};


std::vector<std::shared_ptr<Expression>> Syntaxer::parse_interior(const std::vector<Token>& tokens, size_t& pos) {
    std::vector<std::shared_ptr<Expression>> args;
    expect_token(Token::LPAREN, tokens, pos);
    pos--;
    while (1) {
        pos++;
        if (tokens[pos].type == Token::RPAREN)
            break;
        args.push_back(parse_binary_expression(tokens, pos));
        if (tokens[pos].type == Token::COMMA)
            continue;
        expect_token(Token::RPAREN, tokens, pos);
        break;
    }
    return args;
}

void Syntaxer::expect_token(const Token::TokenType& expected_type, const std::vector<Token>& tokens, size_t& pos) {
    auto token = tokens[pos];
    if (token.type == expected_type)
        pos++;
    else
        throw std::runtime_error("Syntaxer::expect_token(): Unexpected token: " + tokens[pos].value);
}


std::shared_ptr<Expression> Syntaxer::parse_binary_expression(const std::vector<Token>& tokens, size_t& pos, const int& min_precedence) {
    std::shared_ptr<Expression> left;

    if (tokens[pos].type == Token::PLUS || tokens[pos].type == Token::MINUS) {
        left = std::make_shared<Unary>(tokens[pos++].value, parse_binary_expression(tokens, pos));
    }
    else {
        left = parse_simple_expression(tokens, pos);

    }

    for (auto token = tokens[pos]; ; token = tokens[pos]) {
        auto it = precedences.find(token.value);
        if (it == precedences.end() || it->second < min_precedence) {
            break;
        }
        auto right = parse_binary_expression(tokens, ++pos, it->second);
        left = std::make_shared<Binary>(token.value, left, right);
    }
    return left;
}

std::shared_ptr<Expression> Syntaxer::parse_simple_expression(const std::vector<Token>& tokens, size_t& pos) {
    auto token = tokens[pos++];

    if (token.type == Token::INT) {
        return std::make_shared<Const<int>>(std::stoi(token.value), INT);
    }
    else if (token.type == Token::FLOAT) {
        return std::make_shared<Const<float>>(std::stof(token.value), FLOAT);
    }
    else if (token.type == Token::BOOL) {
        return std::make_shared<Const<bool>>(token.value == "True", FLOAT);
    }
    else if (token.type == Token::STRING) {
        return std::make_shared<Const<std::string>>(token.value, STRING);
    }
    else if (token.type == Token::IDENTIFIER) {
        if (tokens[pos].type == Token::LPAREN) {
            std::vector<std::shared_ptr<Expression>> args = parse_interior(tokens, pos);
            return std::make_shared<Func>(token.value, std::move(args));
        }
        else
            return std::make_shared<Var>(token.value);
    }
    else if (token.type == Token::BUILTIN_FUNC) {
        return std::make_shared<Func>(token.value, parse_interior(tokens, pos), BUILTIN_FUNC);
    }
    else if (token.type == Token::PLUS || token.type == Token::MINUS) {
        auto expr = parse_simple_expression(tokens, pos);
        return std::make_shared<Unary>(token.value, std::move(expr));
    }
    else if (token.type == Token::LPAREN) {
        auto node = parse_binary_expression(tokens, pos);
        expect_token(Token::RPAREN, tokens, pos);
        return node;
    }
    else
        throw std::runtime_error("Syntaxer::parse_simple_expression(): Invalid token: " + token.value + " (pos: " + std::to_string(pos) + ")");
}