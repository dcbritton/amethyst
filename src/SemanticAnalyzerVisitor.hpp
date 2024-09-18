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
        operatorDefn,

        whileLoop,
        conditionalBlock
    };

    struct Variable {
        std::string name;
        std::string type;

        Variable(const std::string& name, const std::string& type)
            : name(name), type(type) {}
    };

    // used for functions, methods, and operators
    struct Procedure {
        std::string name;
        std::string signature;
        std::string returnType;
        std::vector<Variable> parameters;

        Procedure() {}

        Procedure(const std::string& returnType)
            : returnType(returnType) {}

        Procedure(const std::string& name, const std::string& signature, const std::string& returnType, const std::vector<Variable>& parameters) 
            : name(name), signature(signature), returnType(returnType), parameters(parameters) {}
    };

    std::string formOpSignature(const std::string& op, const std::string& rhsType) {
        return op + "@" + rhsType;
    }

    struct Type {
        std::string name;
        std::unordered_map<std::string, Variable> members;
        std::unordered_map<std::string, Procedure> methods;
        std::unordered_map<std::string, Procedure> operators;

        Type(const std::string& name) 
            : name(name) {}

        Type(const std::string& name, const std::unordered_map<std::string, Variable>& members, const std::unordered_map<std::string, Procedure>& methods, const std::unordered_map<std::string, Procedure>& operators)
            : name(name), members(members), methods(methods), operators(operators) {}

        bool has(const std::string& signature) {
            return operators.find(signature) != operators.end();
        }

        Procedure getOperator(const std::string& signature) {
            return operators.find(signature)->second;
        }
    };

    struct Scope {
        ScopeType type = undefined;
        std::unordered_map<std::string, Variable> variables;
        std::string name; // name of function/type/method/operator for definition

        // constructor
        Scope(ScopeType type, const std::string& name)
            : type(type), name(name) {}
    };

    // stack used for tracking the types of subexpressions as an expression is evaluated
    std::vector<std::string> exprTypes;


    // SCOPE
    // stack containing variables at all scopes
    std::vector<Scope> scopeTable;

    void enterScope(ScopeType type, const std::string& name) {
        scopeTable.push_back(Scope(type, name));
    }
    
    void addToScope(Variable variable) {
        scopeTable.back().variables.emplace(variable.name, variable);
    }
    
    void endScope() {
        scopeTable.pop_back();
    }


    // GLOBAL
    bool globalExists(const std::string& name) {
        if (scopeTable.empty()) {
            std::cout << "Internal error. globalExists() called when scopeTable is empty.\n";
            exit(1);
        }

        if (scopeTable.front().type != global) {
            std::cout << "Internal error. globalExists() determined that the first scope is not global.\n";
            exit(1);
        }

        auto globals = scopeTable.front().variables;
        return globals.find(name) != globals.end();
    }

    // PRE: global must exist as verified by globalExists
    std::string getGlobalType(const std::string& name) {
        auto globals = scopeTable.front().variables;
        return globals.find(name)->second.type;
    }


    // FUNCTION / PROCEDURE
    // ptr to current function/method/opdefn (gets reset() at end of visits)
    std::unique_ptr<Procedure> currentProcedure =  nullptr;

    // contains available functions (functions are only declarable in global)
    std::unordered_map<std::string, Procedure> functions;

    bool inProcedure() {
        return currentProcedure != nullptr;
    }

    bool functionExists(const std::string& signature) {
        return functions.find(signature) != functions.end();
    }

    // PRE: currentProcedure != nullptr
    // used by functions, methods, and opdefns
    std::vector<Scope>::const_iterator getProcedureDefnScope() {
        auto it = scopeTable.begin();
        for (; it != scopeTable.end(); ++it) {
            if (it->type == functionDefn || it->type == methodDefn || it->type == operatorDefn) {
                return it;
            }
        }
        
        std::cout << "Internal error. getProcedureDefnScope() called when not in a function, method, or operator definition.\n";
        exit(1);
    }


    // TYPE
    // ptr to current Type if in type defn (gets std::moved to 'types' at end of visit(Node::TypeDefn))^
    std::unique_ptr<Type> currentType =  nullptr;

    // contains available types (types are only declarable in global)
    std::unordered_map<std::string, Type> types;

    bool inTypeDefn() {
        return currentType != nullptr;
    }

    bool typeExists(const std::string& name) {
        return types.find(name) != types.end();
    }

    // PRE: must be in type definition, as verified by inTypeDefn()
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


    // VARIABLES
    bool variableExists(const std::string& name) {

        // functions don't capture external variables
        if (inProcedure()) {
            // check parameters
            for (const auto& parameter : currentProcedure->parameters) {
                if (name == parameter.name) {
                    return true;
                }
            }
           // check only past the most recent procedure scope
            for (auto procedureScope = getProcedureDefnScope(); procedureScope != scopeTable.end(); ++procedureScope) {
                if (procedureScope->variables.find(name) != procedureScope->variables.end()) {
                    return true;
                }
            }
        }

        // unfound
        return false;
    }
    
    // PRE: variable must exist as verified by variableExists
    std::string getVariableType(const std::string& name) {
        // procedures don't capture external variables
        if (inProcedure()) {
            // check parameters
            for (const auto& parameter : currentProcedure->parameters) {
                if (name == parameter.name) {
                    return parameter.type;
                }
            }
           // check only past the most recent procedure scope
            for (auto procedureScope = getProcedureDefnScope(); procedureScope != scopeTable.end(); ++procedureScope) {
                if (procedureScope->variables.find(name) != procedureScope->variables.end()) {
                    return procedureScope->variables.find(name)->second.type;
                }
            }
        }

        // unfound
        std::cout << "Internal error. getVariableType() could not find variable " << name << ".\n";
        exit(1);
    }


    // VISIT METHODS
    void visit(std::shared_ptr<Node::Node> n) override {}

    // visit program
    void visit(std::shared_ptr<Node::Program> n) override {

        enterScope(global, "global");

        // @TODO: predefined variables, functions, types may go here with addToScope()
        types.emplace("_int32", Type("_int32", {}, {}, {
            std::make_pair<std::string, Procedure>("+@_int32", Procedure("_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("-", "_int32"), Procedure("-", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("!=", "_int32"), Procedure("!=", "_int32", "_int32")),
            // // std::make_pair<std::string, Procedure>(formOpSignature("!", "_int32"), Procedure("!", "_int32", "_int32")),
            // // std::make_pair<std::string, Procedure>(formOpSignature(".", "_int32"), Procedure(".", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("==", "_int32"), Procedure("==", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("*", "_int32"), Procedure("*", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("/", "_int32"), Procedure("/", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("%", "_int32"), Procedure("%", "_int32", "_int32")),
            // // std::make_pair<std::string, Procedure>(formOpSignature("&", "_int32"), Procedure("&", "_int32", "_int32")),
            // // std::make_pair<std::string, Procedure>(formOpSignature("|", "_int32"), Procedure("|", "_int32", "_int32")),
            // // std::make_pair<std::string, Procedure>(formOpSignature("||", "_int32"), Procedure("||", "_int32", "_int32")),
            // // std::make_pair<std::string, Procedure>(formOpSignature("&&", "_int32"), Procedure("&&", "_int32", "_int32")),
            // // std::make_pair<std::string, Procedure>(formOpSignature("^", "_int32"), Procedure("^", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("<<", "_int32"), Procedure("<<", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature(">>", "_int32"), Procedure(">>", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("<", "_int32"), Procedure("<", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("<=", "_int32"), Procedure("<=", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature(">", "_int32"), Procedure(">", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature(">=", "_int32"), Procedure(">=", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("and", "_int32"), Procedure("and", "_int32", "_int32")),
            // std::make_pair<std::string, Procedure>(formOpSignature("or", "_int32"), Procedure("or", "_int32", "_int32")),
            // // std::make_pair<std::string, Procedure>(formOpSignature("[]", "_int32"), Procedure("[]", "_int32", "_int32")),
        }));

        types.emplace("_float32", Type("_float32", {}, {}, {}));
        types.emplace("_string", Type("_string", {}, {}, {}));

        for (auto definition : n->definitions) {
            definition->accept(shared_from_this());
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

    // visit global definition
    void visit(std::shared_ptr<Node::GlobalDefn> n) override {
        // exists?
        if (globalExists(n->name)) {
            std::cout << "Global $" << n->name << " is defined more than once.\n";
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
            std::cout << "In the definition of global $" << n->name
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

    // visit global
    void visit(std::shared_ptr<Node::Global> n) override {
        // exists?
        if (!globalExists(n->name)) {
            std::cout << "Global $" << n->name << " referenced, but it has not yet been defined.\n";
            exit(1);
        }

        // find type
        std::string type = getGlobalType(n->name);

        // expression stack
        exprTypes.push_back(type);
    }

    // visit function body
    void visit(std::shared_ptr<Node::FunctionBody> n) override {
        for (auto stmt : n->statements) {
            stmt->accept(shared_from_this());
        }
    }

    // visit function definition
    void visit(std::shared_ptr<Node::FunctionDefn> n) override {

        // set currentProcedure to an empty Procedure
        currentProcedure = std::make_unique<Procedure>();

        // process parameters, this adds the parameters to currentProcedure
        n->paramList->accept(shared_from_this());

        // construct signature
        std::string signature = n->name;
        for (const auto& parameter : currentProcedure->parameters) {
            signature += "@" + parameter.type;
        }

        // check signature
        if (functionExists(signature)) {
            std::cout << "Function " << n->name << " is defined more than once with the same parameter types.\n"
                        << "(Mangled: " << signature << ")\n";
            exit(1);    
        }

        // return type check
        if (!typeExists(n->returnType)) {
            std::cout << "Return type " << n->returnType
                      << " of function " << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }

        // fully define currentProcedure with name, signature, and type (parameters already added when visited)
        currentProcedure->name = n->name;
        currentProcedure->signature = signature;
        currentProcedure->returnType = n->returnType;

        // make new scope
        enterScope(functionDefn, signature);

        // add the function (usable in own scope) - parameters are in the function map, not in scopeTable as variables
        functions.emplace(signature, *currentProcedure);

        // process functionBody
        n->functionBody->accept(shared_from_this());

        // end its own scope
        endScope();

        // clear currentProcedure, leaves nullptr
        currentProcedure.reset();
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
        
        // put in function/method/operator defn
        currentProcedure->parameters.push_back(Variable(n->name, n->type));
    }

    // visit assignment
    void visit(std::shared_ptr<Node::Assignment> n) override {
        std::string type;
        // split on sigil of lhs
        // global
        if (n->sigil == "$") {
            // exists?
            if (!globalExists(n->lhs)) {
                std::cout << "In some scope, tried to assign a value to global $" << n->lhs << ", which has not been defined.\n";
                exit(1);  
            }

            // find type
            type = getGlobalType(n->lhs);
        }

        // member
        else if (n->sigil == "@") {
            // not allowed outside of a type definition
            if (!inTypeDefn()) {
                std::cout << "Tried to assign to member @" << n->lhs << " when not in a type definition.\n";
                exit(1);
            }
            // exists?
            if (currentType->members.find(n->lhs) == currentType->members.end()) {
                std::cout << "In definition of type " << currentType->name
                          << ", tried to assign to member @" << n->lhs
                          << ", which has not yet been defined.\n";
                exit(1);
            }
            // find type
            type = currentType->members.find(n->lhs)->second.type;
        }

        // variable
        else if (n->sigil == "") {
            // exists?
            if (!variableExists(n->lhs)) {
                std::cout << "In some scope, tried to assign a value to " << n->lhs << ", which has not been defined.\n";
                exit(1);
            }
            // find type
            type = getVariableType(n->lhs);
        }

        // otherwise, error
        else {
            std::cout << "Internal error. Unrecognized sigil " << n->sigil << " in assignment.\n";
            exit(1);
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
    void visit(std::shared_ptr<Node::Return> n) override {

        // process expression
        n->expr->accept(shared_from_this());
        
        // match return type to expression type
        if (currentProcedure->returnType != exprTypes.back()) {
            std::cout << "In a return in the definition of " << currentProcedure->signature
                        << ", the return type " << currentProcedure->returnType
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
        exprTypes.push_back(type.getOperator(signature).returnType);
    }

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
        process(n);
    }

    void visit(std::shared_ptr<Node::Primary> n) override {}

    // visit array
    // @NOTE: violates visitor pattern, directly accesses expr list
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
            std::cout << "Variable " << n->name << " referenced, but it has not yet been defined.\n";
            exit(1);
        }

        // find type
        std::string type = getVariableType(n->name);

        // expression stack
        exprTypes.push_back(type);
    }

    // visit call
    // @NOTE: violates visitor pattern, directly accesses expr list
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
    // @NOTE: never called
    void visit(std::shared_ptr<Node::ExprList> n) override {

    }

    // visit type definition
    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        // name check
        if (typeExists(n->name)) {
            std::cout << "Type " << n->name << " is already defined.\n";
            exit(1);
        }

        // set currentTypeDefn to an empty Type
        currentType = std::make_unique<Type>(n->name);

        // enter scope
        enterScope(typeDefn, n->name);

        // process definitions
        for (const auto& definition : n->definitions) {
            definition->accept(shared_from_this());
        }

        // end scope
        endScope();

        // add
        types.emplace(n->name, std::move(*currentType));
        
        // reset currentType to nullptr. It currently points to garbage
        currentType.reset();
    }

    // visit operator defn
    void visit(std::shared_ptr<Node::OperatorDefn> n) override {

       // set currentProcedure to an empty Procedure
        currentProcedure = std::make_unique<Procedure>();

        // process parameters, this adds the parameters to currentProcedure
        n->parameter->accept(shared_from_this());

        // construct signature
        std::string signature = n->op;
        for (const auto& parameter : currentProcedure->parameters) {
            signature += "@" + parameter.type;
        }

        // signature check
        if (currentType->operators.find(signature) != currentType->operators.end()) {
            std::cout << "In definition of type " << currentType->name << " operator " << n->op << " is defined more than once with the same parameter types.\n"
                        << "(Mangled: " << signature << ")\n";
            exit(1);    
        }

        // return type check
        if (!typeExists(n->returnType)) {
            std::cout << "In the definition of type " << currentType->name << ", the return type " << n->returnType
                      << " of operator " << n->op
                      << " has not yet been defined.\n";
            exit(1);
        }

        // fully define currentProcedure with name, signature, and type (parameters already added when visited)
        currentProcedure->returnType = n->returnType;
        currentProcedure->name = n->op;
        currentProcedure->signature = signature;
        
        // make new scope
        enterScope(operatorDefn, signature);

        // add to type's operators (allows it to be used in itself)
        currentType->operators.emplace(signature, *currentProcedure);

        // process functionBody
        n->stmts->accept(shared_from_this());

        // end its own scope
        endScope();

        // clear currentProcedure, leaves nullptr
        currentProcedure.reset();
    }

    // visit member definition
    void visit(std::shared_ptr<Node::MemberDefn> n) override {
        // exists?
        if (currentType->members.find(n->name) != currentType->members.end()) {
            std::cout << "In the definition of type " << currentType->name << ", member @" << n->name << " is defined more than once.\n";
            exit(1);
        }

        // does n->type exist?
        if (!typeExists(n->type)) {
            std::cout << "The given type " << n->type
                      << " of member @" << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }
        
        // match lhs and rhs type
        n->expression->accept(shared_from_this());
        if (exprTypes.back() != n->type) {
            std::cout << "In the definition of member @" << n->name
                      << ", the given type " << n->type
                      << " does not match the expression type " << exprTypes.back()
                      << ".\n";
            exit(1);
        }

        // empty the stack
        exprTypes.pop_back();

        // add to currentType
        currentType->members.emplace(n->name, Variable(n->name, n->type));

        // add to scope
        // @TODO: probably redundant. members are checked for existence in currentType->members
        addToScope(Variable(n->name, n->type));
    }

    // visit method definition
    void visit(std::shared_ptr<Node::MethodDefn> n) override {
        // set currentProcedure to an empty Procedure
        currentProcedure = std::make_unique<Procedure>();

        // process parameters, this adds the parameters to currentProcedure
        n->paramList->accept(shared_from_this());

        // construct signature
        std::string signature = n->name;
        for (const auto& parameter : currentProcedure->parameters) {
            signature += "@" + parameter.type;
        }

        // signature check
        if (currentType->methods.find(signature) != currentType->methods.end()) {
            std::cout << "In definition of type " << currentType->name << " method @" << n->name << " is defined more than once with the same parameter types.\n"
                        << "(Mangled: " << signature << ")\n";
            exit(1);    
        }

        // return type check
        if (!typeExists(n->returnType)) {
            std::cout << "Return type " << n->returnType
                      << " of method @" << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }

        // fully define currentProcedure with name, signature, and type (parameters already added when visited)
        currentProcedure->name = n->name;
        currentProcedure->signature = signature;
        currentProcedure->returnType = n->returnType;

        // make new scope
        enterScope(methodDefn, signature);

        // add to type's methods (allows it to be used in itself)
        currentType->methods.emplace(signature, *currentProcedure);

        // process functionBody
        n->functionBody->accept(shared_from_this());

        // end its own scope
        endScope();

        // clear currentProcedure, leaves nullptr
        currentProcedure.reset();
    }

    // visit member
    void visit(std::shared_ptr<Node::Member> n) override {
        // only usable in type definitions
        if (currentType == nullptr) {
            std::cout << "Tried to use member @" << n->name << " when not in a type definition.\n";
            exit(1);
        }

        // exists?
        if (currentType->members.find(n->name) == currentType->members.end()) {
            std::cout << "Tried to use member @" << n->name << " in the definition of type " << currentType->name << ", but it has not yet been defined.\n";
            exit(1);
        }

        // find type
        std::string type = currentType->members.find(n->name)->second.type;

        // expression stack
        exprTypes.push_back(type);
    }

    // visit method call
    // @NOTE: violates visitor pattern, directly accesses expr list
    void visit(std::shared_ptr<Node::MethodCall> n) override {
        // only usable in type definitions
        if (currentType == nullptr) {
            std::cout << "Tried to use member @" << n->name << " when not in a type definition.\n";
            exit(1);
        }

        // determine argument expression types & deal with expr stack
        std::vector<std::string> types = {};
        for (auto& expr : n->args->exprs) {
            expr->accept(shared_from_this());
            types.push_back(exprTypes.back());
            exprTypes.pop_back();
        }

        // create signature
        std::string signature = n->name;
        for (const std::string& type : types) {
            signature += "@" + type;
        }

        // method exists?
        if (currentType->methods.find(signature) == currentType->methods.end()) {
            std::cout << "In definition of type " << currentType->name 
                      << ", could not find a matching definition for method call with signature " << signature << ".\n";
            exit(1);
        }

        // push type to expression stack
        std::string type = currentType->methods.find(signature)->second.returnType;
        exprTypes.push_back(type);

    }

    void visit(std::shared_ptr<Node::ConditionalBlock> n) override {}

    void visit(std::shared_ptr<Node::WhileLoop> n) override {}


};

#endif
