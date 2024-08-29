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

    std::shared_ptr<Program> parse() {
        auto programBody = parseCompStatement();

        if (it != tokens.end()) {
            std::cout << "Bad statement in global scope.\n";
            exit(1);
        }

        return std::make_shared<Program>(programBody); 
    }

    // compound statement
    std::shared_ptr<CompStatement> parseCompStatement() {

        std::vector<std::shared_ptr<Statement>> statements;
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

            // unrecognized statement type
            else {
                return std::make_shared<CompStatement>(statements);
            }
        }

        return std::make_shared<CompStatement>(statements);
    }

    // identifier = expr
    std::shared_ptr<Assignment> parseAssignment() {
        std::string name = consume(Token::identifier);
        discard(Token::opAssign);
        auto expr = parseEqualityExpr();

        return std::make_shared<Assignment>(name, expr);
    }

    // return expr
    std::shared_ptr<Return> parseReturn() {
        discard(Token::kwReturn);
        auto expr = parseEqualityExpr();

        return std::make_shared<Return>(expr);
    }

    // type identifier [func_def | member_def]* end
    std::shared_ptr<TypeDefn> parseTypeDefn() {
        currentContext = "class definition";

        discard(Token::kwClass);
        std::string name = consume(Token::identifier);
        std::vector<std::shared_ptr<Node>> members {};
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

        return std::make_shared<TypeDefn>(name, members);
    }

    // @identifier:type_name = expr
    std::shared_ptr<MemberDefn> parseMemberDefn() {
        
        discard(Token::at);
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);
        discard(Token::opAssign);
        auto expr = parseEqualityExpr();

        return std::make_shared<MemberDefn>(name, type, expr);
    }

    // def identifier(param_list):typename comp_stmt end
    std::shared_ptr<FunctionDefn> parseFunctionDefn() {
        currentContext = "function definition";

        discard(Token::kwDef);
        std::string name = consume(Token::identifier);
        discard(Token::openParen);

        std::shared_ptr<ParamList> parameters = nullptr;
        if (*(it) != Token::closeParen)
            parameters = parseParamList();
        discard(Token::closeParen);
        
        discard(Token::colon);
        std::string returnType = consume(Token::identifier);
        // discard(Token::terminator);
        auto functionBody = parseCompStatement();
        discard(Token::kwEnd);

        return std::make_shared<FunctionDefn>(name, returnType, parameters, functionBody);
    }

    // [parameter, ]* parameter
    std::shared_ptr<ParamList> parseParamList() {
        currentContext = "parameter list";

        std::vector<std::shared_ptr<Parameter>> parameters;
        while (*(it+3) == Token::comma) {
            parameters.push_back(parseParameter());
            discard(Token::comma);
        }
        parameters.push_back(parseParameter());
        return std::make_shared<ParamList>(parameters);
    }

    // identifier:typename
    std::shared_ptr<Parameter> parseParameter() {
        currentContext = "parameter";

        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);

        return std::make_shared<Parameter>(name, type);
    }

    // parseEqualityExpr
    std::shared_ptr<Node> parseEqualityExpr() {

        std::shared_ptr<Node> LHS = parseRelationExpr();
        while (*it == Token::opEquality || *it == Token::opNotEquals) {
            std::string op = (*it == Token::opEquality) ? consume(Token::opEquality) : consume(Token::opNotEquals);
            auto RHS = parseRelationExpr();
            LHS = std::make_shared<EqualityExpr>(LHS, op, RHS);
        }

        return LHS;
    }

    // parseRelationExpr
    std::shared_ptr<Node> parseRelationExpr() {
        std::shared_ptr<Node> LHS = parseShiftExpr();
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
            LHS = std::make_shared<RelationExpr>(LHS, op, RHS);
        }
        return LHS;
    }

    // parseShiftExpr
    std::shared_ptr<Node> parseShiftExpr() {
        std::shared_ptr<Node> LHS = parseAdditionExpr();
        while (*it == Token::opLeftShift || *it == Token::opRightShift) {
            std::string op = (*it == Token::opLeftShift) ? consume(Token::opLeftShift) : consume(Token::opRightShift);
            auto RHS = parseAdditionExpr();
            LHS = std::make_shared<ShiftExpr>(LHS, op, RHS);
        }
        return LHS;
    }

    // parseAdditionExpr
    std::shared_ptr<Node> parseAdditionExpr() {
        std::shared_ptr<Node> LHS = parseMultiplicationExpr();
        while (*it == Token::opPlus || *it == Token::opMinus) {
            std::string op = (*it == Token::opPlus) ? consume(Token::opPlus) : consume(Token::opMinus);
            auto RHS = parseMultiplicationExpr();
            LHS = std::make_shared<AdditionExpr>(LHS, op, RHS);
        }
        return LHS;
    }

    // parseMultiplicationExpr
    std::shared_ptr<Node> parseMultiplicationExpr() {
        std::shared_ptr<Node> LHS = parsePrimary();
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
            auto RHS = parsePrimary();
            LHS = std::make_shared<MultiplicationExpr>(LHS, op, RHS);
        }
        return LHS;
    }

    // parsePrimary
    std::shared_ptr<Node> parsePrimary() {
        if (*it == Token::intLiteral) {
            return std::make_shared<IntLiteral>(consume(Token::intLiteral));
        }
        else if (*it == Token::identifier) {
            return std::make_shared<Variable>(consume(Token::identifier));
        }
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

    // identifier:typename = expression
    std::shared_ptr<VariableDefn> parseVariableDefn() {
        currentContext = "variable definition";
        
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);
        discard(Token::opAssign);
        auto expression = parseEqualityExpr();

        scopes.top().insert({name, type});
        return std::make_shared<VariableDefn>(name, type, expression);
    }

    // if expr comp_stmt [elsif expr comp_stmt]* [else comp_stmt] end
    std::shared_ptr<ConditionalBlock> parseConditionalBlock() {
        currentContext = "conditional block";

        discard(Token::kwIf);
        auto ifExpr = parseEqualityExpr();
        auto ifStmt = parseCompStatement();

        std::vector<std::shared_ptr<Node>> elsifExprs;
        std::vector<std::shared_ptr<CompStatement>> elsifStmts;
        while(*it == Token::kwElsif) {
            discard(Token::kwElsif);
            elsifExprs.push_back(parseEqualityExpr());
            elsifStmts.push_back(parseCompStatement());
        }

        std::shared_ptr<CompStatement> elseStmts; 
        if (*it == Token::kwElse) {
            discard(Token::kwElse);
            elseStmts = parseCompStatement();
        }

        discard(Token::kwEnd);

        return std::make_shared<ConditionalBlock>(ifExpr, ifStmt, elsifExprs, elsifStmts, elseStmts);
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