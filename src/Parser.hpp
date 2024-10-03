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

    // parse program
    std::shared_ptr<Node::Program> parse() {
        
        // discard initial newline token
        if (*it == Token ::terminator) {
            discard(Token::terminator);
        }

        // main parsing loop
        std::vector<std::shared_ptr<Node::Node>> definitions;
        while (it != tokens.end()) {
            if (*it == Token::globalSigil) {
                definitions.push_back(parseGlobalDefn());
            }
            else if (*it == Token::kwDef) {
                definitions.push_back(parseFunctionDefn());
            }
            else if (*it == Token::kwType) {
                definitions.push_back(parseTypeDefn());
            }
            else {
                std::cout << "Parser error on line " << it->lineNumber << ". Expected a definition in global scope.\n";
                exit(1);
            }

            // at end of program, no newline needed
            if (it == tokens.end()) {
                break;
            }
            // not at end of program, require terminator
            else {
                if (*it != Token::terminator) {
                    std::cout << "Parser error on line " << it->lineNumber << ". Did not separate defintions in global scope with a newline.\n";
                    exit(1);
                }
                discard(Token::terminator);
            }
        }

        return std::make_shared<Node::Program>(definitions); 
    }

    // global_def - $ identifier : identifier [*]* = logic_expr
    std::shared_ptr<Node::GlobalDefn> parseGlobalDefn() {
        currentContext = "global definition";
        
        discard(Token::globalSigil);
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);

        // pointer
        while (*it == Token::opMultiply) {
            type += consume(Token::opMultiply);
        }

        discard(Token::opAssign);
        auto expression = parseLogicalExpr();

        return std::make_shared<Node::GlobalDefn>(name, type, expression);
    }

    // global - $ identifier
    std::shared_ptr<Node::Global> parseGlobal() {
        discard(Token::globalSigil);
        std::string name = consume(Token::identifier);

        return std::make_shared<Node::Global>(name);
    }

    // func_body - [stmt TERM]*
    std::shared_ptr<Node::FunctionBody> parseFunctionBody() {

        std::vector<std::shared_ptr<Node::Node>> statements;
        while (true) {
            
            // variable definition
            if (*it == Token::identifier && *(it+1) == Token::colon) {
                statements.push_back(parseVariableDefn());
            }

            // return_stmt
            else if (*it == Token::kwReturn) {
                statements.push_back(parseReturn());
            }

            // delete
            else if (*it == Token::kwDelete) {
                statements.push_back(parseDelete());
            }

            // conditional block
            else if (*it == Token::kwIf) {
                statements.push_back(parseConditionalBlock());
            }

            // while loop
            else if (*it == Token::kwWhile) {
                statements.push_back(parseWhileLoop());
            }

            // @NOTE: non-standard increment of 'it' here
            // break
            else if (*it == Token::kwBreak) {
                statements.push_back(std::make_shared<Node::Break>());
                ++it;
            }
            // continue
            else if  (*it == Token::kwContinue) {
                statements.push_back(std::make_shared<Node::Continue>());
                ++it;
            }

            // end of function body
            else if (*it == Token::kwEnd) {
                break;
            }

            // otherwise, it must be an assignment or an expression
            else {
                statements.push_back(parseAssignment());
            }

            // statement must be terminated with newline
            discard(Token::terminator);
        }

        return std::make_shared<Node::FunctionBody>(statements);
    }

    // return expr
    std::shared_ptr<Node::Return> parseReturn() {
        discard(Token::kwReturn);

        // non-void return, require expression
        std::shared_ptr<Node::Node> expr = nullptr;
        if (*it != Token::terminator) {
            expr = parseLogicalExpr();
        }

        return std::make_shared<Node::Return>(expr);
    }

    // type identifier TERM [temp TERM]* end
    std::shared_ptr<Node::TypeDefn> parseTypeDefn() {
        currentContext = "type definition";

        discard(Token::kwType);
        std::string name = consume(Token::identifier);
        discard(Token::terminator);
        
        std::vector<std::shared_ptr<Node::MemberDecl>> members;
        std::vector<std::shared_ptr<Node::MethodDefn>> methods; 
        std::vector<std::shared_ptr<Node::OperatorDefn>> operators; 
        std::vector<std::shared_ptr<Node::ConstructorDefn>> constructors; 
        while (true) {
            if (*it == Token::memberSigil) {
                members.push_back(parseMemberDecl());
            }
            else if (*it == Token::kwDef) {
                methods.push_back(parseMethodDefn());
            }
            else if (*it == Token::kwOp) {
                operators.push_back(parseOperatorOverload());
            }
            else if (*it == Token::kwNew) {
                constructors.push_back(parseConstructorDefn());
            }
            else {
                break;
            }
            discard(Token::terminator);
        }
        discard(Token::kwEnd);

        return std::make_shared<Node::TypeDefn>(name, members, constructors, methods, operators);
    }

    // constructor - new ( param_list ) TERM func_body end
    std::shared_ptr<Node::ConstructorDefn> parseConstructorDefn() {

        discard(Token::kwNew);
        discard(Token::openParen);
        auto parameters = parseParamList();
        discard(Token::closeParen);
        discard(Token::terminator);
        auto body = parseFunctionBody();
        discard(Token::kwEnd);

        return std::make_shared<Node::ConstructorDefn>(parameters, body);
    } 

    // op operator ( parameter ) [: identifier [*]*] TERM func_body end
    // @TODO: rename to OperatorDefinition
    std::shared_ptr<Node::OperatorDefn> parseOperatorOverload() {

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

        std::string returnType = "nil";
        // non-void
        if (*it != Token::terminator) {
            discard(Token::colon);
            returnType = consume(Token::identifier);

            // pointer type
            while (*it == Token::opMultiply) {
                returnType += consume(Token::opMultiply);
            }
        }

        discard(Token::terminator);
        auto stmts = parseFunctionBody();
        discard(Token::kwEnd);

        return std::make_shared<Node::OperatorDefn>(op, parameter, returnType, stmts);
    }

    // member_decl - @ identifier : identifier [*]*
    std::shared_ptr<Node::MemberDecl> parseMemberDecl() {
        currentContext = "member declaration";
        
        discard(Token::memberSigil);
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);

        // pointer
        while (*it == Token::opMultiply) {
            type += consume(Token::opMultiply);
        }

        return std::make_shared<Node::MemberDecl>(name, type);
    }

    // method_def - def @ identifier ( param_list ) [: identifier [*]*] TERM func_body end
    std::shared_ptr<Node::MethodDefn> parseMethodDefn() {

        discard(Token::kwDef);
        discard(Token::memberSigil);
        std::string name = consume(Token::identifier);
        discard(Token::openParen);
        std::shared_ptr<Node::ParamList> parameters = nullptr;
        parameters = parseParamList();
        discard(Token::closeParen);

        std::string returnType = "nil";
        // non-void function
        if (*it != Token::terminator) {
            discard(Token::colon);
            returnType = consume(Token::identifier);

            // pointer type
            while (*it == Token::opMultiply) {
                returnType += consume(Token::opMultiply);
            }
        }

        discard(Token::terminator);
        auto functionBody = parseFunctionBody();
        discard(Token::kwEnd);

        return std::make_shared<Node::MethodDefn>(name, returnType, parameters, functionBody);  
    }

    // def identifier ( param_list ) [: identifier [*]*] TERM func_body end
    std::shared_ptr<Node::FunctionDefn> parseFunctionDefn() {
        currentContext = "function definition";

        discard(Token::kwDef);
        std::string name = consume(Token::identifier);
        discard(Token::openParen);
        std::shared_ptr<Node::ParamList> parameters = nullptr;
        parameters = parseParamList();
        discard(Token::closeParen);

        std::string returnType = "nil";
        // non-void function
        if (*it != Token::terminator) {
            discard(Token::colon);
            returnType = consume(Token::identifier);

            // pointer type
            while (*it == Token::opMultiply) {
                returnType += consume(Token::opMultiply);
            }
        }

        discard(Token::terminator);
        auto functionBody = parseFunctionBody();
        discard(Token::kwEnd);

        return std::make_shared<Node::FunctionDefn>(name, returnType, parameters, functionBody);
    }

    // param_list - [parameter [, parameter]*]
    std::shared_ptr<Node::ParamList> parseParamList() {
        std::vector<std::shared_ptr<Node::Parameter>> params = {};

        // empty
        if (*it == Token::closeParen || *it == Token::closeBracket) {
            return std::make_shared<Node::ParamList>(params);
        }

        // not empty, get first param
        params.push_back(parseParameter());

        // until ) or ], get any number of ', param'
        while (*it != Token::closeParen && *it != Token::closeBracket) {
            discard(Token::comma);
            params.push_back(parseParameter());
        }

        return std::make_shared<Node::ParamList>(params);
    }

    // identifier : identifier [*]*
    std::shared_ptr<Node::Parameter> parseParameter() {
        currentContext = "parameter";

        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);

        // pointer
        while (*it == Token::opMultiply) {
            type += consume(Token::opMultiply);
        }

        return std::make_shared<Node::Parameter>(name, type);
    }

    // determines if the token range is a valid assignment LHS
    // follows token pattern [@ | $] identifier [. identifier | [ logic_expr ]]* 
    bool isValidLHS(const std::vector<Token>::const_iterator begin, const std::vector<Token>::const_iterator end) {
        auto current = begin;
        
        // must begin with @, $ or identifier
        if (*current != Token::memberSigil && *current != Token::globalSigil && *current != Token::identifier) {
            return false;
        }
        
        // move to first identifier if @ or $
        if (*current == Token::memberSigil || *current == Token::globalSigil) {
            ++current;
        }

        // require identifier
        if (*current != Token::identifier) {
            return false;
        }
        ++current;

        // followed by any number of [ logic_expr ] or . identifier
        while (current != end) {
            // . identifier
            if (*current == Token::opDot && *(current+1) == Token::identifier) {
                current += 2;
            }

            // [ logic_expr ]
            else if (*current == Token::openBracket)  {
                // move to beginning of logical expr
                ++current;
                
                // use 'it' and a parsing method to validate the logical expr
                auto placeholder = it;
                it = current;
                parseLogicalExpr();
                current = it;
                it = placeholder;

                // verify closing ]
                if (*current != Token::closeBracket) {
                    return false;
                }
                ++current;
            }

            // otherwise, it is invalid
            else {
                return false;
            }
        }
        
        return true;
    }

    // assignment  -   access_expr = assgn_expr
    //             |   logic_expr
    std::shared_ptr<Node::Node> parseAssignment() {

        auto startLHS = it;
        std::shared_ptr<Node::Node> LHS = parseLogicalExpr();
        // assignment path
        if (*it == Token::opAssign) {
            discard(Token::opAssign);

            if (!isValidLHS(startLHS, it-1)) {
                std::cout << "Parser error on line " << startLHS->lineNumber << ". Invalid LHS in assignment.\n";
                exit(1);
            }
            auto RHS = parseLogicalExpr();

            return std::make_shared<Node::Assignment>(LHS, RHS); 
        }

        // logic_expr path
        return LHS;
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
                // only a DotRHS may follow a dot operation
                std::shared_ptr<Node::Node> RHS = parseDotRHS();
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

    // parseDotRHS
    std::shared_ptr<Node::DotRHS> parseDotRHS() {

        // @TODO: method call RHS
        std::string name = consume(Token::identifier);

        return std::make_shared<Node::DotRHS>(name);
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

        // member, method call
        else if (*it == Token::memberSigil) {

            // method call
            if (*(it+2) == Token::openParen) {
                return parseMethodCall();
            }

            // member
            return parseMember();
        }

        // global
        else if (*it == Token::globalSigil) {
            return parseGlobal();
        }

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

        // new_expr
        else if (*it == Token::kwNew) {
            return parseNewExpr();
        }

        // stack_expr
        else if (*it == Token::kwStack) {
            return parseStackExpr();
        }

        // heap_expr
        else if (*it == Token::kwHeap) {
            return parseHeapExpr();
        }

        else if (*it == Token::kwTrue || *it == Token::kwFalse) {
            std::string value = consume(*it);
            return std::make_shared<Node::BoolLiteral>(value);
        }

        else {
            std::cout << "Parser error on line " << it->lineNumber << ". Unrecognizable primary beginning with token " << it->toString() << ".\n";
            exit(1);
        }
    }

    // new_expr - new identifier ( expr_list )
    std::shared_ptr<Node::NewExpr> parseNewExpr() {

        discard(Token::kwNew);
        std::string type = consume(Token::identifier);
        discard(Token::openParen);
        auto args = parseExprList();
        discard(Token::closeParen);
        
        return std::make_shared<Node::NewExpr>(type, args);
    }

    // stack_expr - stack [ int_literal ] identifier [*]*
    std::shared_ptr<Node::StackExpr> parseStackExpr() {

        discard(Token::kwStack);
        discard(Token::openBracket);
        std::string number = consume(Token::intLiteral);
        discard(Token::closeBracket);
        std::string type = consume(Token::identifier);

        // pointer
        while (*it == Token::opMultiply) {
            type += consume(Token::opMultiply);
        }
        
        return std::make_shared<Node::StackExpr>(type, number);
    }

    // heap_expr - heap [ int_literal ] identifier [*]*
    std::shared_ptr<Node::HeapExpr> parseHeapExpr() {

        discard(Token::kwHeap);
        discard(Token::openBracket);
        std::string number = consume(Token::intLiteral);
        discard(Token::closeBracket);
        std::string type = consume(Token::identifier);
        
        // pointer
        while (*it == Token::opMultiply) {
            type += consume(Token::opMultiply);
        }
        
        return std::make_shared<Node::HeapExpr>(type, number);
    }

    // @ identifier
    std::shared_ptr<Node::Member> parseMember() {
        discard(Token::memberSigil);
        std::string name = consume(Token::identifier);

        return std::make_shared<Node::Member>(name);
    }

    // @ identifier ( expr_list )
    std::shared_ptr<Node::MethodCall> parseMethodCall() {
        discard(Token::memberSigil);
        std::string name = consume(Token::identifier);
        discard(Token::openParen);
        auto callArgs = parseExprList();
        discard(Token::closeParen);

        return std::make_shared<Node::MethodCall>(name, callArgs);
    }

    // func_name(expr_list)
    std::shared_ptr<Node::Call> parseCall() {
        std::string name = consume(Token::identifier);
        discard(Token::openParen);
        auto callArgs = parseExprList();
        discard(Token::closeParen);

        return std::make_shared<Node::Call>(name, callArgs);
    }

    // expr_list - [logic_expr [, logic_expr]*]
    std::shared_ptr<Node::ExprList> parseExprList() {
        std::vector<std::shared_ptr<Node::Node>> exprs = {};

        // empty
        if (*it == Token::closeParen || *it == Token::closeBracket) {
            return std::make_shared<Node::ExprList>(exprs);
        }

        // not empty, get first expr
        exprs.push_back(parseLogicalExpr());

        // until ) or ], get any number of , logical_expr
        while (*it != Token::closeParen && *it != Token::closeBracket) {
            discard(Token::comma);
            exprs.push_back(parseLogicalExpr());
        }

        return std::make_shared<Node::ExprList>(exprs);
    }

    // identifier:typename [*]* = expression
    std::shared_ptr<Node::VariableDefn> parseVariableDefn() {
        currentContext = "variable definition";
        
        std::string name = consume(Token::identifier);
        discard(Token::colon);
        std::string type = consume(Token::identifier);

        // pointer
        while (*it == Token::opMultiply) {
            type += consume(Token::opMultiply);
        }

        discard(Token::opAssign);
        auto expression = parseLogicalExpr();

        return std::make_shared<Node::VariableDefn>(name, type, expression);
    }

    // delete_stmt - delete [[ int_literal ]] identifier
    std::shared_ptr<Node::Delete> parseDelete() {

        discard(Token::kwDelete);
        std::string number = "0";
        if (*it == Token::openBracket) {
            discard(Token::openBracket);
            number = consume(Token::intLiteral);
            discard(Token::closeBracket);
        }
        std::string name = consume(Token::identifier);

        return std::make_shared<Node::Delete>(number, name);
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

    // while expr do TERM func_body end
    std::shared_ptr<Node::WhileLoop> parseWhileLoop() {
        currentContext = "while loop";

        discard(Token::kwWhile);
        auto expr = parseLogicalExpr();
        discard(Token::kwDo);
        discard(Token::terminator);
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
