// GeneratorVisitor.hpp

#ifndef GENERATORVISITOR
#define GENERATORVISITOR

#include <fstream>
#include <unordered_map>
#include "Visitor.hpp"

struct GeneratorVisitor : Visitor {

    // output file
    std::ofstream out;

    // used to track register number
    uint32_t currentRegister = 0;

    // used to track break label names
    uint32_t currentLabelNumber = 0;

    std::unique_ptr<Procedure> currentProcedure = nullptr;

    // maps amethyst non-pointer type name to an llvm type name
    std::unordered_map<std::string, std::string> typeMap {
        {"int", "i64"},
        {"float", "float"},
        {"bool", "i1"}
    };

    std::unordered_map<std::string, uint32_t> nameToRegister;

    // is the current expression being evaluated an "lvalue" ?
    // or, are we in something that needs to return an for storing in assignment
    bool isOuterLHS = false;

    struct SubExprInfo {
        const uint32_t reg;
        std::string type;
    };

    std::vector<SubExprInfo> exprStack; 

    // constructor
    GeneratorVisitor(const std::string& filename) {
        out.open(filename);
    }

    // destructor
    ~GeneratorVisitor() {
        out.close();
    }

    // convert an amethyst type (including pointer types)
    std::string convertType(const std::string amethystType) {
        // remove *'s in order to find if non-ptr type exists
        std::string llvmType = typeMap[amethystType];
        auto it = amethystType.begin();
        while (it != amethystType.end()) {
            if (*it == '*') {
                llvmType = typeMap[std::string(amethystType.begin(), it)];
                break;
            }
            ++it;
        }
        // add *'s back
        llvmType += std::string(it, amethystType.end());

        return llvmType;
    }

    // methods for output of certain statement types

    // output an alloca instuction
    void allocation(const std::string& type) {
        out << "  %" << currentRegister
            << " = alloca " << convertType(type)
            << "\n";
        ++currentRegister;
    }

    // output a load instruction
    void store(const int fromReg, const int toReg, const std::string& type) {
        out << "  store " << convertType(type)
            << " %" << fromReg << ", "
            << convertType(type) << "*"
            << " %" << toReg
            << "\n";
    }

    // load
    void load(const int fromReg, const std::string& type) {
        out << "  %" << currentRegister
            << " = load "  << convertType(type) << ", "
            << convertType(type) << "*"
            << " %" << fromReg
            << "\n";
        ++currentRegister;
    }

    void visit(std::shared_ptr<Node::Node> n) override {}

    void visit(std::shared_ptr<Node::Program> n) override {
        for (const auto& definition : n->definitions) {
            definition->accept(shared_from_this());
            out << "\n";
        }
    }

    void visit(std::shared_ptr<Node::GlobalDefn> n) override {}

    void visit(std::shared_ptr<Node::Global> n) override {}

    void visit(std::shared_ptr<Node::Statement> n) override {}

    void visit(std::shared_ptr<Node::FunctionDefn> n) override {
        // move the Procedure info into currentProcedure
        currentProcedure = std::move(n->info);

        // @TODO: all struct returns should be pass by parameter

        // type and name
        out << "define dso_local " << convertType(n->returnType)
            << " @" << n->name << "(";

        // parameters
        for (auto it = currentProcedure->parameters.begin(); it != currentProcedure->parameters.end(); ++it) {
            out << convertType(it->type) << " noundef %" << currentRegister;
            nameToRegister.emplace(it->name, currentRegister);
            ++currentRegister;

            if (it != currentProcedure->parameters.end() - 1) {
                out << ", ";
            }
        }
        out << ") {\n";

        // allocations & stores for parameters
        out << "  ; Handle parameters\n";
        ++currentRegister;
        for (const auto& parameter : currentProcedure->parameters) {
            allocation(parameter.type);
            store(nameToRegister[parameter.name], currentRegister-1, parameter.type);
            nameToRegister[parameter.name] = currentRegister-1;
        }
        
        n->functionBody->accept(shared_from_this());
        out << "}\n";

        // reset current procedure
        currentProcedure.reset();

        // reset registers and nameToRegister map after each function
        currentRegister = 0;
        currentLabelNumber = 0;
        nameToRegister.clear();
    }

    void visit(std::shared_ptr<Node::ParamList> n) override {}

    void visit(std::shared_ptr<Node::Parameter> n) override {}

    void visit(std::shared_ptr<Node::FunctionBody> n) override {
        for (const auto& statement :n->statements) {
            statement->accept(shared_from_this());
        }
    }

    void visit(std::shared_ptr<Node::LogicalExpr> n) override {}

