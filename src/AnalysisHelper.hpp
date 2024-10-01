// AnalysisHelper.hpp

#ifndef ANALYSISHELPER
#define ANALYSISHELPER

#include "Info.hpp"

struct TypeDefinitionScanner {

    // 
    Type currentType;

    //
    Procedure currentProcedure;

    // collect data for a type
    std::unique_ptr<Type> visit(std::shared_ptr<Node::TypeDefn> n)  {

        currentType.name = n->name;

        for (auto member : n->members) {
            visit(member);
        }
        for (auto constructor : n->constructors) {
            visit(constructor);
        }
        for (auto method : n->methods) {
            visit(method);
        }
        for (auto op : n->operators) {
            visit(op);
        }

        return std::make_unique<Type>(currentType);
    }

    void visit(std::shared_ptr<Node::MemberDecl> n)  {
        // exists already?
        if (currentType.members.find(n->name) != currentType.members.end()) {
            std::cout << "In the definition of type " << currentType.name << ", member @" << n->name << " is defined more than once.\n";
            exit(1);
        }

        // add to currentType
        currentType.members.emplace(n->name, Variable(n->name, n->type));
    }

    // visit parameter list
    void visit(std::shared_ptr<Node::ParamList> n)  {
        for (const auto& parameter : n->parameters) {
            visit(parameter);
        }
    }

    // visit parameter
    void visit(std::shared_ptr<Node::Parameter> n)  {
        // no parameters with the same name
        for (const auto& parameter : currentProcedure.parameters) {
            if (parameter.name == n->name) {
                std::cout << "In definition of type " << currentType.name << ", a procedure uses the same parameter name \"" << parameter.name << "\" more than once.\n";
                exit(1);
            }
        }
        // put in function/method/operator defn
        currentProcedure.parameters.push_back(Variable(n->name, n->type));
    }

    void visit(std::shared_ptr<Node::ConstructorDefn> n)  {
        // set currentProcedure to an empty Procedure
        currentProcedure = Procedure();

        // process parameters, this adds the parameters to currentProcedure
        visit(n->parameters);

        // construct signature
        std::string signature = "new";
        for (const auto& parameter : currentProcedure.parameters) {
            signature += "$" + parameter.type;
        }

        // signature check
        if (currentType.constructors.find(signature) != currentType.constructors.end()) {
            std::cout << "In definition of type " << currentType.name << " the constructor with signature " << signature << " is defined more than once.\n";
            exit(1);    
        }

        // fully define currentProcedure with name, signature, and type (parameters already added when visited)
        currentProcedure.name = "new";
        currentProcedure.signature = signature;
        currentProcedure.returnType = "nil";

        // add to type's constructors
        currentType.constructors.emplace(signature, currentProcedure);
    }

    // visit method definition
    void visit(std::shared_ptr<Node::MethodDefn> n)  {
        // set currentProcedure to an empty Procedure
        currentProcedure = Procedure();

        // process parameters, this adds the parameters to currentProcedure
        visit(n->paramList);

        // construct signature
        std::string signature = n->name;
        for (const auto& parameter : currentProcedure.parameters) {
            signature += "$" + parameter.type;
        }

        // signature check
        if (currentType.methods.find(signature) != currentType.methods.end()) {
            std::cout << "In definition of type " << currentType.name << " method @" << n->name << " is defined more than once with the same parameter types.\n"
                        << "(Mangled: " << signature << ")\n";
            exit(1);    
        }

        // fully define currentProcedure with name, signature, and type (parameters already added when visited)
        currentProcedure.name = n->name;
        currentProcedure.signature = signature;
        currentProcedure.returnType = n->returnType;

        // add to type's methods
        currentType.methods.emplace(signature, currentProcedure);
    }

    void visit(std::shared_ptr<Node::OperatorDefn> n)  {
// set currentProcedure to an empty Procedure
        currentProcedure = Procedure();

        // process parameters, this adds the parameters to currentProcedure
        visit(n->parameter);

        // construct signature
        std::string signature = n->op;
        for (const auto& parameter : currentProcedure.parameters) {
            signature += "$" + parameter.type;
        }

        // signature check
        if (currentType.operators.find(signature) != currentType.operators.end()) {
            std::cout << "In definition of type " << currentType.name << " operator " << n->op << " is defined more than once with the same parameter types.\n"
                        << "(Mangled: " << signature << ")\n";
            exit(1);    
        }

        // fully define currentProcedure with name, signature, and type (parameters already added when visited)
        currentProcedure.returnType = n->returnType;
        currentProcedure.name = n->op;
        currentProcedure.signature = signature;

        // add to type's operators (allows it to be used in itself)
        currentType.operators.emplace(signature, currentProcedure);
    }


};

#endif
