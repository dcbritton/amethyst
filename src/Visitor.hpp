// Visitor.hpp

#ifndef VISITOR
#define VISITOR

#include "Node.hpp"

struct Node;
struct Statement;
struct Program;
struct CompStatement;
struct Expression;
struct EqualityExpr;
struct RelationExpr;
struct ShiftExpr;
struct AdditionExpr;
struct MultiplicationExpr;
struct Primary;
struct IntLiteral;
struct Variable;
struct VariableDefn;

struct Visitor : std::enable_shared_from_this<Visitor> {

    virtual void visit(std::shared_ptr<Program> n) {}

    virtual void visit(std::shared_ptr<Node> n) {}

    virtual void visit(std::shared_ptr<Statement> n) {}

    virtual void visit(std::shared_ptr<CompStatement> n) {}

    virtual void visit(std::shared_ptr<Expression> n) {}

    virtual void visit(std::shared_ptr<EqualityExpr> n) {}

    virtual void visit(std::shared_ptr<RelationExpr> n) {}

    virtual void visit(std::shared_ptr<ShiftExpr> n) {}

    virtual void visit(std::shared_ptr<AdditionExpr> n) {}

    virtual void visit(std::shared_ptr<MultiplicationExpr> n) {}

    virtual void visit(std::shared_ptr<Primary> n) {}

    virtual void visit(std::shared_ptr<IntLiteral> n) {}
    
    virtual void visit(std::shared_ptr<Variable> n) {}

    virtual void visit(std::shared_ptr<VariableDefn> n) {}
};

#endif