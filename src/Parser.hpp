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
        variable,
        type,
        function
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
    std::shared_ptr<CompStatement> parse() {
        currentContext = "program";

        std::vector<std::shared_ptr<Statement>> statements;
        while (it != tokens.end()) {
            
            // variable definition
            if (it->type == Token::identifier && (it+1)->type == Token::colon)
                statements.push_back(parseVariableDefn());

            // function definition

            // expression

            // type definition

            // return

            // @TODO: throw a real error
            else {
                std::cout << "parse() couldn't classify token sequence starting with: " + it->toString() + ".\n";
                exit(1);
            }
        }

        return std::make_shared<CompStatement>(statements);
    }

    // @TODO:
    // intLiteral opPlus intLiteral
    std::shared_ptr<Expression> parseExpression() {
        currentContext = "Expression";

        std::string first = consume(Token::intLiteral);
        discard(Token::opPlus);
        std::string second = consume(Token::intLiteral);

        return std::make_shared<Expression>(first, second);
    }

    // varname:typename = expr
    std::shared_ptr<VariableDefn> parseVariableDefn() {
        currentContext = "variable definition";
        
        std::string name = consume(Token::identifier, variable);
        discard(Token::colon);
        std::string kind = consume(Token::identifier, type);
        discard(Token::opAssign);
        auto expression = parseExpression();

        scopes.top().insert({name, kind});
        auto variable = std::make_shared<Variable>(name, kind);
        return std::make_shared<VariableDefn>(variable, expression);
    }

    // discard
    void discard(Token::Type expectedType) {

        if (it == tokens.end()) {
           // throw unexpected end of input
        }

        if (it->type != expectedType) {
            std::cout << "Parser error in " + currentContext + " on line " + std::to_string(it->lineNumber)
                      +  ". Expected token: " + Token(expectedType).toString() + ". Got " + it->toString() + " " + it->value + " instead.\n";
            exit(1);
        }
        ++it;
    }

    // consume for generics
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
    }

    // consume for identifiers
    std::string consume(Token::Type expectedType, IdentifierSubtype subtype) {

        if (it == tokens.end()) {
            // throw unexpected end of input
        }

        if (*it != Token::identifier) {
            std::cout << "Parser error in " + currentContext + " on line " + std::to_string(it->lineNumber)
                      + "Expected an identifier (variable? function? typename?). Got " + it->toString() + " " + it->value + " instead.\n";;
            exit(1);
        }

        
        ++it;

        // @TODO: remove placeholder
        return "placeholder";
    }

};

#endif