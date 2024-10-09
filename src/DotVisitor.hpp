// DotVisitor.hpp

#ifndef DOTVISITOR
#define DOTVISITOR

#include <vector>
#include <fstream>
#include "Visitor.hpp"
#include "Node.hpp"

struct DotVisitor : Visitor {

    int nodeId = 0;
    std::ofstream dotFile;
    std::string fileName;

    // constructor
    DotVisitor(const std::string& fileName) 
        : fileName(fileName) {}

    // visit node
    void visit(std::shared_ptr<Node::Node> n) override {}

    // visit program
    void visit(std::shared_ptr<Node::Program> n) override {
        
        // header
        dotFile.open(fileName);
        dotFile << "graph G {\n";

        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"program\"];\n";

        // process child(ren)
        std::vector<int> definitionIds = {};
        for (auto definition : n->definitions) {
            definitionIds.push_back(nodeId);
            definition->accept(shared_from_this());
        }

        // connect child(ren) to this node
        for (auto id : definitionIds) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
        }

        // footer
        dotFile << "}\n";
        dotFile.close();
    }

    // visit global declaration
    void visit(std::shared_ptr<Node::GlobalDecl> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "global_def\n$" << n->name << " : " << n->type
                << "\"];\n";
    }

    // visit global
    void visit(std::shared_ptr<Node::Global> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"$"
                << n->name
                << "\"];\n";
    }

    // visit function body
    void visit(std::shared_ptr<Node::FunctionBody> n) override {
        int thisId = nodeId;
        ++nodeId;
        
        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"body\"];\n";

        // process child(ren)
        std::vector<int> substatementIds = {};
        for (auto statement : n->statements) {
            substatementIds.push_back(nodeId);
            statement->accept(shared_from_this());
        }

        // connect child(ren) to this node
        for (auto id : substatementIds) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
        }
    }

    // visit variable definition
    void visit(std::shared_ptr<Node::VariableDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "var_def\n" << n->name << " : " << n->type
                << "\"];\n";

        // process child(ren)
        int exprId = nodeId;
        n->expression->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
    }

    // visit logical expression
    void visit(std::shared_ptr<Node::LogicalExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->op
                << "\"];\n";

        // process child(ren)
        int lhsId = nodeId;
        n->LHS->accept(shared_from_this());
        int rhsId = nodeId;
        n->RHS->accept(shared_from_this());
        

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(lhsId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rhsId) << ";\n";
    }

    // visit equality expression == !=
    void visit(std::shared_ptr<Node::EqualityExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->op
                << "\"];\n";

        // process child(ren)
        int lhsId = nodeId;
        n->LHS->accept(shared_from_this());
        int rhsId = nodeId;
        n->RHS->accept(shared_from_this());
        

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(lhsId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rhsId) << ";\n";
    }

    // visit relation expression
    void visit(std::shared_ptr<Node::RelationExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->op
                << "\"];\n";

        // process child(ren)
        int lhsId = nodeId;
        n->LHS->accept(shared_from_this());
        int rhsId = nodeId;
        n->RHS->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(lhsId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rhsId) << ";\n";
    }

    // visit shift expression
    void visit(std::shared_ptr<Node::ShiftExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->op
                << "\"];\n";

        // process child(ren)
        int lhsId = nodeId;
        n->LHS->accept(shared_from_this());
        int rhsId = nodeId;
        n->RHS->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(lhsId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rhsId) << ";\n";
    }

    // visit addition expression
    void visit(std::shared_ptr<Node::AdditionExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->op
                << "\"];\n";

        // process child(ren)
        int lhsId = nodeId;
        n->LHS->accept(shared_from_this());
        int rhsId = nodeId;
        n->RHS->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(lhsId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rhsId) << ";\n";
    }

    // visit multiplication expression
    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->op
                << "\"];\n";

        // process child(ren)
        int lhsId = nodeId;
        n->LHS->accept(shared_from_this());
        int rhsId = nodeId;
        n->RHS->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(lhsId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rhsId) << ";\n";
    }

    // visit access expression
    void visit(std::shared_ptr<Node::AccessExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->op
                << "\"];\n";

        // process child(ren)
        int lhsId = nodeId;
        n->LHS->accept(shared_from_this());
        int rhsId = nodeId;
        n->RHS->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(lhsId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rhsId) << ";\n";
    }

    // visit dot rhs
    void visit(std::shared_ptr<Node::DotRHS> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"member\n"
                << n->name
                << "\"];\n";
    }

    void visit(std::shared_ptr<Node::Primary> n) override {}

    // new expression
    void visit(std::shared_ptr<Node::NewExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"new "
                << n->type
                << "\"];\n";

        // process child(ren)
        int exprsId = nodeId;
        n->args->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprsId) << ";\n";  
    }

    // stack expression
    void visit(std::shared_ptr<Node::StackExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"stack["
                << n->number << "] "
                << n->type << "\"];\n";
    }

    // heap expression
    void visit(std::shared_ptr<Node::HeapExpr> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"heap[ ... ] "
                << n->type << "\"];\n";

        // process child(ren)
        int exprId = nodeId;
        n->expr->accept(shared_from_this());
        
        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
    }

    // visit array
    void visit(std::shared_ptr<Node::Array> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "[ ... ]\"];\n";

        // process child(ren)
        int exprsId = nodeId;
        n->exprs->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprsId) << ";\n";
    }

    // visit call
    virtual void visit(std::shared_ptr<Node::Call> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "call\n" << n->name
                << "\"];\n";

        // process child(ren)
        int argsId = nodeId;
        n->args->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(argsId) << ";\n";
    }

    // visit expr list
    virtual void visit(std::shared_ptr<Node::ExprList> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "exprs\"];\n";

        // process child(ren)
        std::vector<int> exprIds = {};
        for (auto arg : n->exprs) {
            exprIds.push_back(nodeId);
            arg->accept(shared_from_this());
        }

        // connect child(ren) to this node
        for (auto id : exprIds) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
        }
    }
    
    // visit int literal
    void visit(std::shared_ptr<Node::IntLiteral> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->value
                << "\"];\n";
    }

    // visit float literal
    void visit(std::shared_ptr<Node::FloatLiteral> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->value
                << "\"];\n";
    }

    // visit string literal
    void visit(std::shared_ptr<Node::StringLiteral> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"\\\""
                << n->value
                << "\\\"\"];\n";
    }

    // visit bool literal
    void visit(std::shared_ptr<Node::BoolLiteral> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->value
                << "\"];\n";
    }

    // visit variable
    void visit(std::shared_ptr<Node::Variable> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->name
                << "\"];\n";
    }

    // visit function defn
    void visit(std::shared_ptr<Node::FunctionDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "func_def\n" << n->name << " : " << n->returnType
                << "\"];\n";

        // process child(ren)
        int paramListId = nodeId;
        if (n->paramList) {
            n->paramList->accept(shared_from_this());
        }
        int functionBodyId = nodeId;
        n->functionBody->accept(shared_from_this());

        // connect child(ren) to this node
        if (n->paramList) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(paramListId) << ";\n";
        }
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(functionBodyId) << ";\n";
    }

    // visit parameter list
    void visit(std::shared_ptr<Node::ParamList> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "params"
                << "\"];\n";

        // process child(ren)
        std::vector<int> parameterIds = {};
        for (auto parameter : n->parameters) {
            parameterIds.push_back(nodeId);
            parameter->accept(shared_from_this());
        }

        // connect child(ren) to this node
        for (auto id : parameterIds) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
        }
    }

    // visit parameter
    void visit(std::shared_ptr<Node::Parameter> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->name << " : " << n->type
                << "\"];\n";
    }

    // visit assignment
    void visit(std::shared_ptr<Node::Assignment> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"=\"];\n";

        // process child(ren)
        int lhsId = nodeId;
        n->LHS->accept(shared_from_this());
        int rhsId = nodeId;
        n->RHS->accept(shared_from_this());
        

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(lhsId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rhsId) << ";\n";
    }

    // visit return
    void visit(std::shared_ptr<Node::Return> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "return"
                << "\"];\n";

        // process child(ren)
        if (n->expr) {
        int exprId = nodeId;
        n->expr->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
        }
    }

    // visit type defn
    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "type_def\n " << n->name
                << "\"];\n";

        // process child(ren)
        std::vector<int> memberIds = {};
        for (auto member : n->members) {
            memberIds.push_back(nodeId);
            member->accept(shared_from_this());
        }
        for (auto constructor : n->constructors) {
            memberIds.push_back(nodeId);
            constructor->accept(shared_from_this());
        }
        for (auto method : n->methods) {
            memberIds.push_back(nodeId);
            method->accept(shared_from_this());
        }
        for (auto op : n->operators) {
            memberIds.push_back(nodeId);
            op->accept(shared_from_this());
        }

        // connect child(ren) to this node
        for (auto id : memberIds) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
        }

    } 

    // visit constructor definition
    void visit(std::shared_ptr<Node::ConstructorDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "constructor"
                << "\"];\n";

        // process child(ren)
        int paramId = nodeId;
        n->parameters->accept(shared_from_this());

        int bodyId = nodeId;
        n->body->accept(shared_from_this());


        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(paramId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(bodyId) << ";\n";
    }

    // visit operator overload
    void visit(std::shared_ptr<Node::OperatorDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "op" << n->op << " : " << n->returnType
                << "\"];\n";

        // process child(ren)
        int paramId = nodeId;
        n->parameter->accept(shared_from_this());

        int functionBodyId = nodeId;
        n->stmts->accept(shared_from_this());


        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(paramId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(functionBodyId) << ";\n";
    }
    
    // visit member definition
    void visit(std::shared_ptr<Node::MemberDecl> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "member_decl\n@" << n->name << " : " << n->type
                << "\"];\n";
    }

    // visit method definition
    void visit(std::shared_ptr<Node::MethodDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "method_def\n@" << n->name << " : " << n->returnType
                << "\"];\n";

        // process child(ren)
        int paramListId = nodeId;
        if (n->paramList) {
            n->paramList->accept(shared_from_this());
        }
        int functionBodyId = nodeId;
        n->functionBody->accept(shared_from_this());

        // connect child(ren) to this node
        if (n->paramList) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(paramListId) << ";\n";
        }
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(functionBodyId) << ";\n";
    }

    // visit member
    void visit(std::shared_ptr<Node::Member> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"@"
                << n->name
                << "\"];\n";
    }

    // visit method call
    void visit(std::shared_ptr<Node::MethodCall> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "call\n@" << n->name
                << "\"];\n";

        // process child(ren)
        int argsId = nodeId;
        n->args->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(argsId) << ";\n";
    }

    // visit unheap
    void visit(std::shared_ptr<Node::Unheap> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"unheap\"];\n";

        // process child(ren)
        int exprId = nodeId;
        n->expr->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
    }

    // visit conditional block
    void visit(std::shared_ptr<Node::Conditional> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"conditional\"];\n";
            
        // process child(ren)
        std::vector<int> substatementIds = {};

        substatementIds.push_back(nodeId);
        n->ifExpr->accept(shared_from_this());
        substatementIds.push_back(nodeId);
        n->ifStmts->accept(shared_from_this());

        for (auto [expr, stmts] : n->elsifs) {
            substatementIds.push_back(nodeId);
            expr->accept(shared_from_this());
            substatementIds.push_back(nodeId);
            stmts->accept(shared_from_this());
        }

        if (n->elseStmts) {
            substatementIds.push_back(nodeId);
            n->elseStmts->accept(shared_from_this());
        }

        // connect child(ren) to this node
        for (auto id : substatementIds) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
        }
    }

    // visit while loop
    void visit (std::shared_ptr<Node::WhileLoop> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "while"
                << "\"];\n";

        // process child(ren)
        int exprId = nodeId;
        n->expr->accept(shared_from_this());
        int stmtsId = nodeId;
        n->stmts->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(stmtsId) << ";\n";
    }

    // break
    void visit(std::shared_ptr<Node::Break> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId) << " [label=\"break\"];\n";
    }

    // continue
    void visit(std::shared_ptr<Node::Continue> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId) << " [label=\"continue\"];\n";
    }
};

#endif
