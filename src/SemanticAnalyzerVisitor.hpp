// SemanticAnalyzerVisitor.hpp

#ifndef SEMANTICANALYZERVISITOR
#define SEMANTICANALYZERVISITOR

#include <unordered_map>
#include "Visitor.hpp"

struct SemanticAnalyzerVisitor : Visitor {

    enum ScopeType {
        undefined,
        global,
        functionDefn,
        typeDefn,
        whileLoop,
        conditionalBlock
    };

    struct Variable {
        std::string name;
        std::string type;
    };

    struct Function {
        std::string name;
        std::vector<Variable> parameters;
    };

    struct Type {
        std::string name;
        std::vector<Variable> members;
        // @TODO: methods
    };

    struct Scope {
        ScopeType type = undefined;
        std::vector<Type> types;
        std::vector<Variable> variables;
        std::vector<Function> functions;

        Scope(ScopeType type)
            : type(type) {}
    };

    std::unordered_map<std::string, Variable*> variables;
    std::unordered_map<std::string, Function*> functions;
    std::unordered_map<std::string, Type*> types;
    std::vector<Scope> symbolTable;

    void addToScope(Variable&& variable) {
        symbolTable.back().variables.push_back(std::move(variable));
        Variable& variableRef = symbolTable.back().variables.back();
        variables.emplace(variableRef.name, &variableRef);
    }

    void addToScope(Function&& function) {
        symbolTable.back().functions.push_back(std::move(function));
        Function& functionRef = symbolTable.back().functions.back();
        functions.emplace(functionRef.name, &functionRef);
    }

    void addToScope(Type&& type) {
        symbolTable.back().types.push_back(std::move(type));
        Type& typeRef = symbolTable.back().types.back();
        types.emplace(typeRef.name, &typeRef);
    }

    void endScope() {
        for (auto& variable : symbolTable.back().variables) {
            variables.erase(variable.name);
        }
        for (auto& function : symbolTable.back().functions) {
            functions.erase(function.name);
        }
        for (auto& type : symbolTable.back().types) {
            types.erase(type.name);
        }
        symbolTable.pop_back();
    }

    void visit(std::shared_ptr<Node::Node> n) override {
        
    }

    // program
    void visit(std::shared_ptr<Node::Program> n) override {
        symbolTable.push_back(Scope(global));
        // @TODO: predefined variables, functions, types may go here with addToScope()

        n->compStatement->accept(shared_from_this());
    }

    // compstatement
    void visit(std::shared_ptr<Node::CompStatement> n) override {
        for (auto stmt : n->statements) {
            stmt->accept(shared_from_this());
        }
    }

    void visit(std::shared_ptr<Node::VariableDefn> n) override {
        // name check
        // add
    }

    void visit(std::shared_ptr<Node::FunctionDefn> n) override {
        // name check
        // add
    }

    void visit(std::shared_ptr<Node::ParamList> n) override {}

    void visit(std::shared_ptr<Node::Parameter> n) override {
        // add parameters to function's scope
    }

    void visit(std::shared_ptr<Node::Assignment> n) override {
        // name check lhs
        // type check, match lhs & rhs
    }

    void visit(std::shared_ptr<Node::Return> n) override {
        // type check
    }

    void visit(std::shared_ptr<Node::Statement> n) override {}

    void visit(std::shared_ptr<Node::EqualityExpr> n) override {}

    void visit(std::shared_ptr<Node::RelationExpr> n) override {}

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {}

    void visit(std::shared_ptr<Node::AdditionExpr> n) override {}

    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {}

    void visit(std::shared_ptr<Node::DotExpr> n) override {}

    void visit(std::shared_ptr<Node::Primary> n) override {}

    void visit(std::shared_ptr<Node::IntLiteral> n) override {}
    
    void visit(std::shared_ptr<Node::Variable> n) override {
        // name check
        // type check
        
        // add
    }

    void visit(std::shared_ptr<Node::Call> n) override {
        // ensure that call's type, number of args, call's args' types match the definition
    }

    void visit(std::shared_ptr<Node::CallArgs> n) override {}

    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        // name check
        // add
    }

    void visit(std::shared_ptr<Node::MemberDefn> n) override {

    }

    void visit(std::shared_ptr<Node::ConditionalBlock> n) override {}

    void visit(std::shared_ptr<Node::WhileLoop> n) override {}


};

#endif
