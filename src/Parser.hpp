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
    using Identifier = std::string;
    using Kind = std::string;

    enum consumeFlag {
        variable,
        dataType,
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
            // primitive datatypes
            scopes.push(std::unordered_map<Identifier, Kind>({{"_uint32", "type_primitive"}, {"_float32", "type_primitive"}}));
        };

    // contains main parsing loop
    std::shared_ptr<CompStatement> parse() {
        currentContext = "program";

        std::vector<std::shared_ptr<Statement>> statements;
        while (it != tokens.end()) {

            // variable definition
            if (it->type == identifier && (it+1)->type == colon)
                statements.push_back(parseVariableDefn());

            // function definition

            // expression

            // type definition

            // return

            // TODO throw a real error
            else {
                std::cout << "parse() couldn't classify token sequence starting with: " + toString(it->type) + ".\n";
                exit(1);
            }
        }

        return std::make_shared<CompStatement>(statements);
    }

    // TODO: remove placeholder
    // intLiteral opPlus intLiteral
    std::shared_ptr<Expression> parseExpression() {
        currentContext = "Expression";

        std::string first = consume(intLiteral);
        discard(opPlus);
        std::string second = consume(intLiteral);

        return std::make_shared<Expression>(first, second);
    }

    // varname:typename = expr
    std::shared_ptr<VariableDefn> parseVariableDefn() {
        currentContext = "variable definition";

        Identifier name = consume(identifier, variable);
        discard(colon);
        Kind kind = consume(identifier, dataType);
        discard(opAssign);
        auto expression = parseExpression();

        scopes.top().insert({name, kind});
        auto variable = std::make_shared<Variable>(name, kind);
        return std::make_shared<VariableDefn>(variable, expression);
    }

    void discard(TokenType expectedType) {

        if (it == tokens.end()) {
           // throw unexpected end of input
        }

        if (it->type != expectedType) {
            std::cout << "Parser error in " + currentContext + " on line " + std::to_string(it->lineNumber)
                      +  ". Expected token: " + toString(expectedType) + ". Got " + toString(it->type) + " " + it->value + " instead.\n";
            exit(1);
        }
        ++it;
    }

    // consume for generics
    std::string consume(TokenType expectedType) {
        
        if (it == tokens.end()) {
            // throw unexpected end of input
        }

        if (it->type != expectedType) {
            std::cout << "Parser error in " + currentContext + " on line " + std::to_string(it->lineNumber)
                      +  ". Expected token: " + toString(expectedType) + ". Got " + toString(it->type) + " " + it->value + " instead.\n";
            exit(1);
        }

        ++it; // consume token

        if (expectedType == identifier || expectedType == intLiteral)
            return (it-1)->value;
    }

    // consume for identifiers
    std::string consume(TokenType expectedType, consumeFlag flag) {

    }

};

#endif