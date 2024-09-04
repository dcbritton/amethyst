// SemanticAnalyzerVisitor.hpp

#ifndef SEMANTICANALYZERVISITOR
#define SEMANTICANALYZERVISITOR

#include <iostream>
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

        Variable(const std::string& name, const std::string& type)
            : name(name), type(type) {}
    };

    struct Function {
        std::string name;
        std::string signature;
        std::string returnType;
        std::vector<Variable> parameters;

        Function(const std::string& name, const std::string& signature, const std::string& returnType, const std::vector<Variable>& parameters) 
            : name(name), signature(signature), returnType(returnType), parameters(parameters) {}
    };

    struct Type {
        std::string name;
        std::vector<Variable> members;
        std::vector<Function> methods;

        Type(const std::string& name, const std::vector<Variable>& members, const std::vector<Function>& methods)
            : name(name), members(members), methods(methods) {}
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
        addToScope(Type("_uint32", {}, {}));    

        n->compStatement->accept(shared_from_this());
    }

    // compstatement
    void visit(std::shared_ptr<Node::CompStatement> n) override {
        for (auto stmt : n->statements) {
            stmt->accept(shared_from_this());
        }
    }

    // @TODO: look back at scope, check for function defn
    void visit(std::shared_ptr<Node::VariableDefn> n) override {
        // exists?; if in function, don't capture external variables, so check only most recent function scope
        bool found = false;
        auto functionCandidate = symbolTable.crbegin();
        while (functionCandidate != symbolTable.crend()) {
            if (functionCandidate->type == functionDefn) {
                found = true;
                break;
            }
            ++functionCandidate;
        }
        // function scope is found, check only those
        if (found) {
            for (const auto& variable : functionCandidate->variables) {
                if (n->name == variable.name) {
                    std::cout << "Variable " << n->name
                              << " is already defined in function " << functionCandidate->functions.back().name
                              << ".\n";
                    exit(1);
                }
            }
        }
        // otherwise, name check as normal
        else {
            if (variables.find(n->name) != variables.end()) {
                std::cout << "Variable " << n->name << " is defined more than once.\n";
                exit(1);
            }
        }


        // type check, match lhs, rhs

        // add
        addToScope(Variable(n->name, n->type));
    }

    // @TODO: functions should not capture variables from outside function scope?
    void visit(std::shared_ptr<Node::FunctionDefn> n) override {
        // process parameters
        // @TODO: this violates visitor pattern. find a way to propagate the names and types up (return values)?
        std::vector<Variable> parameters = {};
        std::string mangledName = n->name + "!" + n->returnType;
        for (const auto& parameter : n->paramList->parameters) {
            parameters.push_back(Variable(parameter->name, parameter->type));
            mangledName += "@" + parameter->type;
        }

        // return type check
        if (types.find(n->returnType) == types.end()) {
            std::cout << "Return type " << n->returnType
                      << " of function " << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }

        // name check 
        if (functions.find(mangledName) != functions.end()) {
            std::cout << "Function " << n->name << " is defined more than once with the same return type and parameter types.\n"
                      << "(Mangled: " << mangledName << ")\n";
            exit(1);    
        }
        
        // make new scope
        symbolTable.push_back(Scope(functionDefn));

        // add itself and parameters to its own scope
        addToScope(Function(n->name, mangledName, n->returnType, parameters));
        for (const auto& parameter : parameters) {
            addToScope(Variable(parameter.name, parameter.type));
        }

        // process function body
        n->functionBody->accept(shared_from_this());

        // end its own scope, remove itself & parameters
        endScope();

        // add function to scope outside itself
        addToScope(Function(n->name, mangledName, n->returnType, parameters));
    }

    // never called with current implementation, handled by FunctionDefn
    void visit(std::shared_ptr<Node::ParamList> n) override {}

    // never called with current implementation, handled by FunctionDefn
    void visit(std::shared_ptr<Node::Parameter> n) override {}

    void visit(std::shared_ptr<Node::Assignment> n) override {
        // name check lhs
        // type check, match lhs & rhs
    }

    void visit(std::shared_ptr<Node::Return> n) override {
        // type check
        std::string functionType = symbolTable.back().functions.back().returnType;
    }

    void visit(std::shared_ptr<Node::Statement> n) override {}

    // @TODO determine expression type
    void visit(std::shared_ptr<Node::EqualityExpr> n) override {}

    void visit(std::shared_ptr<Node::RelationExpr> n) override {}

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {}

    void visit(std::shared_ptr<Node::AdditionExpr> n) override {}

    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {}

    void visit(std::shared_ptr<Node::DotExpr> n) override {}

    void visit(std::shared_ptr<Node::Primary> n) override {}

    void visit(std::shared_ptr<Node::IntLiteral> n) override {}
    
    void visit(std::shared_ptr<Node::Variable> n) override {
        // exists?
        // expression stack
    }

    void visit(std::shared_ptr<Node::Call> n) override {
        // ensure that call's type, number of args, call's args' types match the definition
        // expression stack
    }

    void visit(std::shared_ptr<Node::CallArgs> n) override {}

    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        // name check
        if (types.find(n->name) != types.end()) {
            std::cout << "Type " << n->name << " is already defined.\n";
            exit(1);
        }
        // process members

        // add
        // @TODO: process members
        addToScope(Type(n->name, {}, {}));
    }

    void visit(std::shared_ptr<Node::MemberDefn> n) override {

    }

    void visit(std::shared_ptr<Node::ConditionalBlock> n) override {}

    void visit(std::shared_ptr<Node::WhileLoop> n) override {}


};

#endif
