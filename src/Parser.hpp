// Parser.hpp

#ifndef PARSER
#define PARSER

#include <vector>
#include <stack>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "Token.hpp"
#include "Node.hpp"

struct Parser {
    std::vector<Token> tokens = {};
    std::vector<Token>::const_iterator it;
    std::string currentContext = "program";

    // symbol table - is it a variable? type? - is it a function? return type? param types? - is it a type?
    std::stack<std::unordered_map<std::string, std::string>> scopes = {};

    // constructor
    Parser(const std::vector<Token>& token_stream) 
        : tokens(token_stream), it(tokens.begin()) {
            // add primitive datatypes to global scope before parsing
            scopes.push(std::unordered_map<std::string, std::string>({{"_uint32", "type_primitive"}, {"_float32", "type_primitive"}}));
        };

    // program
    std::shared_ptr<Node::Program> parse() {
        auto programBody = parseCompStatement();

        if (it != tokens.end()) {
            std::cout << "Bad statement in global scope.\n";
            exit(1);
        }

        return std::make_shared<Node::Program>(programBody); 
    }

    // compound statement
    std::shared_ptr<Node::CompStatement> parseCompStatement() {

        std::vector<std::shared_ptr<Node::Statement>> statements;
        while (it != tokens.end()) {
            
            // variable definition
            if (*it == Token::identifier && *(it+1) == Token::colon) {
                statements.push_back(parseVariableDefn());
            }

            // function definition
            else if (*it == Token::kwDef) {
                statements.push_back(parseFunctionDefn());
            }

            // expression

            // type definition
            else if (*it == Token::kwClass) {
                statements.push_back(parseTypeDefn());
            }

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
                return std::make_shared<Node::CompStatement>(statements);
            }
        }

