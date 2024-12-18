// Info.hpp

// contains defintions of structs built during semantic analysis
// these structs are attached to the AST and used by the code generator

#ifndef INFO
#define INFO

#include <string>
#include <unordered_map>

enum ScopeType {
    undefined,
    global,
    functionDefn,

    typeDefn,
    methodDefn,
    operatorDefn,
    constructorDefn,

    whileLoop,
    conditionalBlock
};

struct Variable {
    std::string name;
    std::string type;

    // if a member, offset for getelement pointer
    uint32_t offset;

    Variable(const std::string& name, const std::string& type)
        : name(name), type(type) {}

    Variable(const std::string& name, const std::string& type, uint32_t offset)
        : name(name), type(type), offset(offset) {}
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

struct Type {
    std::string name;
    std::unordered_map<std::string, Variable> members;
    std::unordered_map<std::string, Procedure> methods;
    std::unordered_map<std::string, Procedure> operators;
    std::unordered_map<std::string, Procedure> constructors;

    Type() {}

    Type(const std::string& name) 
        : name(name) {}

    Type(const std::string& name, const std::unordered_map<std::string, Variable>& members, const std::unordered_map<std::string, Procedure>& methods, const std::unordered_map<std::string, Procedure>& operators, const std::unordered_map<std::string, Procedure>& constructors)
        : name(name), members(members), methods(methods), operators(operators), constructors(constructors) {}

    bool has(const std::string& signature) {
        return operators.find(signature) != operators.end();
    }

    Procedure getOperator(const std::string& signature) {
        return operators.find(signature)->second;
    }
};

// replace *'s with .'s for name mangling
void manglePointers(std::string& signature) {
    for (auto& character : signature) {
        if (character == '*') {
            character = '.';
        }
    }
}

// 
std::string formOpSignature(const std::string& type, const std::string& op, const std::string& rhsType) {
    std::string longformOpName;
    if (op == "and") {
        longformOpName = "and";
    }
    else if (op == "or") {
        longformOpName = "or";
    }
    else if (op == "==") {
        longformOpName = "equals";
    }
    else if (op == "!=") {
        longformOpName = "notEquals";
    }
    else if (op == "<") {
        longformOpName = "lessThan";
    }
    else if (op == ">") {
        longformOpName = "greaterThan";
    }
    else if (op == "<=") {
        longformOpName = "lessThanEquals";
    }
    else if (op == ">=") {
        longformOpName = "greaterThanEquals";
    }
    else if (op == "<<") {
        longformOpName = "leftShift";
    }
    else if (op == ">>") {
        longformOpName = "rightShift";
    }
    else if (op == "+") {
        longformOpName = "plus";
    }
    else if (op == "-") {
        longformOpName = "minus";
    }
    else if (op == "*") {
        longformOpName = "multiply";
    }
    else if (op == "/") {
        longformOpName = "divide";
    }
    else if (op == "%") {
        longformOpName = "modulus";
    }
    else if (op == "[]") {
        std::cout << "Tried to use op[] on user-defined type: " << type <<".\nCaught by formOpSignature().\n";
        exit(1);
    }
    else {
        std::cout << "Internal error. Could not form op signature, because of unrecognized operator: " << op << ".\nThis should have been prevented by the parser.\n";
        exit(1);
    }
    
    std::string signature = type + ".op." + longformOpName + "$" + rhsType;
    manglePointers(signature);
    
    return signature;
}

#endif
