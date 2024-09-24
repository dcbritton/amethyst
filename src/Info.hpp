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

    Type(Type*& other) 
        : name(other->name), members(other->members), methods(other->methods), operators(other->operators), constructors(other->constructors) {}

    bool has(const std::string& signature) {
        return operators.find(signature) != operators.end();
    }

    Procedure getOperator(const std::string& signature) {
        return operators.find(signature)->second;
    }
};

std::string formOpSignature(const std::string& op, const std::string& rhsType) {
    return op + "@" + rhsType;
}

struct Scope {
    ScopeType type = undefined;
    std::unordered_map<std::string, Variable> variables;
    std::string name; // name of function/type/method/operator for definition

    // constructor
    Scope(ScopeType type, const std::string& name)
        : type(type), name(name) {}
};

#endif
