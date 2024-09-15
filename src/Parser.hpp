// Parser.hpp

#ifndef PARSER
#define PARSER

#include "Token.hpp"
#include "Node.hpp"

struct Parser {
    std::vector<Token> tokens = {};
    std::vector<Token>::const_iterator it;
    std::string currentContext = "program";

    // constructor
    Parser(const std::vector<Token>& token_stream) 
        : tokens(token_stream), it(tokens.begin()) {}

    // program - [var_def | func_def | type_def]*
    std::shared_ptr<Node::Program> parse() {
        std::vector<std::shared_ptr<Node::Node>> definitions; 
        
        while (it != tokens.end()) {
            if (*it == Token::identifier && *(it+1) == Token::colon) {
                definitions.push_back(parseVariableDefn());
            }
            else if (*it == Token::kwDef) {
                definitions.push_back(parseFunctionDefn());
            }
            else if (*it == Token::kwClass) {
                definitions.push_back(parseTypeDefn());
            }
            else {
                std::cout << "Parser error on line " << it->lineNumber << ". Expected a definition in global scope.\n";
                exit(1);
            }
        }

        return std::make_shared<Node::Program>(definitions); 
    }

    // func_body - [stmt]*
    std::shared_ptr<Node::FunctionBody> parseFunctionBody() {

        std::vector<std::shared_ptr<Node::Statement>> statements;
        while (true) {
            
            // variable definition
            if (*it == Token::identifier && *(it+1) == Token::colon) {
                statements.push_back(parseVariableDefn());
            }

            //  @TODO: expression

            // return_stmt
            else if (*it == Token::kwReturn) {
                statements.push_back(parseReturn());
            }

            // assignment
            else if (*it == Token::identifier) {
                statements.push_back(parseAssignment());
            }

            // conditional block
            else if (*it == Token::kwIf) {
                statements.push_back(parseConditionalBlock());
            }

            // while loop
            else if (*it == Token::kwWhile) {
                statements.push_back(parseWhileLoop());
            }

            // unrecognized statement type
            else {
                break;
            }
        }

        return std::make_shared<Node::FunctionBody>(statements);
    }

    // identifier = expr
    std::shared_ptr<Node::Assignment> parseAssignment() {
        std::string name = consume(Token::identifier);
        discard(Token::opAssign);
        auto expr = parseLogicalExpr();

        return std::make_shared<Node::Assignment>(name, expr);
    }

    // return expr
    std::shared_ptr<Node::Return> parseReturn() {
        discard(Token::kwReturn);
        auto expr = parseLogicalExpr();

        return std::make_shared<Node::Return>(expr);
    }

    // type identifier [var_def | op_def | func_def]* end
    std::shared_ptr<Node::TypeDefn> parseTypeDefn() {
        currentContext = "class definition";

        discard(Token::kwClass);
        std::string name = consume(Token::identifier);
        
        std::vector<std::shared_ptr<Node::VariableDefn>> members;
        std::vector<std::shared_ptr<Node::FunctionDefn>> methods;
        std::vector<std::shared_ptr<Node::OperatorOverload>> ops;
        while (true) {
            if (*it == Token::identifier) {
                members.push_back(parseVariableDefn());
            }
            else if (*it == Token::kwDef) {
                methods.push_back(parseFunctionDefn());
            }
            else if (*it == Token::kwOp) {
                ops.push_back(parseOperatorOverload());
            }
            else {
                break;
            }
        }
        discard(Token::kwEnd);

        return std::make_shared<Node::TypeDefn>(name, members, methods, ops);
    }

    // op operator ( parameter ) : typename comp_stmt end
    std::shared_ptr<Node::OperatorOverload> parseOperatorOverload() {

        discard(Token::kwOp);

        // consume the operator
        if (*it != Token::kwAnd && *it != Token::kwOr && (*it < Token::opDot || *it > Token::opGreaterThanOrEqual)) {
             std::cout << "Parser error in " << currentContext << " on line " << it->lineNumber
                       <<  ". Expected an operator. Got " << it->toString() << " " << it->value << " instead.\n";
            exit(1);
        }
        std::string op = consume(*it);

        discard(Token::openParen);
        auto parameter = parseParameter();
        discard(Token::closeParen);
        discard(Token::colon);
        std::string type = consume(Token::identifier);
        auto stmts = parseFunctionBody();
        discard(Token::kwEnd);

        return std::make_shared<Node::OperatorOverload>(op, parameter, type, stmts);
    }

