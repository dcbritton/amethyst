// DotVisitor.hpp

#ifndef DOTVISITOR
#define DOTVISITOR

#include <fstream>
#include "Visitor.hpp"
#include "Node.hpp"

struct DotVisitor : Visitor {

    // visit compound statement
    void visit(std::shared_ptr<CompStatement> n) override {
        for (auto statement : n->statements) {
            statement->accept(shared_from_this());
        }
    }

    // visit variable definition
    void visit(std::shared_ptr<VariableDefn> n) override {

    }

    // visit variable
    void visit(std::shared_ptr<Variable> n) override {

    }

    // visit expression
    void visit(std::shared_ptr<Expression> n) override {

    }
};

#endif