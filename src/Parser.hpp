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

    enum IdentifierSubtype {
        variableName,
        dataType,
        functionName
    };
    
    std::vector<Token> tokens = {};
    std::vector<Token>::const_iterator it;
    std::string currentContext = "program";

    // symbol table - is it a variable? type? - is it a function? return type? param types? - is it a type?
    std::stack<std::unordered_map<std::string, std::string>> scopes = {};

    // constructor
    Parser(std::vector<Token> token_stream) 
        : tokens(token_stream), it(tokens.begin()) {
            // add primitive datatypes to global scope before parsing
            scopes.push(std::unordered_map<std::string, std::string>({{"_uint32", "type_primitive"}, {"_float32", "type_primitive"}}));
        };

    // main parsing loop
    std::shared_ptr<CompStatement> parseCompStatement() {

        std::vector<std::shared_ptr<Statement>> statements;
        while (it != tokens.end()) {
            
            // variable definition
            if (it->type == Token::identifier && (it+1)->type == Token::colon)
                statements.push_back(parseVariableDefn());

            // function definition
            else if (it->type == Token::kwDef)
                statements.push_back(parseFunctionDefn());

            // expression

            // type definition

            // return

            // @TODO: throw a real error
            else {
                std::cout << "parse() couldn't classify token sequence starting with: " + it->toString() + ".\n";
                exit(1);
            }

            // require a terminator after every statement
            discard(Token::terminator);
        }

        return std::make_shared<CompStatement>(statements);
    }

    // def identifier(param_list):typename terminator comp_stmt end
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
        discard(Token::terminator);
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

    // @TODO: expand expression parsing
    // intLiteral opPlus intLiteral
    std::shared_ptr<Expression> parseExpression() {
        currentContext = "expression";

        std::string first = consume(Token::intLiteral);
        discard(Token::opPlus);
        std::string second = consume(Token::intLiteral);

        return std::make_shared<Expression>(first, second);
    }

    // identifier:identifier = expression
    // or more accurately, variableName:dataType = expression
    std::shared_ptr<VariableDefn> parseVariableDefn() {
        currentContext = "variable definition";
        
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);
        discard(Token::opAssign);
        auto expression = parseExpression();

        scopes.top().insert({name, type});
        return std::make_shared<VariableDefn>(name, type, expression);
    }

    // discard
    void discard(Token::Type expectedType) {

        if (it == tokens.end()) {
           // throw unexpected end of input
        }

        if (*it != expectedType) {
            std::cout << "Parser error in " + currentContext + " on line " + std::to_string(it->lineNumber)
                      +  ". Expected token: " + Token(expectedType).toString() + ". Got " + it->toString() + " " + it->value + " instead.\n";
            exit(1);
        }
        ++it;
    }

    // generic consume
    std::string consume(Token::Type expectedType) {
        
        if (it == tokens.end()) {
            // throw unexpected end of input
        }

        if (*it != expectedType) {
            std::cout << "Parser error in " + currentContext + " on line " + std::to_string(it->lineNumber)
                      +  ". Expected token: " + Token(expectedType).toString() + ". Got " + it->toString() + " " + it->value + " instead.\n";
            exit(1);
        }

        ++it;

        if (expectedType == Token::identifier || expectedType == Token::intLiteral)
            return (it-1)->value;

        // @TODO: remove placeholder
        std::cout << "Placeholder for consume(Token::Type).  Got a " + (it-1)->toString() + ":  " + (it-1)->value + ". Exiting.\n";
        exit(1);
    }

};

#endif