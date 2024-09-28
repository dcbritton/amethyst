// Token.hpp

#ifndef TOKEN
#define TOKEN

#include <string>

struct Token {

    enum Type {
        identifier,
        global,
        intLiteral,
        floatLiteral,
        singleQuoteString,
        doubleQuoteString,
        backtickString,

        // operators
        opNot,
        opDot,
        opEquality,
        opPlus,
        opMinus,
        opMultiply,
        opDivide,
        opModulus,
        opBitwiseAnd,
        opBitwiseOr,
        opBitwiseXor,
        opLeftShift,
        opRightShift,
        opNotEquals,
        opLessThan,
        opLessThanOrEqual,
        opGreaterThan,
        opGreaterThanOrEqual,
        
        opSpaceship,
        opRangeExcl,
        opRangeIncl,
        opCaseEquality,

        // assignments
        opAssign,

        // punctuation
        questionMark,
        exclamationPoint,
        comma,
        colon,
        terminator,
        openParen,
        closeParen,
        openBrace,
        closeBrace,
        openBracket,
        closeBracket,
        memberSigil,
        globalSigil,

        // reserved words
        kwNew,
        kwStack,
        kwHeap,
        kwDelete,
        kwOp,
        kwBEGIN,
        kwEND,
        // kwAlias,
        kwAnd,
        kwBegin,
        kwBreak,
        kwContinue,
        kwCase,
        kwType,
        kwDef,
        // kwDefined?,
        kwDo,
        kwElse,
        kwElsif,
        kwEnd,
        kwEnsure,
        kwFalse,
        kwFor,
        kwIf,
        kwIn,
        // kwModule,
        kwNext,
        kwNil,
        kwNot,
        kwOr,
        kwRedo,
        // kwRescue,
        // kwRetry,
        kwReturn,
        kwSelf,
        // kwSuper,
        kwThen,
        kwTrue,
        // kwUndef,
        kwUnless,
        kwUntil,
        kwWhen,
        kwWhile,
        // kw__FILE__,
        // kw__LINE__
    };

    // token's type
    Type type;

    // token's value (e.g. name of identifier, value of integer, etc.)
    std::string value = "";

    // line where the token was ecountered
    unsigned int lineNumber = 0;

    // constructor with only type
    Token(Type type)
        : type(type) {}

    // constructor
    Token(Type type, std::string value, unsigned int lineNumber)
        : type(type), value(value), lineNumber(lineNumber) {};

    // allow implicit conversion from Token to TokenType based on its member, 'type'; this is useful for switch statements and comparison
    operator Type() const {
        return type;
    }

    // disallow bool conversion
    operator bool() const = delete;

    // string representation of the token's type
    std::string toString() const {
        switch (type) {

            case identifier: return "identifier";
            case global: return "global";
            case intLiteral: return "intLiteral";
            case floatLiteral: return "floatLiteral";
            case singleQuoteString: return "singleQuoteString";
            case doubleQuoteString: return "doubleQuoteString";
            case backtickString: return "backtickString";

            // operators
            case opRangeExcl: return "opRangeExcl";
            case opRangeIncl: return "opRangeIncl";
            case opDot: return "opDot";
            case opCaseEquality: return "opCaseEquality";
            case opEquality: return "opEquality";
            case opAssign: return "opAssign";
            case opPlus: return "opPlus";
            case opMinus: return "opMinus";
            case opMultiply: return "opMultiply";
            case opDivide: return "opDivide";
            case opModulus: return "opModulus";
            case opBitwiseAnd: return "opBitwiseAnd";
            case opBitwiseOr: return "opBitwiseOr";
            case opBitwiseXor: return "opBitwiseXor";
            case opLeftShift: return "opLeftShift";
            case opRightShift: return "opRightShift";
            case opNot: return "opNot";
            case opNotEquals: return "opNotEquals";
            case opLessThan: return "opLessThan";
            case opLessThanOrEqual: return "opLessThanOrEqual";
            case opGreaterThan: return "opGreaterThan";
            case opGreaterThanOrEqual: return "opGreaterThanOrEqual";
            case opSpaceship: return "opSpaceship";
            case memberSigil: return "memberSigil";
            case globalSigil: return "globalSigil";

            // punctuation
            case questionMark: return "questionMark";
            case exclamationPoint: return "exclamationPoint";
            case comma: return "comma";
            case colon: return "colon";
            case terminator: return "terminator";
            case openParen: return "openParen";
            case closeParen: return "closeParen";
            case openBrace: return "openBrace";
            case closeBrace: return "closeBrace";
            case openBracket: return "openBracket";
            case closeBracket: return "closeBracket";

            // reserved words
            case kwNew: return "kwNew";
            case kwStack: return "kwStack";
            case kwHeap: return "kwHeap";
            case kwDelete: return "kwDelete";
            case kwOp: return "kwOp";
            case kwBEGIN: return "kwBEGIN";
            case kwEND: return "kwEND";
            // case kwAlias: return "kwAlias";
            case kwAnd: return "kwAnd";
            case kwBegin: return "kwBegin";
            case kwBreak: return "kwBreak";
            case kwContinue: return "kwContinue";
            case kwCase: return "kwCase";
            case kwType: return "kwType";
            case kwDef: return "kwDef";
            // case kwDefined?: return "kwDefined?";
            case kwDo: return "kwDo";
            case kwElse: return "kwElse";
            case kwElsif: return "kwElsif";
            case kwEnd: return "kwEnd";
            case kwEnsure: return "kwEnsure";
            case kwFalse: return "kwFalse";
            case kwFor: return "kwFor";
            case kwIf: return "kwIf";
            case kwIn: return "kwIn";
            // case kwModule: return "kwModule";
            case kwNext: return "kwNext";
            case kwNil: return "kwNil";
            case kwNot: return "kwNot";
            case kwOr: return "kwOr";
            case kwRedo: return "kwRedo";
            // case kwRescue: return "kwRescue";
            // case kwRetry: return "kwRetry";
            case kwReturn: return "kwReturn";
            case kwSelf: return "kwSelf";
            // case kwSuper: return "kwSuper";
            case kwThen: return "kwThen";
            case kwTrue: return "kwTrue";
            // case kwUndef: return "kwUndef";
            case kwUnless: return "kwUnless";
            case kwUntil: return "kwUntil";
            case kwWhen: return "kwWhen";
            case kwWhile: return "kwWhile";
            // case kw__FILE__: return "kw__FILE__";
            // case kw__LINE__: return "kw__LINE__";

            default: return "Unkown token!";
        }
    }
};

#endif
