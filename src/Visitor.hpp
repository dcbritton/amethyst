// Visitor.hpp

#ifndef VISITOR
#define VISITOR

#include "Node.hpp"

namespace Node {
    struct Node;
    struct Statement;
    struct Program;
    struct CompStatement;
    struct EqualityExpr;
    struct RelationExpr;
    struct ShiftExpr;
    struct AdditionExpr;
    struct MultiplicationExpr;
    struct Primary;
    struct IntLiteral;
    struct Variable;
    struct VariableDefn;
    struct FunctionDefn;
    struct ParamList;
    struct Parameter;
    struct Assignment;
    struct Return;
    struct TypeDefn;
    struct MemberDefn;
    struct ConditionalBlock;
    struct ForLoop;
    struct WhileLoop;
    struct DotExpr;
    struct CallArgs;
    struct Call;
}

struct Visitor : std::enable_shared_from_this<Visitor> {

    virtual void visit(std::shared_ptr<Node::Program> n) {}

    virtual void visit(std::shared_ptr<Node::Node> n) {}

    virtual void visit(std::shared_ptr<Node::Statement> n) {}

    virtual void visit(std::shared_ptr<Node::CompStatement> n) {}

    virtual void visit(std::shared_ptr<Node::EqualityExpr> n) {}

    virtual void visit(std::shared_ptr<Node::RelationExpr> n) {}

    virtual void visit(std::shared_ptr<Node::ShiftExpr> n) {}

    virtual void visit(std::shared_ptr<Node::AdditionExpr> n) {}

    virtual void visit(std::shared_ptr<Node::MultiplicationExpr> n) {}

    virtual void visit(std::shared_ptr<Node::DotExpr> n) {}

    virtual void visit(std::shared_ptr<Node::Primary> n) {}

    virtual void visit(std::shared_ptr<Node::IntLiteral> n) {}
    
    virtual void visit(std::shared_ptr<Node::Variable> n) {}

    virtual void visit(std::shared_ptr<Node::CallArgs> n) {}

    virtual void visit(std::shared_ptr<Node::Call> n) {}

    virtual void visit(std::shared_ptr<Node::VariableDefn> n) {}

    virtual void visit(std::shared_ptr<Node::FunctionDefn> n) {}

    virtual void visit(std::shared_ptr<Node::ParamList> n) {}

    virtual void visit(std::shared_ptr<Node::Parameter> n) {}

    virtual void visit(std::shared_ptr<Node::Assignment> n) {}

    virtual void visit(std::shared_ptr<Node::Return> n) {}

    virtual void visit(std::shared_ptr<Node::TypeDefn> n) {}

    virtual void visit(std::shared_ptr<Node::MemberDefn> n) {}

    virtual void visit(std::shared_ptr<Node::ConditionalBlock> n) {}

    virtual void visit(std::shared_ptr<Node::WhileLoop> n) {}
};

#endif