    void visit(std::shared_ptr<Node::EqualityExpr> n) override {
        out << "  ; Begin eq expr\n";

        // process children
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get child registers and type
        SubExprInfo rhs = exprStack.back();
        exprStack.pop_back();
        SubExprInfo lhs = exprStack.back();
        exprStack.pop_back();

        // get type
        // @TODO: check overloads for structs
        std::string type;
        if (lhs.type == "int" && rhs.type == "int") {
            type = "bool";
        }
        else {
            
        }

        // icmp eq/ne
        out << "  %" << currentRegister 
            << " = icmp "
            << (n->op == "==" ? "eq " : "ne ") 
            << convertType(lhs.type)
            << " %" << lhs.reg
            << ", %" << rhs.reg
            << "\n";

        exprStack.push_back({currentRegister, type});

        ++currentRegister;

        out << "  ; End eq expr\n";
    }

    void visit(std::shared_ptr<Node::RelationExpr> n) override {
        // @TODO: icmp <arg> ... - check reference search sgtx  
    }

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {}

    void visit(std::shared_ptr<Node::AdditionExpr> n) override {
        out << "  ; Begin add expr\n";

        // process children
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get child registers and type
        SubExprInfo rhs = exprStack.back();
        exprStack.pop_back();
        SubExprInfo lhs = exprStack.back();
        exprStack.pop_back();

        // get type
        // @TODO: check overloads for structs
        std::string type;
        if (lhs.type == "int" && rhs.type == "int") {
            type = "int";
        }
        else {
            
        }

        // add / sub
        out << "  %" << currentRegister 
            << " = " 
            << (n->op == "+" ? "add " : "sub ")
            << convertType(type)
            << " %" << lhs.reg
            << ", %" << rhs.reg
            << "\n";

        exprStack.push_back({currentRegister, type});

        ++currentRegister;

        out << "  ; End add expr\n";  
    }

    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {
        out << "  ; Begin mult expr\n";

        // process children
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get child registers and type
        SubExprInfo rhs = exprStack.back();
        exprStack.pop_back();
        SubExprInfo lhs = exprStack.back();
        exprStack.pop_back();

        // get type
        // @TODO: check overloads for structs
        std::string type;
        if (lhs.type == "int" && rhs.type == "int") {
            type = "int";
        }
        else {
            
        }

        // mul
        out << "  %" << currentRegister 
            << " = " 
            << (n->op == "*" ? "mul " : "sdiv ")
            << convertType(type)
            << " %" << lhs.reg
            << ", %" << rhs.reg
            << "\n";

        exprStack.push_back({currentRegister, type});

        ++currentRegister;

        out << "  ; End mult expr\n";
    }

    void visit(std::shared_ptr<Node::AccessExpr> n) override {

        // subscript
        if (n->op == "[]") {
            
            bool outerLHS = isOuterLHS;
            isOuterLHS = false;

            // process lhs & get type
            n->LHS->accept(shared_from_this());
            auto lhs = exprStack.back();
            exprStack.pop_back();

            // is ptr?
            if (lhs.type.back() == '*') {
                // evaluate expression in subscript
                n->RHS->accept(shared_from_this());
                auto rhs = exprStack.back();
                exprStack.pop_back();

                // get ptr to index
                std::string dereferencedType = lhs.type.substr(0, lhs.type.length()-1);
                out << "  %" << currentRegister
                    << " = getelementptr " << convertType(dereferencedType)
                    << ", " << convertType(lhs.type)
                    << " %" << lhs.reg
                    << ", i64 %" << rhs.reg
                    << "\n";
                ++currentRegister;

                // outer lhs means a pointer for storing in assignment
                if (!outerLHS) {
                    // load value at that index into new register
                    load(currentRegister-1, dereferencedType);
                }

                // push back a dereferenced LHS type
                exprStack.push_back({currentRegister-1, dereferencedType});
            }
            // otherwise, use overloads
            else {
                // @TODO: subscript overloads
            }
        }
        // dot operator
        else if (n->op == ".") {

        }

        else {
            std::cout << "Error. Code generator reached an access expression that was neither a dot operator nor subscript.\n";
            exit(1);
        }
    }

    void visit(std::shared_ptr<Node::Primary> n) override {}

    void visit(std::shared_ptr<Node::Array> n) override {}

    void visit(std::shared_ptr<Node::NewExpr> n) override {}

    void visit(std::shared_ptr<Node::StackExpr> n) override {
        // @TODO: call functions, will require changing their logic

        // allocate array
        std::string arrayType = "[" + n->number + " x " + convertType(n->type) + "]";
        out << "  %" << currentRegister
            << " = alloca " << arrayType
            << "\n";
        ++currentRegister;

        // bitcast array type to ptr type
        std::string ptrType = convertType(n->type) + "*";
        out << "  %" << currentRegister
            << " = bitcast " << arrayType 
            << "* %" << currentRegister-1
            << " to " << ptrType
            << "\n";

        ++currentRegister;

        exprStack.push_back({currentRegister-1, ptrType});
    }

    void visit(std::shared_ptr<Node::HeapExpr> n) override {}

