// Lexer.hpp

#ifndef LEXER
#define LEXER

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
#include <exception>
#include <bitset>
#include "Token.hpp"

// @TODO: bitset for valid characters
// const std::bitset<128> validMask("00000000000000000000000000000000111111111111111111111111111111100000001111111111011000000000000000000000000000000000000000000000");

class InvalidSymbolException final : public std::exception {
private:
    int lineNumber;
    std::string symbol;
    std::string message;

public:
    // constructor
    InvalidSymbolException(int lineNumber, std::string symbol)
        : lineNumber(lineNumber), symbol(symbol), message("Invalid symbol " + symbol + " on line " + std::to_string(lineNumber) + '.') {}; 
    
    // returns message as cstring
    const char* what() const noexcept override {
        return message.c_str();
    };
};

class Lexer {
private:
    int lineNumber = 1;
    const std::string& text;
    std::vector<Token> tokens = {};
    std::string::const_iterator it;
    std::unordered_map<std::string, Token::Type> keywordMap {
        // // {"String", kwString},
        // // {"Symbol", kwSymbol},
        // // {"Hash", kwHash},
        // // {"Array", kwArray},
        // // {"_int32", kw_int32},
        // // {"_int32", kw_float32},
        {"BEGIN", Token::kwBEGIN},
        {"END", Token::kwEND},
        // {"alias", kwAlias},
        {"and", Token::kwAnd},
        {"begin", Token::kwBegin},
        {"break", Token::kwBreak},
        {"case", Token::kwCase},
        {"class", Token::kwClass},
        {"def", Token::kwDef},
        // {"defined?", kwDefined?},
        {"do", Token::kwDo},
        {"else", Token::kwElse},
        {"elsif", Token::kwElsif},
        {"end", Token::kwEnd},
        {"ensure", Token::kwEnsure},
        {"false", Token::kwFalse},
        {"for", Token::kwFor},
        {"if", Token::kwIf},
        {"in", Token::kwIn},
        // {"module", kwModule},
        {"next", Token::kwNext},
        {"nil", Token::kwNil},
        {"not", Token::kwNot},
        {"or", Token::kwOr},
        {"redo", Token::kwRedo},
        // {"rescue", kwRescue},
        // {"retry", kwRetry},
        {"return", Token::kwReturn},
        {"self", Token::kwSelf},
        // {"super", kwSuper},
        {"then", Token::kwThen},
        {"true", Token::kwTrue},
        // {"undef", kwUndef},
        {"unless", Token::kwUnless},
        {"until", Token::kwUntil},
        {"when", Token::kwWhen},
        {"while", Token::kwWhile},
        // {"__FILE__", kw__FILE__},
        // {"__LINE__", kw__LINE__}
    };

public:
    // constructor
    Lexer(const std::string& text)
        : text(text), it(text.begin()) {}

