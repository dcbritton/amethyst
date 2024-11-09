// SemanticAnalyzerVisitor.hpp

#ifndef SEMANTICANALYZERVISITOR
#define SEMANTICANALYZERVISITOR

#include <unordered_map>
#include "AnalysisHelper.hpp"
#include "Visitor.hpp"

struct SemanticAnalyzerVisitor : Visitor {

    // stack used for tracking the types of subexpressions as an expression is evaluated
    std::vector<std::string> exprTypes;


    // SCOPE
    struct Scope {
        ScopeType type = undefined;
        std::unordered_map<std::string, Variable> variables;
        std::string name; // name of function/type/method/operator for definition

        // constructor
        Scope(ScopeType type)
            : type(type) {}
    };


    // stack containing variables at all scopes
    std::vector<Scope> scopeTable;

    void enterScope(ScopeType type) {
        scopeTable.push_back(Scope(type));
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
            if (it->type == functionDefn || it->type == methodDefn || it->type == operatorDefn || it->type == constructorDefn) {
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
        // remove trailing *'s, b/c if a type exists, its pointers do, too
        std::string nonPtr = name;
        if (name.back() == '*') {
            nonPtr = std::string(name.begin(), std::find(name.begin(), name.end(), '*'));
        }

        // allow a type to be used in its own definition
        if (inTypeDefn() && currentType->name == nonPtr) {
            return true;
        }
        // otherwise, check types map
        return types.find(nonPtr) != types.end();
    }

    // when evaluating a dot operation, points to the type in the types map of the LHS
    // @NOTE: set only in visit(Node::AccessExpr) and must be returned to nullptr immediately after RHS is visited
    // @NOTE: points to memory in a std::unordered map. do not interact with types map when currentDotLhsType is not nullptr
    Type currentDotLhsType;

    
    // LOOPS
    bool inLoop() {
        for (const auto& scope : scopeTable) {
            if (scope.type == whileLoop) {
                return true;
            }
        }

        return false;
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

        enterScope(global);

        // @TODO: predefined variables, functions, types may go here with addToScope()
        types.emplace("int", Type("int", {}, {}, {
            // example signature: int.op.plus$int,
            std::make_pair<std::string, Procedure>(formOpSignature("int", "==", "int"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "!=", "int"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "<", "int"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "<=", "int"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", ">", "int"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", ">=", "int"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "*", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "/", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "%", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "<<", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", ">>", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "+", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "-", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "*", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "/", "int"), Procedure("int")),
            std::make_pair<std::string, Procedure>(formOpSignature("int", "%", "int"), Procedure("int")),

            // std::make_pair<std::string, Procedure>(formOpSignature("int", "and", "int"), Procedure("bool")),
            // std::make_pair<std::string, Procedure>(formOpSignature("int", "or", "int"), Procedure("bool")),
            // std::make_pair<std::string, Procedure>(formOpSignature("&", "int"), Procedure("&", "int", "int")),
            // std::make_pair<std::string, Procedure>(formOpSignature("|", "int"), Procedure("|", "int", "int")),
            // std::make_pair<std::string, Procedure>(formOpSignature("^", "int"), Procedure("^", "int", "int")),
            // std::make_pair<std::string, Procedure>(formOpSignature("!", "int"), Procedure("!", "int", "int")),
            // std::make_pair<std::string, Procedure>(formOpSignature(".", "int"), Procedure(".", "int", "int")),
            // std::make_pair<std::string, Procedure>(formOpSignature("[]", "int"), Procedure("[]", "int", "int")),
        }, {}));

        types.emplace("bool", Type("bool", {}, {}, {
            std::make_pair<std::string, Procedure>(formOpSignature("bool", "and", "bool"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("bool", "or", "bool"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("bool", "==", "bool"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("bool", "!=", "bool"), Procedure("bool")),
        }, {}));

        types.emplace("float", Type("float", {}, {}, {
            std::make_pair<std::string, Procedure>(formOpSignature("float", "==", "float"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", "!=", "float"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", "<", "float"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", ">", "float"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", "<=", "float"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", ">=", "float"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", "+", "float"), Procedure("float")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", "-", "float"), Procedure("float")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", "*", "float"), Procedure("float")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", "/", "float"), Procedure("float")),
            std::make_pair<std::string, Procedure>(formOpSignature("float", "%", "float"), Procedure("float")),
        }, {}));

        types.emplace("char", Type("char", {}, {}, {
            std::make_pair<std::string, Procedure>(formOpSignature("char", "==", "char"), Procedure("bool")),
            std::make_pair<std::string, Procedure>(formOpSignature("char", "!=", "char"), Procedure("bool")),
        }, {}));

        types.emplace("nil", Type("nil", {}, {}, {}, {}));

        // visit statement
        for (auto definition : n->definitions) {
            definition->accept(shared_from_this());
        }

        n->types = std::make_unique<std::unordered_map<std::string, Type>>(std::move(types));
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

    // visit global declaration
    void visit(std::shared_ptr<Node::GlobalDecl> n) override {
        // exists?
        if (globalExists(n->name)) {
            std::cout << "Global $" << n->name << " is defined more than once.\n";
            exit(1);
        }

        // does n->type exist?
        if (!typeExists(n->type)) {
            std::cout << "The given type " << n->type
                      << " of global $" << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }
        
        // add
        addToScope(Variable(n->name, n->type));
    }

    // visit global
    void visit(std::shared_ptr<Node::Global> n) override {
        // exists?
        if (!globalExists(n->name)) {
            std::cout << "Global $" << n->name << " referenced, but it has not yet been declared.\n";
            exit(1);
        }

        // find type
        std::string type = getGlobalType(n->name);

        // expression stack
        exprTypes.push_back(type);

        // attach info for code gen
        n->type = type;
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
            signature += "$" + parameter.type;
        }
        manglePointers(signature);

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
        enterScope(functionDefn);

        // add the function (usable in own scope) - parameters are in the function map, not in scopeTable as variables
        functions.emplace(signature, *currentProcedure);

        // process functionBody
        n->functionBody->accept(shared_from_this());

        // end its own scope
        endScope();

        // move the info the AST, to be used by GeneratorVisitor
        // leaves nullptr
        n->info = std::move(currentProcedure);
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

        // no parameters with the same name
        for (const auto& parameter : currentProcedure->parameters) {
            if (parameter.name == n->name) {
                std::cout << "In a procedure definition, the parameter name \"" << parameter.name << "\" is used more than once.\n";
                exit(1);
            }
        }
        
        // put in function/method/operator defn
        currentProcedure->parameters.push_back(Variable(n->name, n->type));
    }

    // visit assignment
    void visit(std::shared_ptr<Node::Assignment> n) override {
        // process lhs and rhs
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get lhs and rhs types of expr type stack
        std::string rhsType = exprTypes.back();
        exprTypes.pop_back();
        std::string lhsType = exprTypes.back();
        exprTypes.pop_back();
        
        // match lhs and rhs type
        if (lhsType != rhsType) {
            std::cout << "Error in assignment. LHS type " << lhsType << " did not match RHS type " << rhsType << ".\n";
            exit(1);
        }
    }

    // visit return
    void visit(std::shared_ptr<Node::Return> n) override {
        // expression present (for non-void procedures)
        if (n->expr) {
            // process expression
            n->expr->accept(shared_from_this());
            
            // match return type to expression type
            if (currentProcedure->returnType != exprTypes.back()) {
                std::cout << "In a return in the definition of " << currentProcedure->signature
                            << ", the return type " << currentProcedure->returnType
                            << " does not match the expression type " << exprTypes.back() << ".\n";
                exit(1);
            }

            // empty the stack
            exprTypes.pop_back();
        }
        // void return
        else {
            if (currentProcedure->returnType != "nil") {
                std::cout << "Nil return in defintion non-nil procedure " << currentProcedure->signature << ".\n";
                exit(1);
            }
        }
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

        // @TODO: handle pointers more gracefully. currently, no operators may be used (other than perhaps [])
        if (lhsType.back() == '*') {
            std::cout << "May not use operator " << n->op << " on a pointer type.\n";
            exit(1);
        }

        // check the type and see if it has the operator
        std::string signature = formOpSignature(lhsType, n->op, rhsType);
        // in own definition, check its own operators
        if (inTypeDefn() && currentType->name == lhsType) {
            // check for existence of operator
            if (!currentType->has(signature)) {
                std::cout << "Type " << currentType->name << " has no defined op " << signature << ".\n";
                exit(1);
            }
            exprTypes.push_back(currentType->getOperator(signature).returnType);
        }
        // otherwise, check types map
        else {
            Type& type = types.find(lhsType)->second;
            // check for existence of operator
            if (!type.has(signature)) {
                std::cout << "Type " << type.name << " has no defined op " << signature << ".\n";
                exit(1);
            }
            exprTypes.push_back(type.getOperator(signature).returnType);
        }
    }

    void visit(std::shared_ptr<Node::LogicalExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::EqualityExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::RelationalExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::AdditiveExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::MultiplicativeExpr> n) override {
        process(n);
    }

    void visit(std::shared_ptr<Node::AccessExpr> n) override {
        // process lhs & get type
        n->LHS->accept(shared_from_this());
        std::string lhsType = exprTypes.back();
        exprTypes.pop_back();

        // subscript
        if (n->op == "[]") {
            // is ptr?
            if (lhsType.back() == '*') {
                // get rhs type
                n->RHS->accept(shared_from_this());
                std::string rhsType = exprTypes.back();
                exprTypes.pop_back();
                // verify that rhs type is an int
                if (rhsType != "int") {
                    std::cout << "Error. Tried to subscript access a pointer with something other than an int!\n";
                    exit(1);
                }

                // return type is a dereferenced ptr, i.e. the type minus one *
                exprTypes.push_back(std::string(lhsType.begin(), lhsType.end()-1));
            }
            // otherwise, process as normal operator
            else {
                // process(n);

                // @TODO: usage of op[] currently disabled
                std::cout << "Usage of op[] on non-pointer types is currently prohibited.\n";
                exit(1);
            }
        }

        // dot
        else if (n->op == ".") {
            // @TODO: replace all ".back() == '*'" with an isPointer(const std::string&) method 
            // is ptr?
            if (lhsType.back() == '*') {
                std::cout << "Error. Tried to use dot operator on a pointer.\n";
                exit(1);
            }

            // existence of rhs verified during this call in visit(Node::DotRhsMember/DotRhsMethodCall)
            currentDotLhsType = types[lhsType];
            n->RHS->accept(shared_from_this());

            // get the rhs' type
            std::string rhsType = exprTypes.back();
            exprTypes.pop_back();

            // push the RHS' type to type stack
            exprTypes.push_back(rhsType);
        }
    }

    // visit dot rhs member
    void visit(std::shared_ptr<Node::DotRhsMember> n) override {

        std::string type;

        // if so, check the LHS type for the member
        if (currentDotLhsType.members.find(n->name) == currentDotLhsType.members.end()) {
            std::cout << "Error in dot operation. Type " << currentDotLhsType.name << " has no member " << n->name << ".\n";
            exit(1);
        }

        // get type of member
        type = currentDotLhsType.members.find(n->name)->second.type;

        // expression stack
        exprTypes.push_back(type);

        // set node's type to confirmed type, to be used in code gen
        n->type = type;
    }

    // visit dot rhs method call
    void visit(std::shared_ptr<Node::DotRhsMethodCall> n) override {

        // determine argument expression types & deal with expr stack
        std::vector<std::string> types = {};
        for (auto& expr : n->args->exprs) {
            expr->accept(shared_from_this());
            types.push_back(exprTypes.back());
            exprTypes.pop_back();
        }

        // create signature
        std::string signature = currentDotLhsType.name + "." + n->name;
        for (const std::string& type : types) {
            signature += "$" + type;
        }
        manglePointers(signature);

        // method exists? check lhs type methods
        if (currentDotLhsType.methods.find(signature) == currentDotLhsType.methods.end()) {
            std::cout << "In a DotRhsMethodCall, could not find a matching definition for method with signature " << signature << ".\n";
            exit(1);
        }

        // push type to expression stack
        std::string type = currentDotLhsType.methods.find(signature)->second.returnType;
        exprTypes.push_back(type);

        // attach info for code generator
        n->type = type;
        n->numArgs = types.size();
        n->signature = signature;
    }

    void visit(std::shared_ptr<Node::Primary> n) override {}

    // visit new expression
    void visit(std::shared_ptr<Node::NewExpr> n) override {
        // type exists?
        
        if (!typeExists(n->type)) {
            std::cout << "Undefined type " << n->type << " mentioned in new expression.\n";
            exit(1);
        }

        // process args
        // @NOTE: violates visitor pattern, directly accesses expr list
        std::vector<std::string> typeNames;
        for (auto& expr : n->args->exprs) {
            expr->accept(shared_from_this());
            typeNames.push_back(exprTypes.back());
            exprTypes.pop_back();
        }

        // create constructor signature
        std::string signature = n->type + ".new";
        for (const std::string& typeName : typeNames) {
            signature += "$" + typeName;
        }
        manglePointers(signature);

        // signature exists?
        // in own definition, check currentType
        if (inTypeDefn() && currentType->name == n->type) {
            if (currentType->constructors.find(signature) == currentType->constructors.end()) {
                std::cout << "In new_expr, could not find constructor for " << n->type << " with signature " << signature << ".\n";
                exit(1);
            }
        }
        // otherwise, check types
        else if (types[n->type].constructors.find(signature) == types[n->type].constructors.end()) {
            std::cout << "In new_expr, could not find constructor for " << n->type << " with signature " << signature << ".\n";
            exit(1);
        }

        // push type to expr types stack
        exprTypes.push_back(n->type);

        // attach info for code generator
        n->signature = signature;
    }

    // visit stack expression
    void visit(std::shared_ptr<Node::StackExpr> n) override {
        // type exists?
        if (!typeExists(n->type)) {
            std::cout << "Undefined type " << n->type << " mentioned in stack expression.\n";
            exit(1);
        }

        // push a ptr to the type to the expr type stack
        exprTypes.push_back(n->type + "*");
    }

    // visit heap expression
    void visit(std::shared_ptr<Node::HeapExpr> n) override {
        // type exists?
        if (!typeExists(n->type)) {
            std::cout << "Undefined type " << n->type << " mentioned in heap expression.\n";
            exit(1);
        }
        
        // expression must evaluate to int
        n->expr->accept(shared_from_this());
        auto exprInfo = exprTypes.back();
        exprTypes.pop_back();

        if (exprInfo != "int") {
            std::cout << "Expression in heap expression did not evaluate to int.\n";
            exit(1);
        }

        // push a ptr to the type to the expr type stack
        exprTypes.push_back(n->type + "*");
    }

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
        // @TODO: allow empty arrays, current type "empty array" is, of course, impossible to match
        if (types.empty()) {
            exprTypes.push_back("empty array");
        }
        else {
            exprTypes.push_back(types.front() + "*");
        }
    }

    // visit int literal
    void visit(std::shared_ptr<Node::IntLiteral> n) override {
        // expression stack
        exprTypes.push_back("int");
    }

    // visit float literal
    void visit(std::shared_ptr<Node::FloatLiteral> n) override {
        // expression stack
        exprTypes.push_back("float");
    }

    // visit string literal
    void visit(std::shared_ptr<Node::StringLiteral> n) override {
        // expression stack
        exprTypes.push_back("char*");
    }

    // visit bool literal
    void visit(std::shared_ptr<Node::BoolLiteral> n) override {
        // expression stack
        exprTypes.push_back("bool");
    }
    
    // visit char literal
    void visit(std::shared_ptr<Node::CharLiteral> n) override {
        // expression stack
        exprTypes.push_back("char");
    }

    // visit variable
    void visit(std::shared_ptr<Node::Variable> n) override {
        std::string type;

        // exists?
        if (!variableExists(n->name)) {
            std::cout << "Variable " << n->name << " referenced, but it has not yet been defined.\n";
            exit(1);
        }

        // find type
        type = getVariableType(n->name);

        // expression stack
        exprTypes.push_back(type);

        // set node's type to confirmed type, to be used in code gen
        n->type = type;
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
            signature += "$" + type;
        }
        manglePointers(signature);

        // function exists?
        if (!functionExists(signature)) {
            std::cout << "Could not find a matching definition for call with signature " << signature << ".\n";
            exit(1);
        }

        // return type of free function
        std::string type = functions.find(signature)->second.returnType;

        // push return type to expression stack
        exprTypes.push_back(type);

        // attach info for code generator
        n->type = type;
        n->numArgs = types.size();
        n->signature = signature;
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
        enterScope(typeDefn);

        // build full type
        TypeDefinitionScanner tds;
        currentType = std::move(tds.visit(n));
        // add to types map, can be used in self!
        types.emplace(n->name, *currentType);

        // process definitions
        for (auto member : n->members) {
            member->accept(shared_from_this());
        }
        for (auto constructor : n->constructors) {
            constructor->accept(shared_from_this());
        }
        for (auto method : n->methods) {
            method->accept(shared_from_this());
        }
        for (auto op : n->operators) {
            op->accept(shared_from_this());
        }

        // end scope
        endScope();
        
        // reset currentType to nullptr. It currently points to garbage
        currentType.reset();
    }

    // visit operator defn
    void visit(std::shared_ptr<Node::OperatorDefn> n) override {

        // return type check
        if (!typeExists(n->returnType)) {
            std::cout << "In the definition of type " << currentType->name << ", the return type " << n->returnType
                      << " of operator " << n->op
                      << " has not yet been defined.\n";
            exit(1);
        }

        // get signature by creating a temporary Procedure
        currentProcedure = std::make_unique<Procedure>();
        n->parameter->accept(shared_from_this());
        std::string signature = formOpSignature(currentType->name, n->op, n->parameter->type);

        currentProcedure.reset();
            
        // copy fully defined operator from currentType
        currentProcedure = std::make_unique<Procedure>(currentType->operators[signature]);

        // process body
        enterScope(operatorDefn);
        n->stmts->accept(shared_from_this());
        endScope();

        // move the info the AST, to be used by GeneratorVisitor
        // leaves nullptr
        n->info = std::move(currentProcedure);
    }

    // visit member declaration
    void visit(std::shared_ptr<Node::MemberDecl> n) override {
        // does n->type exist?
        if (!typeExists(n->type)) {
            std::cout << "The given type " << n->type << " of member @" << n->name << " has not yet been defined.\n";
            exit(1);
        }
    }

    // visit method definition
    void visit(std::shared_ptr<Node::MethodDefn> n) override {

        // return type check
        if (!typeExists(n->returnType)) {
            std::cout << "Return type " << n->returnType
                      << " of method @" << n->name
                      << " has not yet been defined.\n";
            exit(1);
        }

        // get signature by creating a temporary Procedure
        currentProcedure = std::make_unique<Procedure>();
        n->paramList->accept(shared_from_this());
        std::string signature = currentType->name + "." + n->name;
        for (const auto& parameter : currentProcedure->parameters) {
            signature += "$" + parameter.type;
        }
        manglePointers(signature);

        currentProcedure.reset();

        // copy fully defined method from currentType
        currentProcedure = std::make_unique<Procedure>(currentType->methods[signature]);

        // process body
        enterScope(methodDefn);
        n->functionBody->accept(shared_from_this());
        endScope();

        // move the info the AST, to be used by GeneratorVisitor
        // leaves nullptr
        n->info = std::move(currentProcedure);
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

        // set node's type to confirmed type, to be used in code gen
        n->type = type;
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
        std::string signature = currentType->name + "." + n->name;
        for (const std::string& type : types) {
            signature += "$" + type;
        }
        manglePointers(signature);

        // method exists?
        if (currentType->methods.find(signature) == currentType->methods.end()) {
            std::cout << "In definition of type " << currentType->name 
                      << ", could not find a matching definition for method call with signature " << signature << ".\n";
            exit(1);
        }

        // push type to expression stack
        std::string type = currentType->methods.find(signature)->second.returnType;
        exprTypes.push_back(type);

        // attach info for code generator
        n->type = type;
        n->numArgs = types.size();
        n->signature = signature;
    }

    // visit unheap
    void visit(std::shared_ptr<Node::Unheap> n) override {

        // visit expr, guaranteed accessexpr by parser
        n->expr->accept(shared_from_this());

        // can't free a non-pointer
        if (exprTypes.back().back() != '*') {
            std::cout << "Used unheap on a non-pointer type.\n";
            exit(1);
        }

        exprTypes.pop_back();
    }

    // visit constructor definition
    void visit(std::shared_ptr<Node::ConstructorDefn> n) override {
        // set currentProcedure to an empty Procedure
        currentProcedure = std::make_unique<Procedure>();

        // process parameters, this adds the parameters to currentProcedure
        n->parameters->accept(shared_from_this());

        // get signature by creating a temporary Procedure
        currentProcedure = std::make_unique<Procedure>();
        n->parameters->accept(shared_from_this());
        std::string signature = currentType->name + ".new";
        for (const auto& parameter : currentProcedure->parameters) {
            signature += "$" + parameter.type;
        }
        manglePointers(signature);

        currentProcedure.reset();

        // copy fully defined constructor from currentType
        currentProcedure = std::make_unique<Procedure>(currentType->constructors[signature]);

        // process body
        enterScope(constructorDefn);
        n->body->accept(shared_from_this());
        endScope();

        // move the info the AST, to be used by GeneratorVisitor
        // leaves nullptr
        n->info = std::move(currentProcedure);
    }

    void visit(std::shared_ptr<Node::Conditional> n) override {
        // make sure expressions evaluate to bool
        n->ifExpr->accept(shared_from_this());
        if (exprTypes.back() != "bool") {
            std::cout << "An if condition did not evaluate to bool.\n";
            exit(1);
        }
        exprTypes.pop_back();

        for (const auto& [expr, body]: n->elsifs) {
            expr->accept(shared_from_this());
            if (exprTypes.back() != "bool") {
                std::cout << "An elsif condition did not evaluate to bool.\n";
                exit(1);
            }
            exprTypes.pop_back();
        }

        // analyze all bodies
        n->ifStmts->accept(shared_from_this());
        for (const auto& [expr, body] : n->elsifs) {
            body->accept(shared_from_this());
        }
        if (n->elseStmts) {
            n->elseStmts->accept(shared_from_this());
        }
    }

    void visit(std::shared_ptr<Node::WhileLoop> n) override {
        // condition must evaluate to bool
        n->expr->accept(shared_from_this());
        if (exprTypes.back() != "bool") {
            std::cout << "While condition does not evaluate to a bool.\n";
            exit(1);
        }

        // process body
        enterScope(whileLoop);
        n->stmts->accept(shared_from_this());
        endScope();
    }

    // break
    void visit(std::shared_ptr<Node::Break> n) override {
        // must be in a loop
        if (!inLoop()) {
           std::cout << "Tried to use a break statement when not in a loop.\n";
           exit(1);
        }
    }

    // continue
    void visit(std::shared_ptr<Node::Continue> n) override {
        // must be in a loop
        if (!inLoop()) {
           std::cout << "Tried to use a continue statement when not in a loop.\n";
           exit(1);
        }
    }

    // redo
    void visit(std::shared_ptr<Node::Redo> n) override {
        // must be in a loop
        if (!inLoop()) {
           std::cout << "Tried to use a redo statement when not in a loop.\n";
           exit(1);
        }
    }
};

#endif