    // def identifier(param_list):typename comp_stmt end
    std::shared_ptr<Node::FunctionDefn> parseFunctionDefn() {
        currentContext = "function definition";

        discard(Token::kwDef);
        std::string name = consume(Token::identifier);
        discard(Token::openParen);
        std::shared_ptr<Node::ParamList> parameters = nullptr;
        parameters = parseParamList();
        discard(Token::closeParen);
        discard(Token::colon);
        std::string returnType = consume(Token::identifier);
        // discard(Token::terminator);
        auto functionBody = parseFunctionBody();
        discard(Token::kwEnd);

        return std::make_shared<Node::FunctionDefn>(name, returnType, parameters, functionBody);
    }

    // [parameter, ]* parameter
    std::shared_ptr<Node::ParamList> parseParamList() {
        currentContext = "parameter list";

        std::vector<std::shared_ptr<Node::Parameter>> parameters;
        while (*it != Token::closeParen) {
            parameters.push_back(parseParameter());
            if (*it == Token::closeParen)
                break;
            discard(Token::comma);
        }

        return std::make_shared<Node::ParamList>(parameters);
    }

    // identifier:typename
    std::shared_ptr<Node::Parameter> parseParameter() {
        currentContext = "parameter";

        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);

        return std::make_shared<Node::Parameter>(name, type);
    }

    // parseLogicalExpr
    std::shared_ptr<Node::Node> parseLogicalExpr() {

        std::shared_ptr<Node::Node> LHS = parseEqualityExpr();
        while (true) {
            std::string op;
            if (*it == Token::kwAnd) {
                op = consume(Token::kwAnd);
            }
            else if (*it == Token::kwOr) {
                op = consume(Token::kwOr);
            }
            else {
                break;
            }
            auto RHS = parseEqualityExpr();
            LHS = std::make_shared<Node::LogicalExpr>(LHS, op, RHS);
        }

        return LHS;
    }

    // parseEqualityExpr
    std::shared_ptr<Node::Node> parseEqualityExpr() {
        std::shared_ptr<Node::Node> LHS = parseRelationExpr();
        while (true) {
            std::string op;
            if (*it == Token::opEquality) {
                op = consume(Token::opEquality);
            }
            else if (*it ==Token::opNotEquals) {
                op = consume(Token::opNotEquals);
            }
            else {
                break;
            }
            auto RHS = parseRelationExpr();
            LHS = std::make_shared<Node::EqualityExpr>(LHS, op, RHS);
        }

        return LHS;
    }

    // parseRelationExpr
    std::shared_ptr<Node::Node> parseRelationExpr() {
        std::shared_ptr<Node::Node> LHS = parseShiftExpr();
        while (true) {
            std::string op;
            if (*it == Token::opGreaterThan) {
                op = consume(Token::opGreaterThan);
            }
            else if (*it == Token::opGreaterThanOrEqual) {
                op = consume(Token::opGreaterThanOrEqual);
            }
            else if (*it == Token::opLessThan) {
                op = consume(Token::opLessThan);
            }
            else if (*it == Token::opLessThanOrEqual) {
                op = consume(Token::opLessThanOrEqual);
            }
            else {
                break;
            }
            auto RHS = parseShiftExpr();
            LHS = std::make_shared<Node::RelationExpr>(LHS, op, RHS);
        }

        return LHS;
    }

    // parseShiftExpr
    std::shared_ptr<Node::Node> parseShiftExpr() {
        std::shared_ptr<Node::Node> LHS = parseAdditionExpr();
        while (true) {
            std::string op; 
            if (*it == Token::opLeftShift) {
                op = consume(Token::opLeftShift);
            }
            else if (*it == Token::opRightShift) {
                op = consume(Token::opRightShift);
            }
            else {
                break;
            }
            auto RHS = parseAdditionExpr();
            LHS = std::make_shared<Node::ShiftExpr>(LHS, op, RHS);
        }

        return LHS;
    }

    // parseAdditionExpr
    std::shared_ptr<Node::Node> parseAdditionExpr() {
        std::shared_ptr<Node::Node> LHS = parseMultiplicationExpr();
        while (true) {
            std::string op;
            if (*it == Token::opPlus) {
                op = consume(Token::opPlus);
            }
            else if (*it == Token::opMinus) {
                op = consume(Token::opMinus);
            }
            else {
                break;
            }
            auto RHS = parseMultiplicationExpr();
            LHS = std::make_shared<Node::AdditionExpr>(LHS, op, RHS);
        }

        return LHS;
    }

    // parseMultiplicationExpr
    std::shared_ptr<Node::Node> parseMultiplicationExpr() {
        std::shared_ptr<Node::Node> LHS = parseAccessExpr();
        while (true) {
            std::string op;
            if (*it == Token::opMultiply) {
                op = consume(Token::opMultiply);
            }
            else if (*it == Token::opDivide) {
                op = consume(Token::opDivide);
            }
            else if (*it == Token::opModulus) {
                op = consume(Token::opModulus);
            }
            else {
                break;
            }
            auto RHS = parseAccessExpr();
            LHS = std::make_shared<Node::MultiplicationExpr>(LHS, op, RHS);
        }
        
        return LHS;
    }

    // parseAccessExpr
    std::shared_ptr<Node::Node> parseAccessExpr() {
        auto LHS = parsePrimary();

        while (true) {
            if (*it == Token::opDot) {
                discard(Token::opDot);
                // only variables and calls may follow opDot
                std::shared_ptr<Node::Node> RHS;
                if (*it == Token::identifier) {
                    RHS = parsePrimary();
                }
                else {
                    std::cout << "Parser error on line " << it->lineNumber << ". Only variables and calls may follow opDot.\n";
                    exit(1);
                }
                LHS = std::make_shared<Node::AccessExpr>(LHS, ".", RHS);
            }
            else if (*it == Token::openBracket) {
                discard(Token::openBracket);
                auto RHS = parseLogicalExpr();
                LHS = std::make_shared<Node::AccessExpr>(LHS, "[]", RHS);
                discard(Token::closeBracket);
            }
            else {
                break;
            }
        }

        return LHS;
    }

    // parsePrimary
    std::shared_ptr<Node::Node> parsePrimary() {

        // int literal
        if (*it == Token::intLiteral) {
            return std::make_shared<Node::IntLiteral>(consume(Token::intLiteral));
        }

        // float literal
        else if (*it == Token::floatLiteral) {
            return std::make_shared<Node::FloatLiteral>(consume(Token::floatLiteral));
        }

        // string literal
        else if (*it == Token::doubleQuoteString) {
            return std::make_shared<Node::StringLiteral>(consume(Token::doubleQuoteString));
        }

        // identifier, call
        else if (*it == Token::identifier) {

            // call
            if (*(it+1) == Token::openParen) {
                return parseCall();
            }

            // identifier
            return std::make_shared<Node::Variable>(consume(Token::identifier));
        }

        // @TODO: disallow paren expressions after dot operator. maybe in semantic analyzer?
        // ( expr )
        else if (*it == Token::openParen) {
            discard(Token::openParen);
            auto subExpr = parseLogicalExpr();
            discard(Token::closeParen);

            return subExpr;
        }

        // array
        else if (*it == Token::openBracket) {
            discard(Token::openBracket);
            auto exprs = parseExprList();
            discard(Token::closeBracket);

            return std::make_shared<Node::Array>(exprs);
        }

        else {
            std::cout << "Primaries are only allowed to be variables, calls, and literals. Got token" << it->toString() << " instead.\n";
            exit(1);
        }
    }

    // func_name(expr_list)
    std::shared_ptr<Node::Call> parseCall() {
        std::string name = consume(Token::identifier);
        discard(Token::openParen);
        auto callArgs = parseExprList();
        discard(Token::closeParen);

        return std::make_shared<Node::Call>(name, callArgs);
    }

    // [expr, ]* expr 
    std::shared_ptr<Node::ExprList> parseExprList() {
        std::vector<std::shared_ptr<Node::Node>> exprs = {};
        while (*it != Token::closeParen && *it != Token::closeBracket) {
            exprs.push_back(parseLogicalExpr());
            if (*it == Token::closeParen || *it == Token::closeBracket)
                break;
            discard(Token::comma);
        }

        return std::make_shared<Node::ExprList>(exprs);
    }

    // identifier:typename = expression
    std::shared_ptr<Node::VariableDefn> parseVariableDefn() {
        currentContext = "variable definition";
        
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);
        discard(Token::opAssign);
        auto expression = parseLogicalExpr();

        return std::make_shared<Node::VariableDefn>(name, type, expression);
    }

    // if expr comp_stmt [elsif expr comp_stmt]* [else comp_stmt] end
    std::shared_ptr<Node::ConditionalBlock> parseConditionalBlock() {
        currentContext = "conditional block";

        discard(Token::kwIf);
        auto ifExpr = parseLogicalExpr();
        auto ifStmt = parseFunctionBody();
        std::vector<std::pair<std::shared_ptr<Node::Node>, std::shared_ptr<Node::FunctionBody>>> elsifs;
        while(*it == Token::kwElsif) {
            discard(Token::kwElsif);
            auto expr = parseLogicalExpr();
            auto stmts = parseFunctionBody();
            // @TODO: come back to this and figure out why it wont work without std::move()
            elsifs.push_back(std::make_pair<std::shared_ptr<Node::Node>, std::shared_ptr<Node::FunctionBody>>(std::move(expr), std::move(stmts)));
        }
        std::shared_ptr<Node::FunctionBody> elseStmts; 
        if (*it == Token::kwElse) {
            discard(Token::kwElse);
            elseStmts = parseFunctionBody();
        }
        discard(Token::kwEnd);

        return std::make_shared<Node::ConditionalBlock>(ifExpr, ifStmt, elsifs, elseStmts);
    }

    // while expr do comp_stmt end
    std::shared_ptr<Node::WhileLoop> parseWhileLoop() {
        currentContext = "while loop";

        discard(Token::kwWhile);
        auto expr = parseLogicalExpr();
        discard(Token::kwDo);
        auto stmts = parseFunctionBody();
        discard(Token::kwEnd);

        return std::make_shared<Node::WhileLoop>(expr, stmts);
    }

    // discard
    void discard(Token::Type expectedType) {

        if (it == tokens.end()) {
           // throw unexpected end of input
           std::cout << "Unexpected end of input.\n";
           exit(1);
        }

        if (*it != expectedType) {
            std::cout << "Parser error in " << currentContext << " on line " << std::to_string(it->lineNumber)
                      <<  ". Expected token: " << Token(expectedType).toString() << ". Got " << it->toString() << " " << it->value << " instead.\n";
            exit(1);
        }

        // discard any number of sequential terminators
        if (*it == Token::terminator) {
            while (*(it) == Token::terminator) {
                ++it;
            }
        }

        else
            ++it;
    }

    // consume
    std::string consume(Token::Type expectedType) {
        
        if (it == tokens.end()) {
            // throw unexpected end of input
            std::cout << "Unexpected end of input.\n";
            exit(1);
        }

        if (*it != expectedType) {
            std::cout << "Parser error in " << currentContext << " on line " << std::to_string(it->lineNumber)
                      <<  ". Expected token: " << Token(expectedType).toString() << ". Got " << it->toString() << " " << it->value << " instead.\n";
            exit(1);
        }

        ++it;

        // if (expectedType == Token::identifier || expectedType == Token::intLiteral)
        return (it-1)->value;

        // @TODO: remove placeholder
        std::cout << "Placeholder for consume(Token::Type).  Got a " << (it-1)->toString() << ":  " << (it-1)->value << ". Exiting.\n";
        exit(1);
    }

};

#endif