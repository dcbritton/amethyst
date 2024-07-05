// main.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "DotVisitor.hpp"

int main(int argc, char** argv) {
    try {
        // get input
        std::ifstream input;
        std::stringstream s;
        std::string script;
        input.open("../examples/example.am");
        s << input.rdbuf();
        input.close();
        script = s.str();
        // std::cout << script + "\n";

        // lex
        Lexer l(script);
        auto tokens = l.lex();
        print(tokens);

        // parse
        Parser p(tokens);
        auto ast = p.parse();

        // DOT output
        DotVisitor v;
        ast->accept(std::make_shared<DotVisitor>(v));
    }
    catch (InvalidSymbolException& e) {
        std::cerr << "Lexer error: " << e.what() << '\n';
        return 1;
    }

    std::cout << "\nProgram has ended properly.\n";
    return 0;
}
