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
        opRangeExcl,
        opRangeIncl,
        opDot,
        opCaseEquality,
        opEquality,
        opAssign,
        opPlus,
        opPlusAssign,
        opMinus,
        opMinusAssign,
        opMultiply,
        opMultiplyAssign,
        opDivide,
        opDivideAssign,
        opModulus,
        opModulusAssign,
        opBitwiseAnd,
        opBitwiseAndAssign,
        opBitwiseOr,
        opBitwiseOrAssign,
        opLogicalOr,
        opLogicalOrAssign,
        opLogicalAnd,
        opLogicalAndAssign,
        opBitwiseXor,
        opBitwiseXorAssign,
        opLeftShift,
        opLeftShiftAssign,
        opRightShift,
        opRightShiftAssign,
        opNot,
        opNotEquals,
        opLessThan,
        opLessThanOrEqual,
        opGreaterThan,
        opGreaterThanOrEqual,
        opSpaceship,

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

        // reserved words
        // // kw_int32,
        // // kw_float32,
        // // kwString,
        // // kwArray,
        // // kwSymbol,
        // // kwHash,
        kwBEGIN,
        kwEND,
        // kwAlias,
        kwAnd,
        kwBegin,
        kwBreak,
        kwCase,
        kwClass,
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
            case opPlusAssign: return "opPlusAssign";
            case opMinus: return "opMinus";
            case opMinusAssign: return "opMinusAssign";
            case opMultiply: return "opMultiply";
            case opMultiplyAssign: return "opMultiplyAssign";
            case opDivide: return "opDivide";
            case opDivideAssign: return "opDivideAssign";
            case opModulus: return "opModulus";
            case opModulusAssign: return "opModulusAssign";
            case opBitwiseAnd: return "opBitwiseAnd";
            case opBitwiseAndAssign: return "opBitwiseAndAssign";
            case opBitwiseOr: return "opBitwiseOr";
            case opBitwiseOrAssign: return "opBitwiseOrAssign";
            case opLogicalOr: return "opLogicalOr";
            case opLogicalOrAssign: return "opLogicalOrAssign";
            case opLogicalAnd: return "opLogicalAnd";
            case opLogicalAndAssign: return "opLogicalAndAssign";
            case opBitwiseXor: return "opBitwiseXor";
            case opBitwiseXorAssign: return "opBitwiseXorAssign";
            case opLeftShift: return "opLeftShift";
            case opLeftShiftAssign: return "opLeftShiftAssign";
            case opRightShift: return "opRightShift";
            case opRightShiftAssign: return "opRightShiftAssign";
            case opNot: return "opNot";
            case opNotEquals: return "opNotEquals";
            case opLessThan: return "opLessThan";
            case opLessThanOrEqual: return "opLessThanOrEqual";
            case opGreaterThan: return "opGreaterThan";
            case opGreaterThanOrEqual: return "opGreaterThanOrEqual";
            case opSpaceship: return "opSpaceship";

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
            // // case kw_int32: return "kw_int32";
            // // case kw_float32: return "kw_float32";
            // // case kwString: return "kwString";
            // // case kwArray: return "kwArray";
            // // case kwSymbol: return "kwSymbol";
            // // case kwHash: return "kwHash";
            case kwBEGIN: return "kwBEGIN";
            case kwEND: return "kwEND";
            // case kwAlias: return "kwAlias";
            case kwAnd: return "kwAnd";
            case kwBegin: return "kwBegin";
            case kwBreak: return "kwBreak";
            case kwCase: return "kwCase";
            case kwClass: return "kwClass";
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
