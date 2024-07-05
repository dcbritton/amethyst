// Visitor.hpp

#ifndef VISITOR
#define VISITOR

#include "Node.hpp"

struct Node;
struct Statement;
struct CompStatement;
struct Expression;
struct Variable;
struct VariableDefn;

struct Visitor : std::enable_shared_from_this<Visitor> {

    virtual void visit(std::shared_ptr<Node> n) {}

    virtual void visit(std::shared_ptr<Statement> n) {}

    virtual void visit(std::shared_ptr<CompStatement> n) {}

    virtual void visit(std::shared_ptr<Expression> n) {}

    virtual void visit(std::shared_ptr<Variable> n) {}

    virtual void visit(std::shared_ptr<VariableDefn> n) {}
};

#endif