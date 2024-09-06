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
        CompStatement(const std::vector<std::shared_ptr<Statement>>& statements) 
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

    // dot operator
    struct DotExpr : Expr, std::enable_shared_from_this<DotExpr> {

        // constructor
        DotExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // primary
    struct Primary : Node {

    };

    // call args
    struct CallArgs : Node, std::enable_shared_from_this<CallArgs> {
        std::vector<std::shared_ptr<Node>> exprs;

        // constructor
        CallArgs(const std::vector<std::shared_ptr<Node>>& exprs)
            : exprs(exprs) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // call
    struct Call : Primary, std::enable_shared_from_this<Call> {
        std::string name;
        std::shared_ptr<CallArgs> args;

        // constructor
        Call(const std::string& name, std::shared_ptr<CallArgs> args)
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
    struct FunctionDefn : Statement, std::enable_shared_from_this<FunctionDefn> {
        std::string name;
        std::string returnType;
        std::shared_ptr<ParamList> paramList;
        std::shared_ptr<CompStatement> functionBody;

        // constuctor
        FunctionDefn(const std::string& name, const std::string& returnType, std::shared_ptr<ParamList> paramList, std::shared_ptr<CompStatement> functionBody) 
            : name(name), returnType(returnType), paramList(paramList), functionBody(functionBody) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // type definition
    struct TypeDefn : Statement, std::enable_shared_from_this<TypeDefn> {
        std::string name;
        std::vector<std::shared_ptr<Node>> members;

        // constuctor
        TypeDefn(const std::string& name, const std::vector<std::shared_ptr<Node>>& members) 
            : name(name), members(members) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // member definition
    struct MemberDefn : Statement, std::enable_shared_from_this<MemberDefn> {
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
        std::shared_ptr<CompStatement> ifStmts;
        std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<CompStatement>>> elsifs;
        std::shared_ptr<CompStatement> elseStmts;

        // constructor
        ConditionalBlock(
            std::shared_ptr<Node> ifExpr,
            std::shared_ptr<CompStatement> ifStmts,
            std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<CompStatement>>> elsifs,
            std::shared_ptr<CompStatement> elseStmts
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
        std::shared_ptr<CompStatement> stmts;

        // constructor
        WhileLoop(std::shared_ptr<Node> expr, std::shared_ptr<CompStatement> stmts)
            : expr(expr), stmts(stmts) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

}

#endif
