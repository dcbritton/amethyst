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

    std::vector<Scope> symbolTable;
    std::vector<std::string> exprTypes;

    bool findVariable(const std::string& name) {
        for (const auto& scope : symbolTable) {
            for (const auto& variable : scope.variables) {
                if (name == variable.name)
                    return true;
            }
        }
        return false;
    }

    bool findFunction(const std::string& name) {
        for (const auto& scope : symbolTable) {
            for (const auto& function : scope.functions) {
                if (name == function.signature)
                    return true;
            }
        }
        return false;
    }

    bool findType(const std::string& name) {
        for (const auto& scope : symbolTable) {
            for (const auto& type : scope.types) {
                if (name == type.name)
                    return true;
            }
        }
        return false;
    }

    void addToScope(Variable variable) {
        symbolTable.back().variables.push_back(variable);
    }

    void addToScope(Function function) {
        symbolTable.back().functions.push_back(function);
    }

    void addToScope(Type type) {
        symbolTable.back().types.push_back(type);
    }

    void endScope() {
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

    // compound statement
    void visit(std::shared_ptr<Node::CompStatement> n) override {
        for (auto stmt : n->statements) {
            stmt->accept(shared_from_this());
        }
    }
    
    // variable definition
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
            if (findVariable(n->name)) {
                std::cout << "Variable " << n->name << " is defined more than once.\n";
                exit(1);
            }
        }

        // type check, match lhs and rhs
        n->expression->accept(shared_from_this());
        if (exprTypes.back() != n->type) {
            std::cout << "In the definition of variable " << n->name
                      << ", the given type " << n->type
                      << " does not match the expression type " << exprTypes.back()
                      << ".\n";
            exit(1);
        }
        // empty the stack
        exprTypes.pop_back();

        // add
        addToScope(Variable(n->name, n->type));
    }

    // function definition
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
        if (!findType(n->returnType)) {
            std::cout << "Return type " << n->returnType
                      << " of function " << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }

        // name check 
        if (findFunction(mangledName)) {
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

    // @TODO: verify types
    void visit(std::shared_ptr<Node::Parameter> n) override {}

    // assignment
    void visit(std::shared_ptr<Node::Assignment> n) override {
        // name check lhs
        // type check, match lhs & rhs
    }

    // return
    void visit(std::shared_ptr<Node::Return> n) override {
        // type check
        std::string functionType = symbolTable.back().functions.back().returnType;
    }

    void visit(std::shared_ptr<Node::Statement> n) override {}

    // @TODO determine expression type
    void visit(std::shared_ptr<Node::EqualityExpr> n) override {
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());
    }

    void visit(std::shared_ptr<Node::RelationExpr> n) override {
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());
    }

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());
    }

    void visit(std::shared_ptr<Node::AdditionExpr> n) override {
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());
    }

    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        std::string rhsType = exprTypes.back();
        exprTypes.pop_back();
        std::string lhsType = exprTypes.back();
        exprTypes.pop_back();

        // @TODO: check implicit conversions
        if (lhsType != rhsType) {
            std::cout << "Type error in expression. Could not convert "
                      << rhsType << " to type " << lhsType << ".\n";
            exit(1);
        }

        exprTypes.push_back(lhsType);
    }

    void visit(std::shared_ptr<Node::DotExpr> n) override {
        n->lhs->accept(shared_from_this());
        n->rhs->accept(shared_from_this());
    }

    void visit(std::shared_ptr<Node::Primary> n) override {}

    void visit(std::shared_ptr<Node::IntLiteral> n) override {
        // expression stack
        exprTypes.push_back("_uint32");
    }
    
    // check only functions scope if in function
    void visit(std::shared_ptr<Node::Variable> n) override {
        // exists?; if in function, don't capture external variables, so check only most recent function scope
        bool functionFound = false;
        auto functionCandidate = symbolTable.crbegin();
        while (functionCandidate != symbolTable.crend()) {
            if (functionCandidate->type == functionDefn) {
                functionFound = true;
                break;
            }
            ++functionCandidate;
        }
        // function scope is found, check only those
        if (functionFound) {
            bool variableFound = false;
            for (const auto& variable : functionCandidate->variables) {
                if (n->name == variable.name) {
                    variableFound = true;
                    break;
                }
            }
            if (!variableFound) {
                std::cout << "Variable " << n->name
                        << " in function " << functionCandidate->functions.back().name
                        << " has not been defined in function scope.\n";
                exit(1);
            }
        }
        // otherwise, name check as normal
        else {
            if (!findVariable(n->name)) {
                std::cout << "Variable " << n->name << " has not been defined.\n";
                exit(1);
            }
        }

        // find type
        std::string type;
        for (const auto& scope : symbolTable) {
            for (const auto& variable : scope.variables) {
                if (n->name == variable.name)
                    type = variable.type;
            }
        }

        // expression stack
        exprTypes.push_back(type);
    }

    void visit(std::shared_ptr<Node::Call> n) override {
        // ensure that call's type, number of args, call's args' types match the definition
        // expression stack
    }

    void visit(std::shared_ptr<Node::CallArgs> n) override {}

    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        // name check
        if (findType(n->name)) {
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
