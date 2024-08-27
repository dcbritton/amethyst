// Node.hpp

#ifndef NODE
#define NODE

#include <vector>
#include <memory>
#include <string>
#include "Token.hpp"
#include "Visitor.hpp"

// node
struct Node {
    virtual void accept(std::shared_ptr<Visitor> v) {}

    ~Node() = default;
};

// program
struct Program : Node, std::enable_shared_from_this<Program> {
    std::shared_ptr<CompStatement> compStatement;

    // constructor
    Program(std::shared_ptr<CompStatement> compStatement) 
    : compStatement(compStatement) {}

    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

// compound statement
struct CompStatement : Node, std::enable_shared_from_this<CompStatement> {
    std::vector<std::shared_ptr<Statement>> statements;

    // constructor
    CompStatement(const std::vector<std::shared_ptr<Statement>>& statements) 
    : statements(statements) {}

    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

// statement
struct Statement : Node {

};

// expression
struct Expression : Statement, std::enable_shared_from_this<Expression> {
    std::shared_ptr<Node> expr;

    // constructor
    Expression(std::shared_ptr<Node> expr)
        : expr(expr) {}
    
    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

struct EqualityExpr : Node, std::enable_shared_from_this<EqualityExpr> {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    EqualityExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}

    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

struct RelationExpr : Node, std::enable_shared_from_this<RelationExpr> {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    RelationExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}

    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

struct ShiftExpr : Node, std::enable_shared_from_this<ShiftExpr> {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    ShiftExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}

    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

struct AdditionExpr : Node, std::enable_shared_from_this<AdditionExpr> {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    AdditionExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}

    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

struct MultiplicationExpr : Node, std::enable_shared_from_this<MultiplicationExpr> {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    MultiplicationExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}

    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

// primary
struct Primary : Node {

};

// int literal
struct IntLiteral : Primary, std::enable_shared_from_this<IntLiteral> {
    std::string value;

    // constructor
    IntLiteral(const std::string& value)
        : value(value) {}

    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

// variable
struct Variable : Primary, std::enable_shared_from_this<Variable> {
    std::string name;

    // constructor
    Variable(const std::string& name)
        : name(name) {}

    // accept
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

// variable definiton
struct VariableDefn : Statement, std::enable_shared_from_this<VariableDefn> {
    std::string name;
    std::string type;
    std::shared_ptr<Expression> expression;

    // constructor
    VariableDefn(const std::string& name, const std::string& type, std::shared_ptr<Expression> expression)
        : name(name), type(type), expression(expression) {}

    // accept visitor
    void accept(std::shared_ptr<Visitor> v) override {
        v->visit(shared_from_this());
    }
};

// parameter
struct Parameter : Node {
        std::string name;
        std::string type;

        // constructor
        Parameter(const std::string& name, const std::string& type)
            : name(name), type(type) {}
};

// parameter list
struct ParamList : Node {
    std::vector<std::shared_ptr<Parameter>> parameters;

    // constructor
    ParamList(const std::vector<std::shared_ptr<Parameter>>& parameters) 
        : parameters(parameters) {}
};

// function definition
struct FunctionDefn : Statement {
    std::string name;
    std::string returnType;
    std::shared_ptr<ParamList> paramList;
    std::shared_ptr<CompStatement> functionBody;

    // constuctor
    FunctionDefn(const std::string& name, const std::string& returnType, std::shared_ptr<ParamList> paramList, std::shared_ptr<CompStatement> functionBody) 
        : name(name), returnType(returnType), paramList(paramList), functionBody(functionBody) {}
};

struct TypeDefn : Statement {

};

#endif
