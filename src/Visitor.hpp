// Visitor.hpp

#ifndef VISITOR
#define VISITOR

#include "Node.hpp"

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

struct Visitor : std::enable_shared_from_this<Visitor> {

    virtual void visit(std::shared_ptr<Program> n) {}

    virtual void visit(std::shared_ptr<Node> n) {}

    virtual void visit(std::shared_ptr<Statement> n) {}

    virtual void visit(std::shared_ptr<CompStatement> n) {}

    virtual void visit(std::shared_ptr<EqualityExpr> n) {}

    virtual void visit(std::shared_ptr<RelationExpr> n) {}

    virtual void visit(std::shared_ptr<ShiftExpr> n) {}

    virtual void visit(std::shared_ptr<AdditionExpr> n) {}

    virtual void visit(std::shared_ptr<MultiplicationExpr> n) {}

    virtual void visit(std::shared_ptr<DotExpr> n) {}

    virtual void visit(std::shared_ptr<Primary> n) {}

    virtual void visit(std::shared_ptr<IntLiteral> n) {}
    
    virtual void visit(std::shared_ptr<Variable> n) {}

    virtual void visit(std::shared_ptr<CallArgs> n) {}

    virtual void visit(std::shared_ptr<Call> n) {}

    virtual void visit(std::shared_ptr<VariableDefn> n) {}

    virtual void visit(std::shared_ptr<FunctionDefn> n) {}

    virtual void visit(std::shared_ptr<ParamList> n) {}

    virtual void visit(std::shared_ptr<Parameter> n) {}

    virtual void visit(std::shared_ptr<Assignment> n) {}

    virtual void visit(std::shared_ptr<Return> n) {}

    virtual void visit(std::shared_ptr<TypeDefn> n) {}

    virtual void visit(std::shared_ptr<MemberDefn> n) {}

    virtual void visit(std::shared_ptr<ConditionalBlock> n) {}

    virtual void visit(std::shared_ptr<ForLoop> n) {}

    virtual void visit(std::shared_ptr<WhileLoop> n) {}
};

#endif