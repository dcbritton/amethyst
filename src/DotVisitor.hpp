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

    // constructor
    DotVisitor(const std::string& filename) {
        dotFile.open(filename);
        dotFile << "graph G {\n";
    };

    // destructor
    ~DotVisitor() {
        dotFile << "}\n";
        dotFile.close();
    }

    // visit node
    void visit(std::shared_ptr<Node::Node> n) override {}

    // visit program
    void visit(std::shared_ptr<Node::Program> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"program\"];\n";

        // process child(ren)
        n->compStatement->accept(shared_from_this());
        
        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(thisId+1) << ";\n";
    }

    // visit compound statement
    void visit(std::shared_ptr<Node::CompStatement> n) override {
        int thisId = nodeId;
        ++nodeId;
        
        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\"comp_stmt\"];\n";

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
                << "Define " << n->type << " " << n->name
                << "\"];\n";

        // process child(ren)
        int exprId = nodeId;
        n->expression->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
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

    // visit dot expression
    void visit(std::shared_ptr<Node::DotExpr> n) override {
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

    void visit(std::shared_ptr<Node::Primary> n) override {}

    // visit call
    virtual void visit(std::shared_ptr<Node::Call> n) {
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

    // visit call args
    virtual void visit(std::shared_ptr<Node::CallArgs> n) {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "args\n"
                << "\"];\n";

        // process child(ren)
        std::vector<int> argIds = {};
        for (auto arg : n->exprs) {
            argIds.push_back(nodeId);
            arg->accept(shared_from_this());
        }

        // connect child(ren) to this node
        for (auto id : argIds) {
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
                << "Define " << n->name << " " << n->returnType
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
                << "Parameters"
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
                << n->name << " " << n->type
                << "\"];\n";
    }

    // visit assignment
    void visit(std::shared_ptr<Node::Assignment> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "Assign " << n->lhs
                << "\"];\n";

        // process child(ren)
        int exprId = nodeId;
        n->expr->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
    }

    // visit return
    void visit(std::shared_ptr<Node::Return> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "Return"
                << "\"];\n";

        // process child(ren)
        int exprId = nodeId;
        n->expr->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
    }

    // visit type defn
    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "define type\n " << n->name
                << "\"];\n";

        // process child(ren)
        std::vector<int> parameterIds = {};
        for (auto parameter : n->members) {
            parameterIds.push_back(nodeId);
            parameter->accept(shared_from_this());
        }

        // connect child(ren) to this node
        for (auto id : parameterIds) {
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
        }
    }

    // visit conditional block
    void visit(std::shared_ptr<Node::ConditionalBlock> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "conditional block\n"
                << "if, "
                << std::to_string(n->elsifs.size()) << " elsif(s)"
                << (n->elseStmts->statements.size() ? ", else" : " ")
                << "\"];\n";
            
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

        if (n->elseStmts->statements.size()) {
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
};

#endif