// Node.hpp

#ifndef NODE
#define NODE

#include <vector>
#include <memory>
#include <string>
#include "Visitor.hpp"

// node
struct Node : std::enable_shared_from_this<Node> {
    virtual void accept(std::shared_ptr<Visitor> v) {
        v->visit(shared_from_this());
    }

    ~Node() = default;
};

// program
struct Program : Node {
    std::shared_ptr<CompStatement> compStatement;

    // constructor
    Program(std::shared_ptr<CompStatement> compStatement) 
    : compStatement(compStatement) {}
};

// compound statement
struct CompStatement : Node {
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
    std::string type;
    std::string first;
    std::string second;

    Expression(std::string first, std::string second) 
        : first(first), second(second) {}
};

// variable
struct Variable : Node {
    std::string name;
    std::string type;

    // constructor
    Variable(std::string name, std::string type)
        : name(name), type(type) {}
};

// variable definiton
struct VariableDefn : Statement {
    std::string name;
    std::string type;
    std::shared_ptr<Expression> expression;

    // constructor
    VariableDefn(std::string name, std::string type, std::shared_ptr<Expression> expression)
        : name(name), type(type), expression(expression) {}
};

// parameter
struct Parameter : Node {
        std::string name;
        std::string type;

        // constructor
        Parameter(std::string name, std::string type)
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
    FunctionDefn(std::string name, std::string returnType, std::shared_ptr<ParamList> paramList, std::shared_ptr<CompStatement> functionBody) 
        : name(name), returnType(returnType), paramList(paramList), functionBody(functionBody) {}
};

struct TypeDefn : Statement {

};

#endif
