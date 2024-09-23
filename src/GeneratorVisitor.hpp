// GeneratorVisitor.hpp

#ifndef GENERATORVISITOR
#define GENERATORVISITOR

#include <fstream>
#include <unordered_map>
#include "Visitor.hpp"

struct GeneratorVisitor : Visitor {

    std::ofstream out;

    uint32_t Register = 0;

    std::unordered_map<std::string, std::string> typeMap {
        {"int", "i32"},
        {"float", "float"},
        {"bool", "i1"}
    };

    std::unordered_map<std::string, uint32_t> nameToRegister;

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
        //
        out << "define dso_local "
            << typeMap[n->returnType]
            << " @"
            << n->name
            << "(";
        n->paramList->accept(shared_from_this());
        out << ")";

        out << " {\n";
        n->functionBody->accept(shared_from_this());
        out << "}\n";
    }

    void visit(std::shared_ptr<Node::ParamList> n) override {
        for (auto it = n->parameters.begin(); it != n->parameters.end(); ++it) {
            (*it)->accept(shared_from_this());
            if (it != n->parameters.end() - 1) {
                out << ", ";
            }
        }
    }

    void visit(std::shared_ptr<Node::Parameter> n) override {
        out << typeMap[n->type] << " noundef %" << Register;
        nameToRegister.emplace(n->name, Register);
        ++Register;
    }

    void visit(std::shared_ptr<Node::FunctionBody> n) override {}

    void visit(std::shared_ptr<Node::LogicalExpr> n) override {}

    void visit(std::shared_ptr<Node::EqualityExpr> n) override {}

    void visit(std::shared_ptr<Node::RelationExpr> n) override {}

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {}

    void visit(std::shared_ptr<Node::AdditionExpr> n) override {}

    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {}

    void visit(std::shared_ptr<Node::AccessExpr> n) override {}

    void visit(std::shared_ptr<Node::Primary> n) override {}

    void visit(std::shared_ptr<Node::Array> n) override {}

    void visit(std::shared_ptr<Node::NewExpr> n) override {}

    void visit(std::shared_ptr<Node::IntLiteral> n) override {}

    void visit(std::shared_ptr<Node::FloatLiteral> n) override {}

    void visit(std::shared_ptr<Node::StringLiteral> n) override {}

    void visit(std::shared_ptr<Node::BoolLiteral> n) override {}
    
    void visit(std::shared_ptr<Node::Variable> n) override {}

    void visit(std::shared_ptr<Node::ExprList> n) override {}

    void visit(std::shared_ptr<Node::Call> n) override {}

    void visit(std::shared_ptr<Node::VariableDefn> n) override {}

    void visit(std::shared_ptr<Node::Assignment> n) override {}

    void visit(std::shared_ptr<Node::Return> n) override {}

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