    void visit(std::shared_ptr<Node::IntLiteral> n) override {
        // @NOTE: all int literals are i64
        // put value in register by adding to 0
        out << "  %" << currentRegister
            << " = add i64 0, " << n->value << "\n";
        // register & type stack
        exprStack.push_back({currentRegister, "int"});
        ++currentRegister;
    }

    void visit(std::shared_ptr<Node::FloatLiteral> n) override {}

    void visit(std::shared_ptr<Node::StringLiteral> n) override {}

    void visit(std::shared_ptr<Node::BoolLiteral> n) override {}
    
    void visit(std::shared_ptr<Node::Variable> n) override {
        // in LHS
        if (isOuterLHS) {
            // give the assignment the register of the variable
            exprStack.push_back({nameToRegister[n->name], n->type});   
        }
        else {
            // register & type stack
            exprStack.push_back({currentRegister, n->type});
            // load
            load(nameToRegister[n->name], n->type);
        }
    }

    void visit(std::shared_ptr<Node::ExprList> n) override {
        for (const auto& expr : n->exprs) {
            expr->accept(shared_from_this());
        }
    }

    void visit(std::shared_ptr<Node::Call> n) override {
        // process args
        n->args->accept(shared_from_this());
        // top of expr stack now contains types and registers for arg results

        // output call
        out << "  %" << currentRegister
            << " = call " << convertType(n->type)
            << " @" << n->name
            << "(";

        // output args
        for (auto argIt = exprStack.end() - n->numArgs; argIt != exprStack.end(); ++argIt) {
            out << convertType(argIt->type) << " noundef %" << argIt->reg;
            if (argIt != exprStack.end()-1) {
                out << ", ";
            }
        }
        out << ")\n";

        // clear the top of the args from the expr stack
        for (int i = 0; i < n->numArgs; ++i) {
            exprStack.pop_back();
        }

        // add this call to the expr stack
        exprStack.push_back({currentRegister, n->type});

        ++currentRegister;
    }

    void visit(std::shared_ptr<Node::VariableDefn> n) override {
        out << "\n  ; Define " << n->name << ":" << n->type << "\n";

        nameToRegister[n->name] = currentRegister;
        allocation(n->type);
        n->expression->accept(shared_from_this());
        store(currentRegister-1, nameToRegister[n->name], n->type);

        // clear expression stack
        exprStack.pop_back();

        out << "  ; End definition of " << n->name << ":" << n->type << "\n";
    }

    void visit(std::shared_ptr<Node::Assignment> n) override {
        // evaluate rhs
        n->RHS->accept(shared_from_this());
        SubExprInfo rhs = exprStack.back();
        exprStack.pop_back();

        // evaluate lhs
        isOuterLHS = true;
        n->LHS->accept(shared_from_this());
        isOuterLHS = false;
        SubExprInfo lhs = exprStack.back();
        exprStack.pop_back();

        // store
        store(rhs.reg, lhs.reg, lhs.type);
    }

    void visit(std::shared_ptr<Node::Return> n) override {
        // process expression
        n->expr->accept(shared_from_this());
        SubExprInfo expr = exprStack.back();
        exprStack.pop_back();

        // output return
        out << "  ret " << convertType(expr.type)
            << " %" << expr.reg << "\n";
    }

    void visit(std::shared_ptr<Node::TypeDefn> n) override {}

    void visit(std::shared_ptr<Node::ConstructorDefn> n) override {}

    void visit(std::shared_ptr<Node::MemberDecl> n) override {}

    void visit(std::shared_ptr<Node::MethodDefn> n) override {}

    void visit(std::shared_ptr<Node::Member> n) override {}

    void visit(std::shared_ptr<Node::MethodCall> n) override {}

    void visit(std::shared_ptr<Node::OperatorDefn> n) override {}

    void visit(std::shared_ptr<Node::Delete> n) override {}

    void visit(std::shared_ptr<Node::ConditionalBlock> n) override {}

    // @TODO: reintroduce loop metadata
    void visit(std::shared_ptr<Node::WhileLoop> n) override {
        std::string suffix = std::to_string(currentLabelNumber);
        ++currentLabelNumber;
        std::string conditionLabel = "cond" + suffix;
        std::string bodyLabel = "body" + suffix;
        std::string exitLabel = "exit" + suffix;

        // break to condition
        out << "  br label %" << conditionLabel << "\n\n";

        // condition
        out << conditionLabel << ":\n";
        n->expr->accept(shared_from_this());
        out << "  br i1 %" << currentRegister-1
            << ", label %" << bodyLabel
            << ", label %" << exitLabel
            << "\n\n";

        // body
        out << bodyLabel << ":\n";
        n->stmts->accept(shared_from_this());
        out << "  br label %" << conditionLabel
            << "\n\n";

        // exit
        out << exitLabel <<  ":\n";

    }

    void visit(std::shared_ptr<Node::Break> n) override {}

    void visit(std::shared_ptr<Node::Continue> n) override {}
};

#endif
