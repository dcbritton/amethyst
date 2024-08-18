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
    CompStatement(std::vector<std::shared_ptr<Statement>> statements) 
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
struct Expression : Statement {
    std::shared_ptr<Node> expr;

    // constructor
    Expression(std::shared_ptr<Node> expr)
        : expr(expr) {}
};

struct EqualityExpr : Node {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    EqualityExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}
};

struct RelationExpr : Node {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    RelationExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}
};

struct ShiftExpr : Node {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    ShiftExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}
};

struct AdditionExpr : Node {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    AdditionExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}
};

struct MultiplicationExpr : Node {
    std::shared_ptr<Node> LHS;
    std::string op;
    std::shared_ptr<Node> RHS;

    // constructor
    MultiplicationExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
        : LHS(LHS), op(op), RHS(RHS) {}
};

// primary
struct Primary : Node {

};

// int literal
struct IntLiteral : Primary {
    std::string value;

    // constructor
    IntLiteral(const std::string& value)
        : value(value) {}
};

// variable
struct Variable : Primary {
    std::string name;
    std::string type;

    // constructor
    Variable(const std::string& name, const std::string& type)
        : name(name), type(type) {}
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
    ParamList(std::vector<std::shared_ptr<Parameter>> parameters) 
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
