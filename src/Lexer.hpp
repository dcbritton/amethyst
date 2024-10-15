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

        {"new", Token::kwNew},
        {"stack", Token::kwStack},
        {"heap", Token::kwHeap},
        {"unheap", Token::kwUnheap},
        {"op", Token::kwOp},
        // {"BEGIN", Token::kwBEGIN},
        // {"END", Token::kwEND},
        // {"alias", kwAlias},
        {"and", Token::kwAnd},
        // {"begin", Token::kwBegin},
        {"break", Token::kwBreak},
        {"continue", Token::kwContinue},
        // {"case", Token::kwCase},
        {"type", Token::kwType},
        {"def", Token::kwDef},
        // {"defined?", kwDefined?},
        {"do", Token::kwDo},
        {"else", Token::kwElse},
        {"elsif", Token::kwElsif},
        {"end", Token::kwEnd},
        // {"ensure", Token::kwEnsure},
        {"false", Token::kwFalse},
        {"for", Token::kwFor},
        {"if", Token::kwIf},
        // {"in", Token::kwIn},
        // {"module", kwModule},
        // {"next", Token::kwNext},
        {"nil", Token::kwNil},
        {"not", Token::kwNot},
        {"or", Token::kwOr},
        {"redo", Token::kwRedo},
        // {"rescue", kwRescue},
        // {"retry", kwRetry},
        {"return", Token::kwReturn},
        // {"self", Token::kwSelf},
        // {"super", kwSuper},
        {"then", Token::kwThen},
        {"true", Token::kwTrue},
        // {"undef", kwUndef},
        // {"unless", Token::kwUnless},
        // {"until", Token::kwUntil},
        // {"when", Token::kwWhen},
        {"while", Token::kwWhile},
        // {"__FILE__", kw__FILE__},
        // {"__LINE__", kw__LINE__}
    };

public:
    // constructor
    Lexer(const std::string& text)
        : text(text), it(text.begin()) {}

        bool isOpMinus() {
            // conditions that guarantee opMinus, anything else assumed negative number
            // last token is a straightforward determiner
            return (tokens.back() == Token::closeParen
                || tokens.back() == Token::closeBracket
                || tokens.back() == Token::identifier
                || tokens.back() == Token::intLiteral
                || tokens.back() == Token::floatLiteral
                || tokens.back() == Token::kwTrue
                || tokens.back() == Token::kwFalse
                || tokens.back() == Token::doubleQuoteString
                || tokens.back() == Token::charLiteral
                // @TODO:complete this condition
                // || tokens.back() == Token::
            );
        }

    // convert input into a token stream
    std::vector<Token> lex() {

        // traverse input characters
        while (it != text.end()) {

            // whitespace
            if (isspace(*it)) {

                // @NOTE: condense any number of sequential newlines into one token

                // windows newline crlf
                if (*it == '\r' && *(it+1) == '\n') {
                    while (isspace(*it)) {
                        if (*it == '\r' && *(it+1) == '\n') {
                            ++lineNumber;
                        }
                        ++it;
                    }
                    tokens.push_back(Token(Token::terminator, "\\n", lineNumber));
                    continue;
                }
                // *nix 
                else if (*it == '\n') {
                    while (isspace(*it)) {
                        ++lineNumber;
                    }
                    ++it;
                    tokens.push_back(Token(Token::terminator, "\\n", lineNumber));
                    continue;
                }

                // ignore other whitespace
                ++it;
                continue;
            }

            // comment
            else if (*it == '#') {
                // ignore until next newline
                while (*it != '\r' && *it != '\n') {
                    ++it;
                }

                // if the most recent token was a terminator, prevent another from being made after the comment by ignoring all whitespace
                if (tokens.back() == Token::terminator) {
                    while (isspace(*it)) {
                        ++lineNumber;
                        ++it;
                    }
                }
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

            // special case due to ambiguity between opMinus and a negative number
            // -
            else if (*it == '-' && isOpMinus()) {
                tokens.push_back(Token(Token::opMinus, "-", lineNumber));
                ++it;
                continue;
            }

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
                // if (*(it+1) == '.') {
                //     if(*(it+2) == '.') {
                //         tokens.push_back(Token(Token::opRangeExcl, "...", lineNumber));
                //         it += 3;
                //         continue;              
                //     }

                //     tokens.push_back(Token(Token::opRangeIncl, "..", lineNumber));
                //     it += 2;
                //     continue;
                // }

                tokens.push_back(Token(Token::opDot, ".", lineNumber));
                ++it;
                continue;
            }

            // =, ==, ===
            else if (*it == '=') {
                if (*(it+1) == '=') {
                    // if(*(it+2) == '=') {
                    //     tokens.push_back(Token(Token::opCaseEquality, "===", lineNumber));
                    //     it += 3;
                    //     continue;              
                    // }

                    tokens.push_back(Token(Token::opEquality, "==", lineNumber));
                    it += 2;
                    continue;
                }

                tokens.push_back(Token(Token::opAssign, "=", lineNumber));
                ++it;
                continue;
            }


            // +
            else if (*it == '+') {
                tokens.push_back(Token(Token::opPlus, "+", lineNumber));
                ++it;
                continue;
            }

            // *
            else if (*it == '*') {
                tokens.push_back(Token(Token::opMultiply, "*", lineNumber));
                ++it;
                continue;
            }

            // /
            else if (*it == '/') {
                tokens.push_back(Token(Token::opDivide, "/", lineNumber));
                ++it;
                continue;
            }

            // %
            else if (*it == '%') {
                tokens.push_back(Token(Token::opModulus, "%", lineNumber));
                ++it;
                continue;
            }

            // &
            else if (*it == '&') {

                tokens.push_back(Token(Token::opBitwiseAnd, "&", lineNumber));
                ++it;
                continue;
            }

            // |
            else if (*it == '|') {
                tokens.push_back(Token(Token::opBitwiseOr, "|", lineNumber));
                ++it;
                continue;
            }

            // ^
            else if (*it == '^') {
                tokens.push_back(Token(Token::opBitwiseXor, "^", lineNumber));
                ++it;
                continue;
            }

            // <, <=, <<
            else if (*it == '<') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opLessThanOrEqual, "<=", lineNumber));
                    it += 2;
                    continue;
                }
                if (*(it+1) == '<') {
                    tokens.push_back(Token(Token::opLeftShift, "<<", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opLessThan, "<", lineNumber));
                ++it;
                continue;
            }

            // >, >=, >>
            else if (*it == '>') {
                if (*(it+1) == '=') {
                    tokens.push_back(Token(Token::opGreaterThanOrEqual, ">=", lineNumber));
                    it += 2;
                    continue;
                }
                if (*(it+1) == '>') {
                    tokens.push_back(Token(Token::opRightShift, ">>", lineNumber));
                    it += 2;
                    continue;
                }
                tokens.push_back(Token(Token::opGreaterThan, ">", lineNumber));
                ++it;
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
                tokens.push_back(Token(Token::memberSigil, "@", lineNumber));
                ++it;
                continue;
            }

            // $
            else if (*it == '$') {
                tokens.push_back(Token(Token::globalSigil, "$", lineNumber));
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