    // convert input into a token stream
    std::vector<Token> lex() {

        // traverse input characters
        while (it != text.end()) {

            // whitespace
            if (isspace(*it)) {
                if (*it == '\n') {
                    ++lineNumber;
                    // tokens.push_back(Token(Token::terminator, "\\n", lineNumber));
                }
                ++it;
                continue;
            }

            // identifier or keyword
            else if (isalpha(*it) || *it == '_') {

                std::string::const_iterator wordEnd = it;
                while (isalnum(*wordEnd) || *wordEnd == '_') {        
                    ++wordEnd;
                }
                std::string word = std::string(it, wordEnd);

                // identifier found if not in keyword map
                std::unordered_map<std::string, Token::Type>::const_iterator possibleKeyword = keywordMap.find(word);
                if (possibleKeyword == keywordMap.end()) {
                    tokens.push_back(Token(Token::identifier, word, lineNumber));
                }
                // otherwise keyword
                else {
                    tokens.push_back(Token(possibleKeyword->second, word, lineNumber));
                }
                it = wordEnd;

                continue;
            }

            // global (restricting all globals to '$IDENTIFIER' for now)
            // @TODO: add option variables '$[-]any_char', listed at https://ruby-doc.org/docs/ruby-doc-bundle/Manual/man-1.4/variable.html#number
            else if (*it == '$') {
                std::string::const_iterator wordEnd = it+1;

                // report if the word following the $ is not an identifier
                if (!(isalpha(*(it+1)) || *(it+1) == '_')) {
                    while (!isspace(*wordEnd)) {
                        ++wordEnd;
                    }
                    throw InvalidSymbolException(lineNumber, std::string(it, wordEnd));
                }

                // lex an identifier as normal after the '$'
                while (isalnum(*wordEnd) || *wordEnd == '_') {        
                    ++wordEnd;
                }
                std::string word = std::string(it, wordEnd);

                // identifier found if not in keyword map
                std::unordered_map<std::string, Token::Type>::const_iterator possibleKeyword = keywordMap.find(word);
                if (possibleKeyword == keywordMap.end()) {
                    tokens.push_back(Token(Token::global, word, lineNumber));
                }
                // otherwise keyword
                else {
                    tokens.push_back(Token(possibleKeyword->second, word, lineNumber));
                }
                it = wordEnd;

                continue;
            }

            // @TODO fix bug here regarding - sign 
            // numeric
            else if (isdigit(*it) || *it == '-') {
                if (*it == '-' && !isdigit(*(it+1))) {
                    std::cout << "Tokenization error on line " << lineNumber << ". '-' must be followed by a digit.\n";
                    exit(1);
                }
                std::string::const_iterator numberEnd = it + 1;
                while (isdigit(*numberEnd))
                    numberEnd++;
                
                // float
                if (*numberEnd == '.') {

                    // float, b/c digit after .
                    ++numberEnd;
                    if (isdigit(*numberEnd)) {
                        
                        // expect only more digits
                        while (isdigit(*numberEnd))
                            numberEnd++;

                        std::string number(it, numberEnd);
                        tokens.push_back(Token(Token::floatLiteral, number, lineNumber));
                        it = numberEnd;
                    }

                    // otherwise, the '.' is a dot operator
                    else {
                        --numberEnd; // exclude the '.'
                        std::string number(it, numberEnd);
                        tokens.push_back(Token(Token::intLiteral, number, lineNumber));
                        it = numberEnd;
                    }
                    continue;
                }

                // int
                std::string number(it, numberEnd);
                try {
                    std::stoi(number);
                }
                catch (const std::out_of_range& e) {
                    std::cout << "Tokenization error on line " << lineNumber << ". The integer " << number << " out of 32 bit int range.\n";
                    exit(1);
                }

                tokens.push_back(Token(Token::intLiteral, number, lineNumber));
                it = numberEnd;
                continue;
            }

            // double quote string
            else if (*it == '\"') {
                std::string::const_iterator string_end = it;
                string_end++; // consume "
                while (*string_end != '\"' && string_end != text.end()) {
                    string_end++;
                }

                if (string_end == text.end()) {
                    std::cout << "Tokenizer error. Unpaired \" on line " << lineNumber << ".\n";
                    exit(1);
                }
                string_end++; // consume ending "
                
                // discard the quotes
                std::string string(it+1, string_end-1);
                tokens.push_back(Token(Token::doubleQuoteString, string, lineNumber));
                it = string_end;
            }

            // dot operator AND ranges
            else if (*it == '.') {
                if (*(it+1) == '.') {
                    if(*(it+2) == '.') {
                        tokens.push_back(Token(Token::opRangeExcl, "...", lineNumber));
                        it += 3;
                        continue;              
                    }

                    tokens.push_back(Token(Token::opRangeIncl, "..", lineNumber));
                    it += 2;
                    continue;
                }

                tokens.push_back(Token(Token::opDot, ".", lineNumber));
                ++it;
                continue;
            }

            // =, ==, ===
            else if (*it == '=') {
                if (*(it+1) == '=') {
                    if(*(it+2) == '=') {
                        tokens.push_back(Token(Token::opCaseEquality, "===", lineNumber));
                        it += 3;
                        continue;              
                    }

                    tokens.push_back(Token(Token::opEquality, "==", lineNumber));
                    it += 2;
                    continue;
                }

                tokens.push_back(Token(Token::opAssign, "=", lineNumber));
                ++it;
                continue;
            }


            // +, +=
            else if (*it == '+') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opPlusAssign, "+=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opPlus, "+", lineNumber));
                ++it;
                continue;
            }

            // -, -=
            else if (*it == '-') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opMinusAssign, "-=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opMinus, "-", lineNumber));
                ++it;
                continue;
            }

