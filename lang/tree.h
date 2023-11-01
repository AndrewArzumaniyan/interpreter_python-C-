#pragma once
#include <vector>
#include <string>
#include <memory>

class MutableVisitor;
class ConstVisitor;

enum NodeType {
    STRING,
    INT,
    FLOAT,
    FUNC,
    BUILTIN_FUNC,
    VAR,
    BLOCK,
    CLASS,
    WHILE,
    FOR,
    BINARY,
    UNARY,
    CONDITIONAL,
    RETURN,
    CONTINUE,
    BREAK,
    FUNC_DECL,
    PASS
};

struct Node {
    virtual void accept(MutableVisitor&) = 0;
    virtual void accept(ConstVisitor&) const = 0;
    virtual ~Node() = default;
    NodeType type;
    Node(NodeType type) : type(type) {}
};

using Block = std::vector<std::shared_ptr<Node>>;

struct BlockContainer {
    Block block;
    std::size_t level;
    
    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;

    BlockContainer() : level(0) {}
    BlockContainer(Block block, std::size_t level) : block(block), level(level) {}
};

// Statement
struct Statement : Node {
    virtual ~Statement() = default;
    virtual void accept(MutableVisitor&) = 0;
    virtual void accept(ConstVisitor&) const = 0;
    Statement(NodeType type) : Node(type) {}
};

struct Pass : Statement {
    std::string value;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Pass(std::string value) : value(value), Statement(PASS) {}
};

//Expression
struct Expression : Statement {
    virtual void accept(MutableVisitor&) = 0;
    virtual void accept(ConstVisitor&) const = 0;
    virtual ~Expression() = default;
    Expression(NodeType type) : Statement(type) {}
};

template <typename T>
struct Const : Expression {
    T value;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Const(T value, NodeType type) : value(value), Expression(type) {}
};

struct Var : Expression {
    std::string value;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Var(std::string value) : value(value), Expression(VAR) {}
};

struct Binary : Expression {
    std::string value;
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Binary(std::string value, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
        : value(value), left(std::move(left)), right(std::move(right)), Expression(BINARY) {}
};

struct Unary : Expression {
    std::string value;
    std::shared_ptr<Expression> child;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Unary(std::string value, std::shared_ptr<Expression> child)
        : value(value), child(std::move(child)), Expression(UNARY) {}
};

struct Func : Expression {
    std::string value;
    std::vector<std::shared_ptr<Expression>> branches;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Func(std::string value, std::vector<std::shared_ptr<Expression>> branches, NodeType type = FUNC)
        : value(value), branches(branches), Expression(type) {}
};

struct Container : Expression {
    // hold
};

struct Conditional : Statement {
    std::vector<std::shared_ptr<Expression>> expr;
    std::vector<BlockContainer> ifblocks;
    BlockContainer elseblock;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Conditional(std::vector<std::shared_ptr<Expression>> expr, std::vector<BlockContainer> ifblocks, BlockContainer elseblock)
        : expr(expr), ifblocks(ifblocks), elseblock(elseblock), Statement(CONDITIONAL) {}
};

struct Loop : Statement {
    virtual void accept(MutableVisitor&) = 0;
    virtual void accept(ConstVisitor&) const = 0;
    virtual ~Loop() = default;
    Loop(NodeType type) : Statement(type) {}
};

struct While : Loop {
    std::shared_ptr<Expression> expr;
    BlockContainer block;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    While(std::shared_ptr<Expression> expr, BlockContainer block)
        : expr(std::move(expr)), block(block), Loop(WHILE) {}
};

struct For : Loop {
    std::string it;
    std::shared_ptr<Expression> expr;
    BlockContainer block;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    For(std::shared_ptr<Expression> container_expr, BlockContainer block)
        : expr(std::move(container_expr)), block(block), Loop(FOR) {}
};

struct Jump : Statement {
    virtual void accept(MutableVisitor&) = 0;
    virtual void accept(ConstVisitor&) const = 0;
    virtual ~Jump() = default;
    Jump(NodeType type) : Statement(type) {}
};

struct Return : Jump {
    std::string value;
    std::shared_ptr<Expression> expr;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Return() : Jump(RETURN) {}
    Return(std::shared_ptr<Expression> expr) : expr(std::move(expr)), Jump(RETURN), value("return") {}
};

struct Break : Jump {
    std::string value;
    std::shared_ptr<Expression> level;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Break(std::shared_ptr<Expression> level) : level(std::move(level)), Jump(BREAK), value("break") {}
};

struct Continue : Jump {
    std::string value;
    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Continue() : Jump(CONTINUE), value("continue") {}
};


// Declaration
struct Decl : Node {
    virtual void accept(MutableVisitor&) = 0;
    virtual void accept(ConstVisitor&) const = 0;
    virtual ~Decl() = default;
    Decl(NodeType type) : Node(type) {}
};

struct Func_decl : Decl {
    std::string value;
    std::vector<std::string> args;
    BlockContainer block;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;
    Func_decl(std::string value, std::vector<std::string> args, BlockContainer block)
        : value(value), args(args), block(block), Decl(FUNC_DECL) {}
};

using ClassBlock = std::vector<std::shared_ptr<Func_decl>>;

struct ClassBlockContainer {
    ClassBlock block;
    std::size_t level;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;

    ClassBlockContainer() : level(0) {}
    ClassBlockContainer(ClassBlock block, std::size_t level) : block(block), level(level) {}
};

struct Class_decl : Decl {
    std::string value;
    std::vector<std::string> args;
    ClassBlockContainer block;

    void accept(MutableVisitor&);
    void accept(ConstVisitor&) const;

    Class_decl(std::string value, std::vector<std::string> args, ClassBlockContainer block) : value(value), args(args), block(block), Decl(CLASS) {}
};