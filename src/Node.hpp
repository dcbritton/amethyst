// Node.hpp

#ifndef NODE
#define NODE

#include <vector>
#include <memory>
#include <string>
#include "Token.hpp"
#include "Visitor.hpp"

namespace Node {

    // node
    struct Node {
        virtual void accept(std::shared_ptr<Visitor> v) {}

        ~Node() = default;
    };

    // program
    struct Program : Node, std::enable_shared_from_this<Program> {
        std::vector<std::shared_ptr<Node>> definitions;

        // constructor
        Program(const std::vector<std::shared_ptr<Node>>& definitions) 
        : definitions(definitions) {}

        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // global definiton
    struct GlobalDefn : Node, std::enable_shared_from_this<GlobalDefn> {
        std::string name;
        std::string type;
        std::shared_ptr<Node> expression;

        // constructor
        GlobalDefn(const std::string& name, const std::string& type, std::shared_ptr<Node> expression)
            : name(name), type(type), expression(expression) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // compound statement
    struct FunctionBody : Node, std::enable_shared_from_this<FunctionBody> {
        std::vector<std::shared_ptr<Statement>> statements;

        // constructor
        FunctionBody(const std::vector<std::shared_ptr<Statement>>& statements) 
        : statements(statements) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // statement
    struct Statement : Node {

    };

    struct Expr : Node {
        std::shared_ptr<Node> LHS;
        std::string op;
        std::shared_ptr<Node> RHS;

        // constructor
        Expr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : LHS(LHS), op(op), RHS(RHS) {}
    };

    struct LogicalExpr : Expr, std::enable_shared_from_this<LogicalExpr> {

