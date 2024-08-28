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
    void visit(std::shared_ptr<Node> n) override {}

    // visit program
    void visit(std::shared_ptr<Program> n) override {
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
    void visit(std::shared_ptr<CompStatement> n) override {
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
        // std::cout << "This comp_stmt has " << std::to_string(substatementIds.size()) << " child(ren).\n";

        // connect child(ren) to this node
        for (auto id : substatementIds) {
            // std::cout << "connecting to compstmt\n";
            dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
        }
    }

    // visit variable definition
    void visit(std::shared_ptr<VariableDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // std::cout << "this is a variable definition\n";

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
    void visit(std::shared_ptr<EqualityExpr> n) override {
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
    void visit(std::shared_ptr<RelationExpr> n) override {
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
    void visit(std::shared_ptr<ShiftExpr> n) override {
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
    void visit(std::shared_ptr<AdditionExpr> n) override {
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
    void visit(std::shared_ptr<MultiplicationExpr> n) override {
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

    void visit(std::shared_ptr<Primary> n) override {

    }

    // visit int literal
    void visit(std::shared_ptr<IntLiteral> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->value
                << "\"];\n";
    }
    
    // visit variable
    void visit(std::shared_ptr<Variable> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->name
                << "\"];\n";
    }

    // visit function defn
    void visit(std::shared_ptr<FunctionDefn> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "Define " << n->name << " " << n->returnType
                << "\"];\n";

        // process child(ren)
        int paramListId = nodeId;
        n->paramList->accept(shared_from_this());
        int functionBodyId = nodeId;
        n->functionBody->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(paramListId) << ";\n";
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(functionBodyId) << ";\n";
    }

    // visit parameter list
    void visit(std::shared_ptr<ParamList> n) override {
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
    void visit(std::shared_ptr<Parameter> n) override {
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->name << " " << n->type
                << "\"];\n";
    }

    // visit assignment
    void visit(std::shared_ptr<Assignment> n) override {
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
    void visit(std::shared_ptr<Return> n) override {
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

};

#endif