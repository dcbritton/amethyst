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

    // visit expression
    void visit(std::shared_ptr<Expression> n) override {
        std::cout << "expression visited\n";
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << "dummy expr - remove?"
                << "\"];\n";

        // process child(ren)
        int subExprId = nodeId;
        n->expr->accept(shared_from_this());

        // connect child(ren) to this node
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(subExprId) << ";\n";
    
    }

    // visit equality expression == !=
    void visit(std::shared_ptr<EqualityExpr> n) override {
        std::cout << "eqexpr visited";
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

    void visit(std::shared_ptr<RelationExpr> n) override {
        std::cout << "relexpr visited";
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

    void visit(std::shared_ptr<ShiftExpr> n) override {
        std::cout << "shiftexpr visited";
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

    void visit(std::shared_ptr<AdditionExpr> n) override {
        std::cout << "addexpr visited";
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

    void visit(std::shared_ptr<MultiplicationExpr> n) override {
        std::cout << "multexpr visited";
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

    void visit(std::shared_ptr<IntLiteral> n) override {
        std::cout << "intliteral visited";
        int thisId = nodeId;
        ++nodeId;

        // create this node
        dotFile << "node" << std::to_string(thisId)
                << " [label=\""
                << n->value
                << "\"];\n";
    }
    
    void visit(std::shared_ptr<Variable> n) override {

    }
};

#endif