        // constructor
        LogicalExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    struct EqualityExpr : Expr, std::enable_shared_from_this<EqualityExpr> {

        // constructor
        EqualityExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    struct RelationExpr : Expr, std::enable_shared_from_this<RelationExpr> {

        // constructor
        RelationExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    struct ShiftExpr : Expr, std::enable_shared_from_this<ShiftExpr> {

        // constructor
        ShiftExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    struct AdditionExpr : Expr, std::enable_shared_from_this<AdditionExpr> {

        // constructor
        AdditionExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    struct MultiplicationExpr : Expr, std::enable_shared_from_this<MultiplicationExpr> {

        // constructor
        MultiplicationExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // . []
    struct AccessExpr : Expr, std::enable_shared_from_this<AccessExpr> {

        // constructor
        AccessExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // primary
    struct Primary : Node {

    };

    // global
    struct Global : Primary, std::enable_shared_from_this<Global> {
        std::string name;

        // constructor
        Global(const std::string& name)
            : name(name) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // array
    struct Array : Primary, std::enable_shared_from_this<Array> {
        std::shared_ptr<ExprList> exprs;

        Array(std::shared_ptr<ExprList> exprs)
            : exprs(exprs) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // expr list
    struct ExprList : Node, std::enable_shared_from_this<ExprList> {
        std::vector<std::shared_ptr<Node>> exprs;

        // constructor
        ExprList(const std::vector<std::shared_ptr<Node>>& exprs)
            : exprs(exprs) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // call
    struct Call : Primary, std::enable_shared_from_this<Call> {
        std::string name;
        std::shared_ptr<ExprList> args;

        // constructor
        Call(const std::string& name, std::shared_ptr<ExprList> args)
            : name(name), args(args) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
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
    
    // float literal
    struct FloatLiteral : Primary, std::enable_shared_from_this<FloatLiteral> {
        std::string value;

        // constructor
        FloatLiteral(const std::string& value)
            : value(value) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // string literal
    struct StringLiteral : Primary, std::enable_shared_from_this<StringLiteral> {
        std::string value;

        // constructor
        StringLiteral(const std::string& value)
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
        std::shared_ptr<Node> expression;

        // constructor
        VariableDefn(const std::string& name, const std::string& type, std::shared_ptr<Node> expression)
            : name(name), type(type), expression(expression) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // parameter
    struct Parameter : Node, std::enable_shared_from_this<Parameter> {
        std::string name;
        std::string type;

        // constructor
        Parameter(const std::string& name, const std::string& type)
            : name(name), type(type) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // parameter list
    struct ParamList : Node, std::enable_shared_from_this<ParamList> {
        std::vector<std::shared_ptr<Parameter>> parameters;

        // constructor
        ParamList(const std::vector<std::shared_ptr<Parameter>>& parameters) 
            : parameters(parameters) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // function definition
    struct FunctionDefn : Node, std::enable_shared_from_this<FunctionDefn> {
        std::string name;
        std::string returnType;
        std::shared_ptr<ParamList> paramList;
        std::shared_ptr<FunctionBody> functionBody;

        // constuctor
        FunctionDefn(const std::string& name, const std::string& returnType, std::shared_ptr<ParamList> paramList, std::shared_ptr<FunctionBody> functionBody) 
            : name(name), returnType(returnType), paramList(paramList), functionBody(functionBody) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // type definition
    struct TypeDefn : Node, std::enable_shared_from_this<TypeDefn> {
        std::string name;
        std::vector<std::shared_ptr<Node>> definitions;

        // constuctor
        TypeDefn(const std::string& name, const std::vector<std::shared_ptr<Node>>& definitions) 
            : name(name), definitions(definitions) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // operator overload
    struct OperatorOverload : Node, std::enable_shared_from_this<OperatorOverload> {
        std::string op;
        std::shared_ptr<Parameter> parameter;
        std::string returnType;
        std::shared_ptr<FunctionBody> stmts;

        OperatorOverload(const std::string& op, std::shared_ptr<Parameter> parameter, const std::string& returnType, std::shared_ptr<FunctionBody> stmts)
            : op(op), parameter(parameter), returnType(returnType), stmts(stmts) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // member definition
    struct MemberDefn : Node, std::enable_shared_from_this<MemberDefn> {
        std::string name;
        std::string type;
        std::shared_ptr<Node> expression;

        // constructor
        MemberDefn(const std::string& name, const std::string& type, std::shared_ptr<Node> expression)
            : name(name), type(type), expression(expression) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // method defintion
    struct MethodDefn : Node, std::enable_shared_from_this<MethodDefn> {
        std::string name;
        std::string returnType;
        std::shared_ptr<ParamList> paramList;
        std::shared_ptr<FunctionBody> functionBody;

        // constuctor
        MethodDefn(const std::string& name, const std::string& returnType, std::shared_ptr<ParamList> paramList, std::shared_ptr<FunctionBody> functionBody) 
            : name(name), returnType(returnType), paramList(paramList), functionBody(functionBody) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // member
    struct Member : Primary, std::enable_shared_from_this<Member> {
        std::string name;

        // constructor
        Member(const std::string& name)
            : name(name) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // method call
    struct MethodCall : Primary, std::enable_shared_from_this<MethodCall> {
        std::string name;
        std::shared_ptr<ExprList> args;

        // constructor
        MethodCall(const std::string& name, std::shared_ptr<ExprList> args)
            : name(name), args(args) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // assignment
    struct Assignment : Statement, std::enable_shared_from_this<Assignment> {
        std::string lhs;
        std::shared_ptr<Node> expr;

        // constuctor
        Assignment(const std::string& lhs, std::shared_ptr<Node> expr) 
            : lhs(lhs), expr(expr) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // return
    struct Return : Statement, std::enable_shared_from_this<Return> {
        std::shared_ptr<Node> expr;

        // constuctor
        Return(std::shared_ptr<Node> expr) 
            : expr(expr) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    struct ConditionalBlock : Statement, std::enable_shared_from_this<ConditionalBlock> {
        std::shared_ptr<Node> ifExpr;
        std::shared_ptr<FunctionBody> ifStmts;
        std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<FunctionBody>>> elsifs;
        std::shared_ptr<FunctionBody> elseStmts;

        // constructor
        ConditionalBlock(
            std::shared_ptr<Node> ifExpr,
            std::shared_ptr<FunctionBody> ifStmts,
            std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<FunctionBody>>> elsifs,
            std::shared_ptr<FunctionBody> elseStmts
        ) 
            : ifExpr(ifExpr),
            ifStmts(ifStmts),
            elsifs(elsifs),
            elseStmts(elseStmts)
        {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // while loop
    struct WhileLoop : Statement, std::enable_shared_from_this<WhileLoop> {
        std::shared_ptr<Node> expr;
        std::shared_ptr<FunctionBody> stmts;

        // constructor
        WhileLoop(std::shared_ptr<Node> expr, std::shared_ptr<FunctionBody> stmts)
            : expr(expr), stmts(stmts) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

}

#endif