            // *, *=
            else if (*it == '*') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opMultiplyAssign, "*=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opMultiply, "*", lineNumber));
                ++it;
                continue;
            }

            // /, /=
            else if (*it == '/') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opDivideAssign, "/=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opDivide, "/", lineNumber));
                ++it;
                continue;
            }

            // %, %=
            else if (*it == '%') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opModulusAssign, "%=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opModulus, "%", lineNumber));
                ++it;
                continue;
            }

            // &, &=
            else if (*it == '&') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opBitwiseAndAssign, "&=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opBitwiseAnd, "&", lineNumber));
                ++it;
                continue;
            }

            // |, |=
            else if (*it == '|') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opBitwiseOrAssign, "|=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opBitwiseOr, "|", lineNumber));
                ++it;
                continue;
            }

            // ||, ||=
            else if (*it == '|') {
                if (*(it+1) == '|') {
                    if (*(it+2) == '=') {
                        tokens.push_back(Token(Token::opLogicalOrAssign, "||=", lineNumber));
                        it += 3;
                        continue;
                    }
                    tokens.push_back(Token(Token::opLogicalOr, "||", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opBitwiseOr, "|", lineNumber));
                ++it;
                continue;
            }

            // &&, &&=
            else if (*it == '&') {
                if (*(it+1) == '&') {
                    if (*(it+2) == '=') {
                        tokens.push_back(Token(Token::opLogicalAndAssign, "&&=", lineNumber));
                        it += 3;
                        continue;
                    }
                    tokens.push_back(Token(Token::opLogicalAnd, "&&", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opBitwiseAnd, "&", lineNumber));
                ++it;
                continue;
            }

            // ^, ^=
            else if (*it == '^') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opBitwiseXorAssign, "^=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opBitwiseXor, "^", lineNumber));
                ++it;
                continue;
            }

            // <, <=, <<, <<=
            else if (*it == '<') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opLessThanOrEqual, "<=", lineNumber));
                    it += 2;
                    continue;
                }
                if (*(it+1) == '<') {
                    if (*(it+2) == '=') {
                        tokens.push_back(Token(Token::opLeftShiftAssign, "<<=", lineNumber));
                        it += 3;
                        continue;
                    }
                    tokens.push_back(Token(Token::opLeftShift, "<<", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opLessThan, "<", lineNumber));
                ++it;
                continue;
            }

            // >, >=, >>, >>=
            else if (*it == '>') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opGreaterThanOrEqual, ">=", lineNumber));
                    it += 2;
                    continue;
                }
                if (*(it+1) == '>') {
                    if (*(it+2) == '=') {
                        tokens.push_back(Token(Token::opRightShiftAssign, ">>=", lineNumber));
                        it += 3;
                        continue;
                    }
                    tokens.push_back(Token(Token::opRightShift, ">>", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opGreaterThan, ">", lineNumber));
                ++it;
                continue;
            }

            // <=>
            else if (*it == '<' && *(it+1) == '=' && *(it+2) == '>') {
                tokens.push_back(Token(Token::opSpaceship, "<=>", lineNumber));
                it += 3;
                continue;
            }

            // ?
            else if (*it == '?') {
                tokens.push_back(Token(Token::questionMark, "?", lineNumber));
                ++it;
                continue;
            }

            // !
            else if (*it == '!') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opNotEquals, "!=", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::exclamationPoint, "!", lineNumber));
                ++it;
                continue;
            }

            // ,
            else if (*it == ',') {
                tokens.push_back(Token(Token::comma, ",", lineNumber));
                ++it;
                continue;         
            }

            // ;
            else if (*it == ';') {
                tokens.push_back(Token(Token::terminator, ";", lineNumber));
                ++it;
                continue;
            }

            // (
            else if (*it == '(') {
                tokens.push_back(Token(Token::openParen, "(", lineNumber));
                ++it;
                continue;
            }

            // )
            else if (*it == ')') {
                tokens.push_back(Token(Token::closeParen, ")", lineNumber));
                ++it;
                continue;
            }

            // {
            else if (*it == '{') {
                tokens.push_back(Token(Token::openBrace, "{", lineNumber));
                ++it;
                continue;
            }

            // }
            else if (*it == '}') {
                tokens.push_back(Token(Token::closeBrace, "}", lineNumber));
                ++it;
                continue;
            }

            // [
            else if (*it == '[') {
                tokens.push_back(Token(Token::openBracket, "[", lineNumber));
                ++it;
                continue;
            }

            // ]
            else if (*it == ']') {
                tokens.push_back(Token(Token::closeBracket, "]", lineNumber));
                ++it;
                continue;
            }

            // :
            else if (*it == ':') {
                tokens.push_back(Token(Token::colon, ":", lineNumber));
                ++it;
                continue;
            }

            // @
            else if (*it == '@') {
                tokens.push_back(Token(Token::at, "@", lineNumber));
                ++it;
                continue;
            }

            else {
                throw InvalidSymbolException(lineNumber, std::string(1, *it));
            }
        }

        return tokens;
    }

};

// output token information for testing
void print(const std::vector<Token>& tokens) {

    std::cout << "Token stream:\n-------------\n";
    std::cout << "Values: ";
    for (auto x : tokens) 
        std::cout << x.value << ' ';
    std::cout << '\n';
    std::cout << "Types: ";
    for (auto x : tokens) 
        std::cout << x.type << ' ';
    std::cout << "\n";
    std::cout << "Type names: ";
    for (auto x : tokens) 
        std::cout << x.toString() << ' ';
    std::cout << "\n\n";
}


#endif