// GeneratorVisitor.hpp

#ifndef GENERATORVISITOR
#define GENERATORVISITOR

#include <fstream>
#include <unordered_map>
#include "Visitor.hpp"

struct GeneratorVisitor : Visitor {

    // output file
    std::ofstream out;

    uint32_t currentRegister = 0;

    std::unique_ptr<Procedure> currentProcedure = nullptr;

    std::unordered_map<std::string, std::string> typeMap {
        {"int", "i32"},
        {"float", "float"},
        {"bool", "i1"}
    };

    std::unordered_map<std::string, uint32_t> nameToRegister;

    struct RegisterType {
        const uint32_t reg;
        std::string type;
    };
    std::vector<RegisterType> exprStack; 

    // methods for output of certain statement types

    // output an alloca instuction
    void allocation(const std::string& type) {
        out << "  %" << currentRegister
            << " = alloca " << typeMap[type]
            << ", align 4\n";
            // @NOTE: alignment handling may change
        ++currentRegister;
    }

    // output a load instruction
    void store(const int fromReg, const int toReg, const std::string& type) {
        out << "  store " << typeMap[type]
            << " %" << fromReg << ", "
            << typeMap[type] << "*"
            << " %" << toReg
            << ", align 4\n";
    }

    // load
    void load(const int toReg, const std::string& type) {
        out << "  %" << currentRegister
            << " = load "  << typeMap[type] << ", "
            << typeMap[type] << "*"
            << " %" <<toReg
            << ", align 4\n";
        ++currentRegister;
    }

    GeneratorVisitor(const std::string& filename) {
        out.open(filename);
    }

    ~GeneratorVisitor() {
        out.close();
    }

    void visit(std::shared_ptr<Node::Node> n) override {}

    void visit(std::shared_ptr<Node::Program> n) override {
        for (const auto& definition : n->definitions) {
            definition->accept(shared_from_this());
        }
    }

    void visit(std::shared_ptr<Node::GlobalDefn> n) override {}

    void visit(std::shared_ptr<Node::Global> n) override {}

    void visit(std::shared_ptr<Node::Statement> n) override {}

    void visit(std::shared_ptr<Node::FunctionDefn> n) override {
        // move the Procedure info into currentProcedure
        currentProcedure = std::move(n->info);

        // type and name
        out << "define dso_local " << typeMap[n->returnType]
            << " @" << n->name << "(";

        // parameters
        for (auto it = currentProcedure->parameters.begin(); it != currentProcedure->parameters.end(); ++it) {
            out << typeMap[it->type] << " noundef %" << currentRegister;
            nameToRegister.emplace(it->name, currentRegister);
            ++currentRegister;

            if (it != currentProcedure->parameters.end() - 1) {
                out << ", ";
            }
        }
        out << ") {\n";

        // allocations & stores for parameters
        out << "  ; Handle parameters\n";
        for (const auto& parameter : currentProcedure->parameters) {
            allocation(parameter.type);
            store(nameToRegister[parameter.name], currentRegister-1, parameter.type);
            nameToRegister[parameter.name] = currentRegister-1;
        }
        
        n->functionBody->accept(shared_from_this());
        out << "}\n";

        // reset current procedure
        currentProcedure.reset();
    }

    void visit(std::shared_ptr<Node::ParamList> n) override {}

    void visit(std::shared_ptr<Node::Parameter> n) override {}

    void visit(std::shared_ptr<Node::FunctionBody> n) override {
        for (const auto& statement :n->statements) {
            statement->accept(shared_from_this());
        }
    }

    void visit(std::shared_ptr<Node::LogicalExpr> n) override {}

    void visit(std::shared_ptr<Node::EqualityExpr> n) override {}

    void visit(std::shared_ptr<Node::RelationExpr> n) override {}

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {}

    void visit(std::shared_ptr<Node::AdditionExpr> n) override {}

    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {
        out << "  ; Begin mult expr\n";

        // process children
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get child registers and type
        RegisterType rhs = exprStack.back();
        exprStack.pop_back();
        RegisterType lhs = exprStack.back();
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
            << " = mul nsw " << typeMap[type]
            << " %" << lhs.reg
            << ", %" << rhs.reg
            << "\n";

        exprStack.push_back({currentRegister, type});

        ++currentRegister;

        out << "  ; End mult expr\n";
    }

    void visit(std::shared_ptr<Node::AccessExpr> n) override {}

    void visit(std::shared_ptr<Node::Primary> n) override {}

    void visit(std::shared_ptr<Node::Array> n) override {}

    void visit(std::shared_ptr<Node::NewExpr> n) override {}

    void visit(std::shared_ptr<Node::IntLiteral> n) override {}

    void visit(std::shared_ptr<Node::FloatLiteral> n) override {}

    void visit(std::shared_ptr<Node::StringLiteral> n) override {}

    void visit(std::shared_ptr<Node::BoolLiteral> n) override {}
    
    void visit(std::shared_ptr<Node::Variable> n) override {
        // register type stack
        exprStack.push_back({currentRegister, n->type});
        // load
        load(nameToRegister[n->name], n->type);
    }

    void visit(std::shared_ptr<Node::ExprList> n) override {}

    void visit(std::shared_ptr<Node::Call> n) override {}

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

    void visit(std::shared_ptr<Node::Assignment> n) override {}

    void visit(std::shared_ptr<Node::Return> n) override {
        // process expression
        n->expr->accept(shared_from_this());
        RegisterType expr = exprStack.back();
        exprStack.pop_back();

        // output return
        out << "  ret " << typeMap[expr.type]
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

    void visit(std::shared_ptr<Node::WhileLoop> n) override {}

    void visit(std::shared_ptr<Node::Break> n) override {}

    void visit(std::shared_ptr<Node::Continue> n) override {}
};

#endif
