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
        methodDefn,
        overloadDefn,

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

    struct OperatorOverload {
        // the operator and parameter (rhs) type are used to differentiate operators of a type, return type is not
        std::string op;
        std::string rhsType;
        std::string returnType;

        // constructor
        OperatorOverload(const std::string& op, const std::string& rhsType, const std::string& returnType)
            : op(op), rhsType(rhsType), returnType(returnType) {}
    };

    std::string formOpSignature(const std::string& op, const std::string& rhsType) {
        return op + "(:" + rhsType + "):";
    }

    struct Type {
        std::string name;
        std::vector<Variable> members;
        std::vector<Function> methods;
        // @TODO: rewrite using a hash structure
        std::unordered_map<std::string, OperatorOverload> overloads;

        Type(const std::string& name, const std::vector<Variable>& members, const std::vector<Function>& methods, const std::unordered_map<std::string, OperatorOverload>& overloads)
            : name(name), members(members), methods(methods), overloads(overloads) {}

        bool has(const std::string& signature) {
            return overloads.find(signature) != overloads.end();
        }

        OperatorOverload getOverload(const std::string& signature) {
            return overloads.find(signature)->second;
        }
    };

    struct Scope {
        ScopeType type = undefined;
        std::unordered_map<std::string, Variable> variables;
        std::string name; // name of function/type for definition

        // constructor
        Scope(ScopeType type, const std::string& name)
            : type(type), name(name) {}
    };

    // stack containing variables at all scopes
    std::vector<Scope> scopeTable;
    // contains available types (types are only declarable in global)
    std::unordered_map<std::string, Type> types;
    // contains available functions (functions are only declarable in global)
    std::unordered_map<std::string, Function> functions;
    // stack used for tracking the types of subexpressions as an expression is evaluated
    std::vector<std::string> exprTypes;

    bool inFunction() {
        for (auto it = scopeTable.begin(); it != scopeTable.end(); ++it) {
            if (it->type == functionDefn) {
                return true;
            }
        }

        return false;
    }

    // fails if not in a function defn
    std::vector<Scope>::const_iterator getFunctionDefnScope() {
        auto it = scopeTable.begin();
        for (; it != scopeTable.end(); ++it) {
            if (it->type == functionDefn) {
                return it;
            }
        }
        
        std::cout << "Internal error. getFunctionDefnScope() called when not in a function definition.\n";
        exit(1);
    }

    bool inTypeDefn() {
        for (auto it = scopeTable.begin(); it != scopeTable.end(); ++it) {
            if (it->type == typeDefn) {
                return true;
            }
        }
        return false;
    }

    // fails if not if a type defn
    std::vector<Scope>::const_iterator getTypeDefnScope() {
        auto it = scopeTable.begin();
        for (; it != scopeTable.end(); ++it) {
            if (it->type == typeDefn) {
                return it;
            }
        }
        std::cout << "Internal error. getTypeDefnScope() called when not in a type definition.\n";
        exit(1); 
    }

    bool variableExists(const std::string& name) {
        // anything in a type (methods, operators) should have access to (previously declared?) members, but nothing outside the type
        if (inTypeDefn()) {
            for (auto typeScope = getTypeDefnScope(); typeScope != scopeTable.end(); ++typeScope) {
                if (typeScope->variables.find(name) != typeScope->variables.end()) {
                    return true;
                }
            }
        }
        // functions don't capture external variables, so check only past the most recent function scope
        else if (inFunction()) {
            // get the function scope & check only past the most recent function scope
            for (auto functionScope = getFunctionDefnScope(); functionScope != scopeTable.end(); ++functionScope) {
                if (functionScope->variables.find(name) != functionScope->variables.end()) {
                    return true;
                }
            }
        }
        // otherwise, check all scopes
        else {
            for (const auto& scope : scopeTable) {
                if (scope.variables.find(name) != scope.variables.end()) {
                    return true;
                }
            }
        }

        return false;
    }

    bool functionExists(const std::string& signature) {
        return functions.find(signature) != functions.end();
    }

    bool typeExists(const std::string& name) {
        return types.find(name) != types.end();
    }

    void enterScope(ScopeType type, const std::string& name) {
        scopeTable.push_back(Scope(type, name));
    }

    void addToScope(Variable variable) {
        scopeTable.back().variables.emplace(variable.name, variable);
    }

    void endScope() {
        scopeTable.pop_back();
    }

    void printScopeTableNames() {
        for (auto& scope: scopeTable) {
            std::cout << scope.name << ", ";
        }
        std::cout << "\n";
    }

    void printExprStack() {
        for (auto& exprType : exprTypes) {
            std::cout << exprType << ", ";
        }
        std::cout << "\n";
    }

    void visit(std::shared_ptr<Node::Node> n) override {
        
    }

    // visit program
    void visit(std::shared_ptr<Node::Program> n) override {

        enterScope(global, "global");

        // @TODO: predefined variables, functions, types may go here with addToScope()
        types.emplace("_int32", Type("_int32", {}, {}, {
            std::make_pair<std::string, OperatorOverload>(formOpSignature("+", "_int32"), OperatorOverload("+", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("-", "_int32"), OperatorOverload("-", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("!=", "_int32"), OperatorOverload("!=", "_int32", "_int32")),
            // std::make_pair<std::string, OperatorOverload>(formOpSignature("!", "_int32"), OperatorOverload("!", "_int32", "_int32")),
            // std::make_pair<std::string, OperatorOverload>(formOpSignature(".", "_int32"), OperatorOverload(".", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("==", "_int32"), OperatorOverload("==", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("*", "_int32"), OperatorOverload("*", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("/", "_int32"), OperatorOverload("/", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("%", "_int32"), OperatorOverload("%", "_int32", "_int32")),
            // std::make_pair<std::string, OperatorOverload>(formOpSignature("&", "_int32"), OperatorOverload("&", "_int32", "_int32")),
            // std::make_pair<std::string, OperatorOverload>(formOpSignature("|", "_int32"), OperatorOverload("|", "_int32", "_int32")),
            // std::make_pair<std::string, OperatorOverload>(formOpSignature("||", "_int32"), OperatorOverload("||", "_int32", "_int32")),
            // std::make_pair<std::string, OperatorOverload>(formOpSignature("&&", "_int32"), OperatorOverload("&&", "_int32", "_int32")),
            // std::make_pair<std::string, OperatorOverload>(formOpSignature("^", "_int32"), OperatorOverload("^", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("<<", "_int32"), OperatorOverload("<<", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature(">>", "_int32"), OperatorOverload(">>", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("<", "_int32"), OperatorOverload("<", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("<=", "_int32"), OperatorOverload("<=", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature(">", "_int32"), OperatorOverload(">", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature(">=", "_int32"), OperatorOverload(">=", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("and", "_int32"), OperatorOverload("and", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("or", "_int32"), OperatorOverload("or", "_int32", "_int32")),
            // std::make_pair<std::string, OperatorOverload>(formOpSignature("[]", "_int32"), OperatorOverload("[]", "_int32", "_int32")),
        }));

        types.emplace("_float32", Type("_float32", {}, {}, {
            std::make_pair<std::string, OperatorOverload>(formOpSignature("+", "_int32"), OperatorOverload("+", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("-", "_int32"), OperatorOverload("-", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("!=", "_int32"), OperatorOverload("!=", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("==", "_int32"), OperatorOverload("==", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("*", "_int32"), OperatorOverload("*", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("/", "_int32"), OperatorOverload("/", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("<", "_int32"), OperatorOverload("<", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature("<=", "_int32"), OperatorOverload("<=", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature(">", "_int32"), OperatorOverload(">", "_int32", "_int32")),
            std::make_pair<std::string, OperatorOverload>(formOpSignature(">=", "_int32"), OperatorOverload(">=", "_int32", "_int32")),

        }));
        types.emplace("_string", Type("_string", {}, {}, {}));

        for (auto definition : n->definitions) {
            definition->accept(shared_from_this());
        }
    }

    // visit compound statement
    void visit(std::shared_ptr<Node::FunctionBody> n) override {
        for (auto stmt : n->statements) {
            stmt->accept(shared_from_this());
        }
    }
    
    // variable definition
    void visit(std::shared_ptr<Node::VariableDefn> n) override {
        // exists?
        if (variableExists(n->name)) {
            std::cout << "In some scope, " << n->name << " is defined more than once.\n";
            exit(1);
        }

        // does n->type exist?
        if (!typeExists(n->type)) {
            std::cout << "The given type " << n->type
                      << " of variable " << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }
        
        // match lhs and rhs type
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

    // visit function definition
    void visit(std::shared_ptr<Node::FunctionDefn> n) override {
        // process parameters
        n->paramList->accept(shared_from_this());
        // @NOTE: this violates visitor pattern
        std::vector<Variable> parameters = {};
        std::string signature = n->name;
        for (const auto& parameter : n->paramList->parameters) {
            parameters.push_back(Variable(parameter->name, parameter->type));
            signature += "@" + parameter->type;
        }

        // return type check
        if (!typeExists(n->returnType)) {
            std::cout << "Return type " << n->returnType
                      << " of function " << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }

        // name check 
        if (functionExists(signature)) {
            std::cout << "Function " << n->name << " is defined more than once with the same parameter types.\n"
                      << "(Mangled: " << signature << ")\n";
            exit(1);    
        }
        
        // make new scope
        enterScope(functionDefn, signature);

        // add the function (including to its own scope)
        functions.emplace(signature, Function(n->name, signature, n->returnType, parameters));
        
        // add parameters to scope
        for (const auto& parameter : parameters) {
            addToScope(Variable(parameter.name, parameter.type));
        }

        // process function body
        n->functionBody->accept(shared_from_this());

        // end its own scope, remove itself & parameters
        endScope();
    }

    // visit parameter list
    void visit(std::shared_ptr<Node::ParamList> n) override {
        for (const auto& parameter : n->parameters) {
            parameter->accept(shared_from_this());
        }
    }

    // visit parameter
    void visit(std::shared_ptr<Node::Parameter> n) override {
        // type exists?
        if (!typeExists(n->type)) {
            std::cout << "The given type " << n->type
                      << " of parameter " << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }
    }

    // visit assignment
    void visit(std::shared_ptr<Node::Assignment> n) override {

        // exists?
        if (!variableExists(n->lhs)) {
            std::cout << "In some scope, tried to assign a value to " << n->lhs << ", which has not been defined.\n";
            exit(1);
        }

        // find type
        std::string type;
        for (const auto& scope : scopeTable) {
            for (const auto& variable : scope.variables) {
                if (n->lhs == variable.first) {
                    type = variable.second.type;
                }
            }
        }

       // match lhs and rhs type
        n->expr->accept(shared_from_this());
        if (exprTypes.back() != type) {
            std::cout << "In an assignment of variable " << n->lhs
                      << ", its type " << type
                      << " does not match the expression type " << exprTypes.back()
                      << ".\n";
            exit(1);
        }
        // empty the stack
        exprTypes.pop_back();
    }

    // visit return
    // @TODO: check for returns?
    // @TODO: disallow if not in a functiondefn
    void visit(std::shared_ptr<Node::Return> n) override {

        // process expression
        n->expr->accept(shared_from_this());
        
        // match return type to expression type
        std::string functionName = getFunctionDefnScope()->name;
        std::string functionType = functions.find(functionName)->second.returnType;
        if (functionType != exprTypes.back()) {
            std::cout << "In a return in the function " << functionName
                        << ", the function's return type " << functionType
                        << " does not match the expression type " << exprTypes.back()
                        << ".\n";
            exit(1);
        }

        // empty the stack
        exprTypes.pop_back();
    }

    void visit(std::shared_ptr<Node::Statement> n) override {}

    // a method for common expression functionality
    void process(std::shared_ptr<Node::Expr> n) {
        // process lhs and rhs
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get lhs and rhs types of expr type stack
        std::string rhsType = exprTypes.back();
        exprTypes.pop_back();
        std::string lhsType = exprTypes.back();
        exprTypes.pop_back();

        // check the type and see if it has
        std::string signature = formOpSignature(n->op, rhsType);
        Type& type = types.find(lhsType)->second;
        if (!type.has(signature)) {
            std::cout << "Type " << type.name << " has no defined op " << signature << ".\n";
            exit(1);
        }
        exprTypes.push_back(type.getOverload(signature).returnType);
    }

    // @TODO: every kind of expr needs to check lhs and rhs against available operator overloads
    void visit(std::shared_ptr<Node::LogicalExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::EqualityExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::RelationExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::AdditionExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::AccessExpr> n) override {
        // @TODO: type inside [] does not have to match outside
        // requires special interaction with call stack
        // @TODO: make sure lhs of . has member rhs
        // no calls on rhs for now, no (expr), no literals, etc 
        process(n);
    }

    void visit(std::shared_ptr<Node::Primary> n) override {}

    // visit array
    // @NOTE: violates visitor pattern
    void visit(std::shared_ptr<Node::Array> n) override {
        // verify types consistency across all elements
        std::vector<std::string> types;
        for (auto& expr : n->exprs->exprs) {
            expr->accept(shared_from_this());
            types.push_back(exprTypes.back());
            exprTypes.pop_back();
        }
        if (!types.empty()) {
            std::string firstType = types.front();
            for (const auto& type : types) {
                if (type != firstType) {
                    std::cout << "In an array literal, the 1st element is of type " << firstType
                              << ", but another element is of type " << type << ".\n";
                    exit(1);
                }
            }
        }

        // expr stack
        if (types.empty()) {
            exprTypes.push_back("any&0");
        }
        else {
            exprTypes.push_back(types.front() + "&" + std::to_string(types.size()));
        }
    }

    // visit int literal
    void visit(std::shared_ptr<Node::IntLiteral> n) override {
        // expression stack
        exprTypes.push_back("_int32");
    }

    // visit float literal
    void visit(std::shared_ptr<Node::FloatLiteral> n) override {
        // expression stack
        exprTypes.push_back("_float32");
    }

    // visit string literal
    void visit(std::shared_ptr<Node::StringLiteral> n) override {
        // expression stack
        // @TODO: should in fact be an array of char/_uint8
        exprTypes.push_back("_string");
    }
    
    // visit variable
    void visit(std::shared_ptr<Node::Variable> n) override {
        // exists?
        if (!variableExists(n->name)) {
            std::cout << "Variable " << n->name << " referenced in some scope, but it has not yet been defined.\n";
            exit(1);
        }

        // find type
        std::string type;
        for (const auto& scope : scopeTable) {
            for (const auto& variable : scope.variables) {
                if (n->name == variable.second.name)
                    type = variable.second.type;
            }
        }

        // expression stack
        exprTypes.push_back(type);
    }

    // visit call
    // @TODO: method call
    // @NOTE: this violates visitor pattern.
    void visit(std::shared_ptr<Node::Call> n) override {
        // determine argument expression types & deal with expr stack
        std::vector<std::string> types = {};
        for (auto& expr : n->args->exprs) {
            expr->accept(shared_from_this());
            types.push_back(exprTypes.back());
            exprTypes.pop_back();
        }

        // create function signature
        std::string signature = n->name;
        for (const std::string& type : types) {
            signature += "@" + type;
        }

        // function exists?
        if (!functionExists(signature)) {
            std::cout << "Could not find a matching definition for call with signature " << signature << ".\n";
            exit(1);
        }

        // push type to expression stack
        std::string type = functions.find(signature)->second.returnType;
        exprTypes.push_back(type);
    }

    // visit expr list
    // @NOTE: violation of visitor pattern, never called
    void visit(std::shared_ptr<Node::ExprList> n) override {

    }

    // visit type definition
    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        // name check
        if (typeExists(n->name)) {
            std::cout << "Type " << n->name << " is already defined.\n";
            exit(1);
        }

        // enter scope
        enterScope(typeDefn, n->name);

        // process members
        std::vector<Variable> members;
        for (auto member : n->members) {
            member->accept(shared_from_this());
            // @NOTE: violates visitor pattern
            members.push_back(Variable(member->name, member->type));
        }

        // process methods
        // @NOTE: violates visitor pattern
        std::vector<Function> methods;
        for (auto method : n->methods) {
            // process parameters
            method->paramList->accept(shared_from_this());
            // @NOTE: this violates visitor pattern
            std::vector<Variable> parameters = {};
            std::string signature = method->name;
            for (const auto& parameter : method->paramList->parameters) {
                parameters.push_back(Variable(parameter->name, parameter->type));
                signature += "@" + parameter->type;
            }

            // return type check
            if (!typeExists(method->returnType)) {
                std::cout << "Return type " << method->returnType
                        << " of function " << method->name
                        << " has not yet been defined.\n";
                exit(1);
            }

            // no two methods with the same signature in a type
            for (const auto& m : methods) {
                if (signature == m.signature) {
                    std::cout << "Method " << method->name << " in type " << n->name << " is defined more than once with the same parameter types.\n"
                            << "(Mangled: " << signature << ")\n";  
                    exit(1);
                }
            }
 
            // make new scope
            enterScope(methodDefn, method->name);

            // add the method to the type
            methods.push_back(Function(method->name, signature, method->returnType, parameters));
            
            // add parameters to scope
            for (const auto& parameter : parameters) {
                addToScope(Variable(parameter.name, parameter.type));
            }

            // process function body
            method->functionBody->accept(shared_from_this());

            // end its own scope, remove itself & parameters
            endScope();
        }

        // process overloads
        // @NOTE: violates visitor pattern
        std::unordered_map<std::string, OperatorOverload> overloads;
        for (const auto& overload : n->opOverloads) {
            // ensure no repeats
            std::string signature = formOpSignature(overload->op, overload->parameter->type);
            if (overloads.find(signature) != overloads.end()) {
                std::cout << "Type " << n->name << "'s overload of op " << signature << " is defined more than once!\n";
                exit(1);
            }
            overloads.emplace(signature, OperatorOverload(overload->op, overload->parameter->type, overload->returnType));

            // process each overload similarly to function
            overload->accept(shared_from_this());
        }

        // end scope
        endScope();

        // add
        types.emplace(n->name, Type(n->name, std::move(members), std::move(methods), std::move(overloads)));
    }

    // visit operator overload
    void visit(std::shared_ptr<Node::OperatorOverload> n) override {
        
        // process parameter
        n->parameter->accept(shared_from_this());

        // get typename
        std::string typeName = getTypeDefnScope()->name;

        // return type check
        if (!typeExists(n->returnType)) {
            std::cout << "Return type " << n->returnType
                      << " of type " << typeName << "'s overload of op " << n->op << "(:" << n->parameter->type
                      << "): has not yet been defined.\n";
            exit(1);
        }
        
        // make new scope
        enterScope(overloadDefn, n->op + "(:" + n->parameter->type + "):");
        
        // add parameter to scope
        addToScope(Variable(n->parameter->name, n->parameter->type));

        // process function body
        n->stmts->accept(shared_from_this());

        // end its own scope, remove itself & parameters
        endScope();    
    }

    void visit(std::shared_ptr<Node::ConditionalBlock> n) override {}

    void visit(std::shared_ptr<Node::WhileLoop> n) override {}


};

#endif