        return std::make_shared<Node::CompStatement>(statements);
    }

    // identifier = expr
    std::shared_ptr<Node::Assignment> parseAssignment() {
        std::string name = consume(Token::identifier);
        discard(Token::opAssign);
        auto expr = parseEqualityExpr();

        return std::make_shared<Node::Assignment>(name, expr);
    }

    // return expr
    std::shared_ptr<Node::Return> parseReturn() {
        discard(Token::kwReturn);
        auto expr = parseEqualityExpr();

        return std::make_shared<Node::Return>(expr);
    }

    // type identifier [func_def | member_def]* end
    std::shared_ptr<Node::TypeDefn> parseTypeDefn() {
        currentContext = "class definition";

        discard(Token::kwClass);
        std::string name = consume(Token::identifier);
        std::vector<std::shared_ptr<Node::Node>> members {};
        while (*it == Token::at || *it == Token::kwDef) {
            if (*it == Token::at) {
                members.push_back(parseMemberDefn());
            }
            else if (*it == Token::kwDef) {
                members.push_back(parseFunctionDefn());
            }
            else {
                std::cout << "Parser error in definition of type " << name
                          << ". Expecting tokens 'def' or '@'.\n";
            }
        }
        discard(Token::kwEnd);

        return std::make_shared<Node::TypeDefn>(name, members);
    }

    // @identifier:type_name = expr
    std::shared_ptr<Node::MemberDefn> parseMemberDefn() {
        
        discard(Token::at);
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);
        discard(Token::opAssign);
        auto expr = parseEqualityExpr();

        return std::make_shared<Node::MemberDefn>(name, type, expr);
    }

    // def identifier(param_list):typename comp_stmt end
    std::shared_ptr<Node::FunctionDefn> parseFunctionDefn() {
        currentContext = "function definition";

        discard(Token::kwDef);
        std::string name = consume(Token::identifier);
        discard(Token::openParen);

        std::shared_ptr<Node::ParamList> parameters = nullptr;
        if (*(it) != Token::closeParen)
            parameters = parseParamList();
        discard(Token::closeParen);
        
        discard(Token::colon);
        std::string returnType = consume(Token::identifier);
        // discard(Token::terminator);
        auto functionBody = parseCompStatement();
        discard(Token::kwEnd);

        return std::make_shared<Node::FunctionDefn>(name, returnType, parameters, functionBody);
    }

    // [parameter, ]* parameter
    std::shared_ptr<Node::ParamList> parseParamList() {
        currentContext = "parameter list";

        std::vector<std::shared_ptr<Node::Parameter>> parameters;
        while (*(it+3) == Token::comma) {
            parameters.push_back(parseParameter());
            discard(Token::comma);
        }
        parameters.push_back(parseParameter());
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

    // parseEqualityExpr
    std::shared_ptr<Node::Node> parseEqualityExpr() {

        std::shared_ptr<Node::Node> LHS = parseRelationExpr();
        while (*it == Token::opEquality || *it == Token::opNotEquals) {
            std::string op = (*it == Token::opEquality) ? consume(Token::opEquality) : consume(Token::opNotEquals);
            auto RHS = parseRelationExpr();
            LHS = std::make_shared<Node::EqualityExpr>(LHS, op, RHS);
        }

        return LHS;
    }

    // parseRelationExpr
    std::shared_ptr<Node::Node> parseRelationExpr() {
        std::shared_ptr<Node::Node> LHS = parseShiftExpr();
        while (*it == Token::opGreaterThan || *it == Token::opGreaterThanOrEqual ||
            *it == Token::opLessThan || *it == Token::opLessThanOrEqual ||
            *it == Token::opSpaceship) {
            std::string op;
            if (*it == Token::opGreaterThan) {
                op = consume(Token::opGreaterThan);
            } else if (*it == Token::opGreaterThanOrEqual) {
                op = consume(Token::opGreaterThanOrEqual);
            } else if (*it == Token::opLessThan) {
                op = consume(Token::opLessThan);
            } else if (*it == Token::opLessThanOrEqual) {
                op = consume(Token::opLessThanOrEqual);
            } else if (*it == Token::opSpaceship) {
                op = consume(Token::opSpaceship);
            }
            auto RHS = parseShiftExpr();
            LHS = std::make_shared<Node::RelationExpr>(LHS, op, RHS);
        }
        return LHS;
    }

    // parseShiftExpr
    std::shared_ptr<Node::Node> parseShiftExpr() {
        std::shared_ptr<Node::Node> LHS = parseAdditionExpr();
        while (*it == Token::opLeftShift || *it == Token::opRightShift) {
            std::string op = (*it == Token::opLeftShift) ? consume(Token::opLeftShift) : consume(Token::opRightShift);
            auto RHS = parseAdditionExpr();
            LHS = std::make_shared<Node::ShiftExpr>(LHS, op, RHS);
        }
        return LHS;
    }

    // parseAdditionExpr
    std::shared_ptr<Node::Node> parseAdditionExpr() {
        std::shared_ptr<Node::Node> LHS = parseMultiplicationExpr();
        while (*it == Token::opPlus || *it == Token::opMinus) {
            std::string op = (*it == Token::opPlus) ? consume(Token::opPlus) : consume(Token::opMinus);
            auto RHS = parseMultiplicationExpr();
            LHS = std::make_shared<Node::AdditionExpr>(LHS, op, RHS);
        }
        return LHS;
    }

    // parseMultiplicationExpr
    std::shared_ptr<Node::Node> parseMultiplicationExpr() {
        std::shared_ptr<Node::Node> LHS = parseDotExpr();
        while (*it == Token::opMultiply || *it == Token::opDivide || *it == Token::opModulus) {
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
            auto RHS = parseDotExpr();
            LHS = std::make_shared<Node::MultiplicationExpr>(LHS, op, RHS);
        }
        return LHS;
    }

    // parseDotExpr
    std::shared_ptr<Node::Node> parseDotExpr() {
        auto lhs = parsePrimary();

        while (*it == Token::opDot) {
            discard(Token::opDot);
            auto rhs = parsePrimary();

            lhs = std::make_shared<Node::DotExpr>(lhs, rhs);
        }
        // otherwise, lhs is the main node
        return lhs;
    }

    // parsePrimary
    std::shared_ptr<Node::Node> parsePrimary() {

        // int literal
        if (*it == Token::intLiteral) {
            return std::make_shared<Node::IntLiteral>(consume(Token::intLiteral));
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

        // ( expr )
        else if (*it == Token::openParen) {
            discard(Token::openParen);
            auto subExpr = parseEqualityExpr();
            discard(Token::closeParen);

            return subExpr;
        }

        else {
            std::cout << "Primaries are only allowed to be integers and non-global variables (identifiers) right now.\n";
            exit(1);
        }
    }

    // func_name(call_args)
    std::shared_ptr<Node::Call> parseCall() {
        std::string name = consume(Token::identifier);
        discard(Token::openParen);
        auto callArgs = parseCallArgs();
        discard(Token::closeParen);

        return std::make_shared<Node::Call>(name, callArgs);
    }

    // [expr, ]* expr 
    std::shared_ptr<Node::CallArgs> parseCallArgs() {

        std::vector<std::shared_ptr<Node::Node>> exprs = {};
        while (*it != Token::closeParen) {
            exprs.push_back(parseEqualityExpr());
            if (*it == Token::closeParen)
                break;
            discard(Token::comma);
        }

        return std::make_shared<Node::CallArgs>(exprs);
    }

    // identifier:typename = expression
    std::shared_ptr<Node::VariableDefn> parseVariableDefn() {
        currentContext = "variable definition";
        
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);
        discard(Token::opAssign);
        auto expression = parseEqualityExpr();

        scopes.top().insert({name, type});
        return std::make_shared<Node::VariableDefn>(name, type, expression);
    }

    // if expr comp_stmt [elsif expr comp_stmt]* [else comp_stmt] end
    std::shared_ptr<Node::ConditionalBlock> parseConditionalBlock() {
        currentContext = "conditional block";

        discard(Token::kwIf);
        auto ifExpr = parseEqualityExpr();
        auto ifStmt = parseCompStatement();
        std::vector<std::pair<std::shared_ptr<Node::Node>, std::shared_ptr<Node::CompStatement>>> elsifs;
        while(*it == Token::kwElsif) {
            discard(Token::kwElsif);
            auto expr = parseEqualityExpr();
            auto stmts = parseCompStatement();
            // @TODO: come back to this and figure out why it wont work without std::move()
            elsifs.push_back(std::make_pair<std::shared_ptr<Node::Node>, std::shared_ptr<Node::CompStatement>>(std::move(expr), std::move(stmts)));
        }
        std::shared_ptr<Node::CompStatement> elseStmts; 
        if (*it == Token::kwElse) {
            discard(Token::kwElse);
            elseStmts = parseCompStatement();
        }
        discard(Token::kwEnd);

        return std::make_shared<Node::ConditionalBlock>(ifExpr, ifStmt, elsifs, elseStmts);
    }

    // while expr do comp_stmt end
    std::shared_ptr<Node::WhileLoop> parseWhileLoop() {
        currentContext = "while loop";

        discard(Token::kwWhile);
        auto expr = parseEqualityExpr();
        discard(Token::kwDo);
        auto stmts = parseCompStatement();
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
            std::cout << "Parser error in " + currentContext + " on line " + std::to_string(it->lineNumber)
                      +  ". Expected token: " + Token(expectedType).toString() + ". Got " + it->toString() + " " + it->value + " instead.\n";
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
            std::cout << "Parser error in " + currentContext + " on line " + std::to_string(it->lineNumber)
                      +  ". Expected token: " + Token(expectedType).toString() + ". Got " + it->toString() + " " + it->value + " instead.\n";
            exit(1);
        }

        ++it;

        // if (expectedType == Token::identifier || expectedType == Token::intLiteral)
        return (it-1)->value;

        // @TODO: remove placeholder
        std::cout << "Placeholder for consume(Token::Type).  Got a " + (it-1)->toString() + ":  " + (it-1)->value + ". Exiting.\n";
        exit(1);
    }

};

#endif