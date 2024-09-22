// Visitor.hpp

#ifndef VISITOR
#define VISITOR

#include "Node.hpp"

namespace Node {
    struct Node;
    struct Statement;
    struct Program;
    struct GlobalDefn;
    struct Global;
    struct FunctionBody;
    struct LogicalExpr;
    struct EqualityExpr;
    struct RelationExpr;
    struct ShiftExpr;
    struct AdditionExpr;
    struct MultiplicationExpr;
    struct Primary;
    struct NewExpr;
    struct IntLiteral;
    struct FloatLiteral;
    struct StringLiteral;
    struct BoolLiteral;
    struct Variable;
    struct VariableDefn;
    struct FunctionDefn;
    struct ParamList;
    struct Parameter;
    struct Assignment;
    struct Return;
    struct TypeDefn;
    struct ConstructorDefn;
    struct MemberDecl;
    struct MethodDefn;
    struct OperatorDefn;
    struct ConditionalBlock;
    struct ForLoop;
    struct WhileLoop;
    struct Break;
    struct Continue;
    struct AccessExpr;
    struct ExprList;
    struct Call;
    struct Array;
    struct Delete;
    struct Member;
    struct MethodCall;
}

struct Visitor : std::enable_shared_from_this<Visitor> {

    virtual void visit(std::shared_ptr<Node::Program> n) {}

    virtual void visit(std::shared_ptr<Node::GlobalDefn> n) {}

    virtual void visit(std::shared_ptr<Node::Global> n) {}

    virtual void visit(std::shared_ptr<Node::Node> n) {}

    virtual void visit(std::shared_ptr<Node::Statement> n) {}

    virtual void visit(std::shared_ptr<Node::FunctionBody> n) {}

    virtual void visit(std::shared_ptr<Node::LogicalExpr> n) {}

    virtual void visit(std::shared_ptr<Node::EqualityExpr> n) {}

    virtual void visit(std::shared_ptr<Node::RelationExpr> n) {}

    virtual void visit(std::shared_ptr<Node::ShiftExpr> n) {}

    virtual void visit(std::shared_ptr<Node::AdditionExpr> n) {}

    virtual void visit(std::shared_ptr<Node::MultiplicationExpr> n) {}

    virtual void visit(std::shared_ptr<Node::AccessExpr> n) {}

    virtual void visit(std::shared_ptr<Node::Primary> n) {}

    virtual void visit(std::shared_ptr<Node::Array> n) {}

    virtual void visit(std::shared_ptr<Node::NewExpr> n) {}

    virtual void visit(std::shared_ptr<Node::IntLiteral> n) {}

    virtual void visit(std::shared_ptr<Node::FloatLiteral> n) {}

    virtual void visit(std::shared_ptr<Node::StringLiteral> n) {}

    virtual void visit(std::shared_ptr<Node::BoolLiteral> n) {}
    
    virtual void visit(std::shared_ptr<Node::Variable> n) {}

    virtual void visit(std::shared_ptr<Node::ExprList> n) {}

    virtual void visit(std::shared_ptr<Node::Call> n) {}

    virtual void visit(std::shared_ptr<Node::VariableDefn> n) {}

    virtual void visit(std::shared_ptr<Node::FunctionDefn> n) {}

    virtual void visit(std::shared_ptr<Node::ParamList> n) {}

    virtual void visit(std::shared_ptr<Node::Parameter> n) {}

    virtual void visit(std::shared_ptr<Node::Assignment> n) {}

    virtual void visit(std::shared_ptr<Node::Return> n) {}

    virtual void visit(std::shared_ptr<Node::TypeDefn> n) {}

    virtual void visit(std::shared_ptr<Node::ConstructorDefn> n) {}

    virtual void visit(std::shared_ptr<Node::MemberDecl> n) {}

    virtual void visit(std::shared_ptr<Node::MethodDefn> n) {}

    virtual void visit(std::shared_ptr<Node::Member> n) {}

    virtual void visit(std::shared_ptr<Node::MethodCall> n) {}

    virtual void visit(std::shared_ptr<Node::OperatorDefn> n) {}

    virtual void visit(std::shared_ptr<Node::Delete> n) {}

    virtual void visit(std::shared_ptr<Node::ConditionalBlock> n) {}

    virtual void visit(std::shared_ptr<Node::WhileLoop> n) {}

    virtual void visit(std::shared_ptr<Node::Break> n) {}

    virtual void visit(std::shared_ptr<Node::Continue> n) {}
};

#endif