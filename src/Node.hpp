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

    Variable(std::string name, std::string type)
        : name(name), type(type) {}
};

// variable definiton
struct VariableDefn : Statement {
    std::string name;
    std::string type;
    std::shared_ptr<Expression> expression;

    VariableDefn(std::string name, std::string type, std::shared_ptr<Expression> expression)
        : name(name), type(type), expression(expression) {}
};

// function definition
struct FuncDefn : Statement {

};

struct TypeDefn : Statement {

};







#endif
