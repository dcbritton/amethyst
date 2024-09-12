// SemanticAnalyzerVisitor.hpp

#ifndef SEMANTICANALYZERVISITOR
#define SEMANTICANALYZERVISITOR

#include <unordered_set>
#include <unordered_map>
#include <optional>
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

    // @TODO: rewrite using hashes of some kind
    struct OperatorOverload {

        // the operator and parameter (rhs) type are used to differentiate operators of a type, return type is not
        std::string op;
        std::string rhsType;
        std::string returnType;

        // overload equality
        bool operator==(const OperatorOverload& other) {
            return (this->op == other.op && this->rhsType == other.rhsType);
        }

        // constructor
        OperatorOverload(const std::string& op, const std::string& rhsType, const std::string& returnType)
            : op(op), rhsType(rhsType), returnType(returnType) {}
    };

    struct Type {
        std::string name;
        std::vector<Variable> members;
        std::vector<Function> methods;
        std::vector<OperatorOverload> overloads;

        Type(const std::string& name, const std::vector<Variable>& members, const std::vector<Function>& methods, const std::vector<OperatorOverload>& overloads)
            : name(name), members(members), methods(methods), overloads(overloads) {}

        bool has(const OperatorOverload& opOverload) {
            return std::find(overloads.begin(), overloads.end(), opOverload) != overloads.end();
        }
    };

    struct Scope {
        ScopeType type = undefined;
        std::unordered_map<std::string, Variable> variables;
        // name of function/type for definition
        std::optional<std::string> name;

        // constructor for type and function/method definitions
        Scope(ScopeType type, std::optional<std::string> name)
            : type(type), name(name) {}
    };

    // contains variables at all scopes
    std::vector<Scope> variableTable;
    // contains available types (types are only declarable in global)
    std::unordered_map<std::string, Type> types;
    // contains available functions (functions are only declarable in global)
    std::unordered_map<std::string, Function> functions;
    // stack used for tracking the types of subexpressions as an expression is evaluated
    std::vector<std::string> exprTypes;

    bool inFunction() {
        for (const auto& scope : variableTable) {
            if (scope.type == functionDefn) {
                return true;
            }
        }
        return false;
    }

    bool findVariable(const std::string& name) {
        // functions don't capture external variables, so check only most recent function scope
        if (inFunction()) {
            if (variableTable.back().variables.find(name) != variableTable.back().variables.end()) {
                return true;
            }
        }
        // otherwise, check all scopes
        else {
            for (const auto& scope : variableTable) {
                if (scope.variables.find(name) != scope.variables.end()) {
                    return true;
                }
            }
        }
        
        return false;
    }

    bool findFunction(const std::string& signature) {
        return functions.find(signature) != functions.end();
    }

    bool findType(const std::string& name) {
        return types.find(name) != types.end();
    }

    void addToScope(Variable variable) {
        variableTable.back().variables.emplace(variable.name, variable);
    }

    void endScope() {
        variableTable.pop_back();
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

        variableTable.push_back(Scope(global, std::nullopt));

        // @TODO: predefined variables, functions, types may go here with addToScope()
        types.emplace("_int32", Type("_int32", {}, {}, {}));
        types.emplace("_float32", Type("_float32", {}, {}, {}));
        types.emplace("_string", Type("_string", {}, {}, {}));

        n->compStatement->accept(shared_from_this());
    }

    // visit compound statement
    void visit(std::shared_ptr<Node::CompStatement> n) override {
        for (auto stmt : n->statements) {
            stmt->accept(shared_from_this());
        }
    }
    
    // variable definition
    void visit(std::shared_ptr<Node::VariableDefn> n) override {
        // exists?
        if (findVariable(n->name)) {
            std::cout << "In some scope, " << n->name << " is defined more than once.\n";
            exit(1);
        }

        // does n->type exist?
        if (!findType(n->type)) {
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
    // @NOTE: this violates visitor pattern. find a way to propagate the names and types up
    void visit(std::shared_ptr<Node::FunctionDefn> n) override {
        // process parameters
        n->paramList->accept(shared_from_this());
        std::vector<Variable> parameters = {};
        std::string signature = n->name;
        for (const auto& parameter : n->paramList->parameters) {
            parameters.push_back(Variable(parameter->name, parameter->type));
            signature += "@" + parameter->type;
        }

        // return type check
        if (!findType(n->returnType)) {
            std::cout << "Return type " << n->returnType
                      << " of function " << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }

        // name check 
        if (findFunction(signature)) {
            std::cout << "Function " << n->name << " is defined more than once with the same parameter types.\n"
                      << "(Mangled: " << signature << ")\n";
            exit(1);    
        }
        
        // make new scope
        variableTable.push_back(Scope(functionDefn, n->name));

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
        if (!findType(n->type)) {
            std::cout << "The given type " << n->type
                      << " of parameter " << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }
    }

    // visit assignment
    void visit(std::shared_ptr<Node::Assignment> n) override {

        // exists?
        if (!findVariable(n->lhs)) {
            std::cout << "In some scope, tried to assign a value to " << n->lhs << ", which has not been defined.\n";
            exit(1);
        }

        // find type
        std::string type;
        for (const auto& scope : variableTable) {
            for (const auto& variable : scope.variables) {
                if (n->lhs == variable.first)
                    type = variable.first;
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
        std::string functionName = variableTable.back().name.value();
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

    void visit(std::shared_ptr<Node::DotExpr> n) override {
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
        if (!findVariable(n->name)) {
            std::cout << "Variable " << n->name << " referenced in some scope, but it has not yet been defined.\n";
            exit(1);
        }

        // find type
        std::string type;
        for (const auto& scope : variableTable) {
            for (const auto& variable : scope.variables) {
                if (n->name == variable.second.name)
                    type = variable.second.type;
            }
        }

        // expression stack
        exprTypes.push_back(type);
    }

    // visit call
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
        if (!findFunction(signature)) {
            std::cout << "Could not find a matching definition for call with signature " << signature << ".\n";
            exit(1);
        }

        // push type to expression stack
        std::string type = functions.find(signature)->second.returnType;
        exprTypes.push_back(type);
    }

    // visit call args
    // @NOTE: violation of visitor pattern, never called
    void visit(std::shared_ptr<Node::ExprList> n) override {

    }

    // visit type definition
    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        // name check
        if (findType(n->name)) {
            std::cout << "Type " << n->name << " is already defined.\n";
            exit(1);
        }

        // enter scope
        variableTable.push_back(Scope(typeDefn, n->name));

        // @TODO: process members
        
        // process overloads
        for (const auto& overload : n->opOverloads) {

            
            overload->accept(shared_from_this());
        }

        // @TODO: process methods

        // end scope
        endScope();

        // add
        types.emplace(n->name, Type(n->name, {}, {}, {}));
    }

    // visit operator overload
    void visit(std::shared_ptr<Node::OperatorOverload> n) override {}

    void visit(std::shared_ptr<Node::ConditionalBlock> n) override {}

    void visit(std::shared_ptr<Node::WhileLoop> n) override {}


};

#endif
