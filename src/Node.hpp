// Node.hpp

#ifndef NODE
#define NODE

#include <vector>
#include <memory>
#include <string>
#include "Token.hpp"
#include "Info.hpp"
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

        // assigned during semantic analysis
        std::unique_ptr<std::unordered_map<std::string, Type>> types;

        // constructor
        Program(const std::vector<std::shared_ptr<Node>>& definitions) 
        : definitions(definitions) {}

        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // global declaration
    struct GlobalDecl : Node, std::enable_shared_from_this<GlobalDecl> {
        std::string name;
        std::string type;

        // constructor
        GlobalDecl(const std::string& name, const std::string& type)
            : name(name), type(type) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // compound statement
    struct FunctionBody : Node, std::enable_shared_from_this<FunctionBody> {
        std::vector<std::shared_ptr<Node>> statements;

        // constructor
        FunctionBody(const std::vector<std::shared_ptr<Node>>& statements) 
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

    struct RelationalExpr : Expr, std::enable_shared_from_this<RelationalExpr> {

        // constructor
        RelationalExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
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

    struct AdditiveExpr : Expr, std::enable_shared_from_this<AdditiveExpr> {

        // constructor
        AdditiveExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
            : Expr(LHS, op, RHS) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    struct MultiplicativeExpr : Expr, std::enable_shared_from_this<MultiplicativeExpr> {

        // constructor
        MultiplicativeExpr(std::shared_ptr<Node> LHS, const std::string& op, std::shared_ptr<Node> RHS)
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

    // DotRhsMember
    struct DotRhsMember : Primary, std::enable_shared_from_this<DotRhsMember> {
        std::string name;

        // @NOTE: assigned during semantic analysis
        std::string type;

        // constructor
        DotRhsMember(const std::string& name)
            : name(name) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // dot rhs method call
    struct DotRhsMethodCall : Primary, std::enable_shared_from_this<DotRhsMethodCall> {
        std::string name;
        std::shared_ptr<ExprList> args;

        // assigned in semantic analysis
        std::string type;
        uint32_t numArgs;
        std::string signature;

        // constructor
        DotRhsMethodCall(const std::string& name, std::shared_ptr<ExprList> args)
            : name(name), args(args) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // new expr
    struct NewExpr : Node, std::enable_shared_from_this<NewExpr> {
        std::string type;
        std::shared_ptr<ExprList> args;

        // assigned during semantic analysis
        std::string signature;

        // constructor
        NewExpr(const std::string& type, std::shared_ptr<ExprList> args)
            : type(type), args(args) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // stack expr
    struct StackExpr : Node, std::enable_shared_from_this<StackExpr> {
        std::string type;
        std::string number;

        // constructor
        StackExpr(const std::string& type, const std::string& number)
            : type(type), number(number) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }   
    };

    // heap expr
    struct HeapExpr : Node, std::enable_shared_from_this<HeapExpr> {
        std::string type;

        std::shared_ptr<Node> expr;

        // constructor
        HeapExpr(const std::string& type, std::shared_ptr<Node> expr)
            : type(type), expr(expr) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }   
    };

    // global
    struct Global : Primary, std::enable_shared_from_this<Global> {
        std::string name;

        // @NOTE: assigned during semantic analysis
        std::string type;

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

        // assigned in semantic analysis
        std::string type;
        uint32_t numArgs;
        std::string signature;

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

    // bool literal
    struct BoolLiteral : Primary, std::enable_shared_from_this<BoolLiteral> {
        std::string value;

        // constructor
        BoolLiteral(const std::string& value)
            : value(value) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // char literal
    struct CharLiteral : Primary, std::enable_shared_from_this<CharLiteral> {
        std::string value;

        // constructor
        CharLiteral(const std::string& value)
            : value(value) {}

        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // variable
    struct Variable : Primary, std::enable_shared_from_this<Variable> {
        std::string name;

        // @NOTE: assigned during semantic analysis
        std::string type;

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

        // assigned during semantic analysis
        std::unique_ptr<Procedure> info = nullptr;

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
        std::vector<std::shared_ptr<MemberDecl>> members;
        std::vector<std::shared_ptr<ConstructorDefn>> constructors; 
        std::vector<std::shared_ptr<MethodDefn>> methods;
        std::vector<std::shared_ptr<OperatorDefn>> operators;

        // constuctor
        TypeDefn(const std::string& name, const std::vector<std::shared_ptr<MemberDecl>>& members, const std::vector<std::shared_ptr<ConstructorDefn>>& constructors, const std::vector<std::shared_ptr<MethodDefn>>& methods, const std::vector<std::shared_ptr<OperatorDefn>>& operators) 
            : name(name), members(members), constructors(constructors), methods(methods), operators(operators) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // operator definition
    struct OperatorDefn : Node, std::enable_shared_from_this<OperatorDefn> {
        std::string op;
        std::shared_ptr<Parameter> parameter;
        std::string returnType;
        std::shared_ptr<FunctionBody> stmts;

        // assigned during semantic analysis
        std::unique_ptr<Procedure> info = nullptr;

        OperatorDefn(const std::string& op, std::shared_ptr<Parameter> parameter, const std::string& returnType, std::shared_ptr<FunctionBody> stmts)
            : op(op), parameter(parameter), returnType(returnType), stmts(stmts) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // constructor definition
    struct ConstructorDefn : Node, std::enable_shared_from_this<ConstructorDefn> {
        std::shared_ptr<ParamList> parameters;
        std::shared_ptr<FunctionBody> body;

        // assigned during semantic analysis
        std::unique_ptr<Procedure> info = nullptr;

        ConstructorDefn(std::shared_ptr<ParamList> parameters, std::shared_ptr<FunctionBody> body)
            : parameters(parameters), body(body) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }  
    };

    // member definition
    struct MemberDecl : Node, std::enable_shared_from_this<MemberDecl> {
        std::string name;
        std::string type;

        // constructor
        MemberDecl(const std::string& name, const std::string& type)
            : name(name), type(type) {}

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

        // assigned during semantic analysis
        std::unique_ptr<Procedure> info = nullptr;

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

        // @NOTE: assigned during semantic analysis
        std::string type;

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

        // assigned in semantic analysis
        std::string type;
        uint32_t numArgs;
        std::string signature;

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
        std::shared_ptr<Node> LHS;
        std::shared_ptr<Node> RHS;

        // constuctor
        Assignment(std::shared_ptr<Node> LHS, std::shared_ptr<Node> RHS) 
            : LHS(LHS), RHS(RHS) {}

        // accept visitor
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // unheap
    struct Unheap : Statement, std::enable_shared_from_this<Unheap> {
        std::shared_ptr<Node> expr;

        // constructor
        Unheap(std::shared_ptr<Node> expr)
            : expr(expr) {}

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

    struct Conditional : Statement, std::enable_shared_from_this<Conditional> {
        std::shared_ptr<Node> ifExpr;
        std::shared_ptr<FunctionBody> ifStmts;
        std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<FunctionBody>>> elsifs;
        std::shared_ptr<FunctionBody> elseStmts;

        // constructor
        Conditional(
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
    
    // break
    struct Break : Statement, std::enable_shared_from_this<Break> {
        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // continue
    struct Continue : Statement, std::enable_shared_from_this<Continue> {
        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };

    // redo
    struct Redo : Statement, std::enable_shared_from_this<Redo> {
        // accept
        void accept(std::shared_ptr<Visitor> v) override {
            v->visit(shared_from_this());
        }
    };
}

#endif
