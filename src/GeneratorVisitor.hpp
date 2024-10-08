// GeneratorVisitor.hpp

#ifndef GENERATORVISITOR
#define GENERATORVISITOR

#include <fstream>
#include <unordered_map>
#include "Visitor.hpp"

struct GeneratorVisitor : Visitor {

    // output file
    std::ofstream out;

    // used to track register number
    uint32_t currentRegister = 0;

    // used to track break label names
    uint32_t currentLabelNumber = 0;

    std::unique_ptr<Type> currentType = nullptr;

    // maps amethyst non-pointer type name to an llvm type name
    std::unordered_map<std::string, std::string> typeMap {
        {"int", "i64"},
        {"float", "float"},
        {"bool", "i1"},
        {"nil", "void"}

        // @TODO: add char
    };

    // types map
    std::unique_ptr<std::unordered_map<std::string, Type>> types;

    // associates variable names to register numbers
    std::unordered_map<std::string, uint32_t> nameToRegister;

    // is the current expression being evaluated an "lvalue" ?
    // or, are we in something that needs to return an for storing in assignment
    bool rootOfLhsOfAssignment = false;

    struct SubExprInfo {
        const uint32_t reg;
        std::string type;
    };

    // stores registers and types of subexpressions
    std::vector<SubExprInfo> exprStack;

    // constructor
    GeneratorVisitor(const std::string& filename) {
        out.open(filename);
    }

    // destructor
    ~GeneratorVisitor() {
        out.close();
    }

    bool isPrimitive(const std::string& typeName) {
        return typeName == "int" || typeName == "float" || typeName == "bool" || typeName == "nil" || typeName.back() == '*';
    }

    // convert an amethyst type (including pointer types)
    std::string convertType(const std::string amethystType) {
        // remove *'s in order to find if non-ptr type exists
        std::string llvmType = typeMap[amethystType];
        auto it = amethystType.begin();
        while (it != amethystType.end()) {
            if (*it == '*') {
                llvmType = typeMap[std::string(amethystType.begin(), it)];
                break;
            }
            ++it;
        }
        // add *'s back
        llvmType += std::string(it, amethystType.end());

        return llvmType;
    }

    // methods for output of certain statement types

    // output an alloca instuction
    void allocation(const std::string& type) {
        out << "  %" << currentRegister
            << " = alloca " << convertType(type)
            << "\n";
        ++currentRegister;
    }

    // output a load instruction
    void store(const int fromReg, const int toReg, const std::string& type) {
        out << "  store " << convertType(type)
            << " %" << fromReg << ", "
            << convertType(type) << "*"
            << " %" << toReg
            << "\n";
    }

    // load
    void load(const int fromReg, const std::string& type) {
        out << "  %" << currentRegister
            << " = load "  << convertType(type) << ", "
            << convertType(type) << "*"
            << " %" << fromReg
            << "\n";
        ++currentRegister;
    }

    void visit(std::shared_ptr<Node::Node> n) override {}

    void visit(std::shared_ptr<Node::Program> n) override {
        
        // move types
        types = std::move(n->types);

        for (auto it = types->begin(); it != types->end(); ++it) {
            if (!isPrimitive(it->first)) {
                typeMap[it->first] = "%struct." + it->first;
            }
        }

        // visit children
        for (const auto& definition : n->definitions) {
            definition->accept(shared_from_this());
            out << "\n";
        }

        // @TODO: declarations for llvm intrinsics
        out << "; Declarations of llvm intrinstics, may be unused\n";
        out << "declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)\n";
        out << "declare noalias i8* @malloc(i64 noundef)\n";
        out << "declare void @free(i8* noundef)\n";
    }

    // global decl
    void visit(std::shared_ptr<Node::GlobalDecl> n) override {
        out << "@.global." << n->name
            << " = dso_local global " << convertType(n->type)
            << " zeroinitializer";
    }

    // global
    void visit(std::shared_ptr<Node::Global> n) override {
        
        auto globalRegisterCopy = currentRegister;
        ++currentRegister;
        
        // get a copy of the global's regiister by bitcasting to its own type
        out << "  %" << globalRegisterCopy
            << " = bitcast " << convertType(n->type + "*")
            << " @.global." << n->name
            << " to " << convertType(n->type + "*")
            << " ; Workaround to use globals in current register management system\n";

        // structs always push their allocation pointer and their type
        if (!isPrimitive(n->type)) {
            exprStack.push_back({globalRegisterCopy, n->type});
            return;
        }

        // primitives
        // if the assignment is literally `name = expression`, with no . or [] on lhs
        if (rootOfLhsOfAssignment) {
            // give the assignment the register of the variable
            exprStack.push_back({globalRegisterCopy, n->type});  
        }
        else {
            // register & type stack
            exprStack.push_back({currentRegister, n->type});
            // load
            load(globalRegisterCopy, n->type);
        }
    }

    void visit(std::shared_ptr<Node::Statement> n) override {}

    void visit(std::shared_ptr<Node::FunctionDefn> n) override {
        // type and name
        out << "define dso_local " << convertType(n->returnType)
            << " @" << n->info->signature << "(";

        // parameters
        for (auto it = n->info->parameters.begin(); it != n->info->parameters.end(); ++it) {
            // primitive parameter
            if (isPrimitive(it->type)) {
                out << convertType(it->type) << " noundef %" << currentRegister;
                nameToRegister.emplace(it->name, currentRegister);
                ++currentRegister;
            }
            // struct parameter (pass by value)
            else {
                out << convertType(it->type + "*") << " noundef byval("
                    << convertType(it->type) << ") %"
                    << currentRegister;
                nameToRegister.emplace(it->name, currentRegister);
                ++currentRegister;
            }

            if (it != n->info->parameters.end() - 1) {
                out << ", ";
            }
        }
        out << ") {\n";

        // allocations & stores for parameters
        out << "  ; Primitive parameter allocations and stores\n";
        ++currentRegister;
        for (const auto& parameter : n->info->parameters) {
            if (isPrimitive(parameter.type)) {
                allocation(parameter.type);
                store(nameToRegister[parameter.name], currentRegister-1, parameter.type);
                nameToRegister[parameter.name] = currentRegister-1;
            }
        }
        out << "  ; End parameter handling\n";
        
        n->functionBody->accept(shared_from_this());
        out << "}\n";

        // reset current procedure
        n->info.reset();

        // reset registers and nameToRegister map after each function
        currentRegister = 0;
        currentLabelNumber = 0;
        nameToRegister.clear();
    }

    void visit(std::shared_ptr<Node::ParamList> n) override {}

    void visit(std::shared_ptr<Node::Parameter> n) override {}

    void visit(std::shared_ptr<Node::FunctionBody> n) override {
        for (const auto& statement :n->statements) {
            statement->accept(shared_from_this());
        }
    }

    void visit(std::shared_ptr<Node::LogicalExpr> n) override {}

    void visit(std::shared_ptr<Node::EqualityExpr> n) override {
        out << "  ; Begin eq expr\n";

        // process children
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get child registers and type
        SubExprInfo rhs = exprStack.back();
        exprStack.pop_back();
        SubExprInfo lhs = exprStack.back();
        exprStack.pop_back();

        // get type
        // @TODO: check overloads for structs
        std::string type;
        if (lhs.type == "int" && rhs.type == "int") {
            type = "bool";
        }
        else {
            
        }

        // icmp eq/ne
        out << "  %" << currentRegister 
            << " = icmp "
            << (n->op == "==" ? "eq " : "ne ") 
            << convertType(lhs.type)
            << " %" << lhs.reg
            << ", %" << rhs.reg
            << "\n";

        exprStack.push_back({currentRegister, type});

        ++currentRegister;

        out << "  ; End eq expr\n";
    }

    void visit(std::shared_ptr<Node::RelationExpr> n) override {
        // @TODO: icmp <arg> ... - check reference search sgtx  
    }

    void visit(std::shared_ptr<Node::ShiftExpr> n) override {}

    void visit(std::shared_ptr<Node::AdditionExpr> n) override {
        out << "  ; Begin add expr\n";

        // process children
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get child registers and type
        SubExprInfo rhs = exprStack.back();
        exprStack.pop_back();
        SubExprInfo lhs = exprStack.back();
        exprStack.pop_back();

        // get type
        // @TODO: check overloads for structs
        std::string type;
        if (lhs.type == "int" && rhs.type == "int") {
            type = "int";
        }
        else {
            
        }

        // add / sub
        out << "  %" << currentRegister 
            << " = " 
            << (n->op == "+" ? "add " : "sub ")
            << convertType(type)
            << " %" << lhs.reg
            << ", %" << rhs.reg
            << "\n";

        exprStack.push_back({currentRegister, type});

        ++currentRegister;

        out << "  ; End add expr\n";  
    }

    void visit(std::shared_ptr<Node::MultiplicationExpr> n) override {
        out << "  ; Begin mult expr\n";

        // process children
        n->LHS->accept(shared_from_this());
        n->RHS->accept(shared_from_this());

        // get child registers and type
        SubExprInfo rhs = exprStack.back();
        exprStack.pop_back();
        SubExprInfo lhs = exprStack.back();
        exprStack.pop_back();

        // get type
        // @TODO: check overloads for structs
        std::string type;
        if (lhs.type == "int" && rhs.type == "int") {
            type = "int";
        }
        else {
            
        }

        // mul
        out << "  %" << currentRegister 
            << " = " 
            << (n->op == "*" ? "mul " : "sdiv ")
            << convertType(type)
            << " %" << lhs.reg
            << ", %" << rhs.reg
            << "\n";

        exprStack.push_back({currentRegister, type});

        ++currentRegister;

        out << "  ; End mult expr\n";
    }

    void visit(std::shared_ptr<Node::AccessExpr> n) override {

        // subscript
        if (n->op == "[]") {
            
            // root of LHS of assignment? set false, because no children can be
            bool outerLHS = rootOfLhsOfAssignment;
            rootOfLhsOfAssignment = false;

            // process lhs & get type
            n->LHS->accept(shared_from_this());
            auto lhs = exprStack.back();
            exprStack.pop_back();

            // is ptr?
            if (lhs.type.back() == '*') {
                // evaluate expression in subscript
                n->RHS->accept(shared_from_this());
                auto rhs = exprStack.back();
                exprStack.pop_back();

                // get ptr to index
                std::string dereferencedType = lhs.type.substr(0, lhs.type.length()-1);
                out << "  %" << currentRegister
                    << " = getelementptr " << convertType(dereferencedType)
                    << ", " << convertType(lhs.type)
                    << " %" << lhs.reg
                    << ", i64 %" << rhs.reg
                    << "\n";
                ++currentRegister;

                // outer lhs means a pointer for storing in assignment
                if (!outerLHS && isPrimitive(dereferencedType)) {
                    // load value at that index into new register
                    load(currentRegister-1, dereferencedType);
                }

                // push back a dereferenced LHS type
                exprStack.push_back({currentRegister-1, dereferencedType});
            }
            // otherwise, use overloads
            else {
                // @TODO: subscript overloads
            }
        }
        // dot operator
        else if (n->op == ".") {    

            // root of LHS of assignment? set false, because no children can be
            bool dotRootOfLhsOfAssignment = rootOfLhsOfAssignment;
            rootOfLhsOfAssignment = false;
            
            // process lhs & get type
            n->LHS->accept(shared_from_this());
            auto lhs = exprStack.back();
            // dont pop_back() b/c DotRHS needs the lhs' info
            // popped in DotRHS

            // @TODO: redundant? should be caught in semantic analysis
            if (lhs.type.back() == '*') {
                std::cout << "Internal error. Code generator found a dot operator with a pointer LHS.\nThis ought to have been caught in semantic analysis.\n";
                exit(1);
            }
            // @TODO: redundant? should be caught in semantic analysis
            if (isPrimitive(lhs.type)) {
                std::cout << "Internal error. Code generator found a dot access with a primitive-type LHS.\nThis ought to have been caught in semantic analysis.\n";
                exit(1);
            }

            // process DotRHS
            n->RHS->accept(shared_from_this());
            auto rhs = exprStack.back();

            // if a dot expr results in a primitive, it should always load
            // UNLESS it is the dot root of the lhs of an assignment
            if (!dotRootOfLhsOfAssignment && isPrimitive(rhs.type)) {
                // load value at that index into new register
                load(currentRegister-1, rhs.type);
                exprStack.pop_back();
                exprStack.push_back({currentRegister-1, rhs.type});
            }
        }

        else {
            std::cout << "Error. Code generator reached an access expression that was neither a dot operator nor subscript.\n";
            exit(1);
        }
    }

    void visit(std::shared_ptr<Node::DotRHS> n) override {
        // get lhs info
        auto lhs = exprStack.back();
        exprStack.pop_back();
        auto offset = (*types)[lhs.type].members.find(n->name)->second.offset;

        out << "  %" << currentRegister
            << " = getelementptr inbounds " << convertType(lhs.type)
            << ", " << convertType(lhs.type)
            << "* %" << lhs.reg
            << ", i32 0, i32 " << offset
            << " ; Getting ptr to member\n";
        ++ currentRegister;

        // push a register to allocation to expr stack
        exprStack.push_back({currentRegister-1, n->type});
    }

    void visit(std::shared_ptr<Node::Primary> n) override {}

    void visit(std::shared_ptr<Node::Array> n) override {}

    void visit(std::shared_ptr<Node::NewExpr> n) override {

        uint32_t placeholderRegister = currentRegister;
        ++currentRegister;
        out << "  %" << placeholderRegister
            << " = alloca " << convertType(n->type)
            << " ; Placeholder allocating space for struct return\n";
            
        // visit args
        n->args->accept(shared_from_this());

        // @NOTE:violation of visitor pattern
        int numArgs = n->args->exprs.size();

        // output call
        out << "  call " << convertType("nil")
            << " @" << n->signature << "(";
        for (auto argIt = exprStack.end() - numArgs; argIt != exprStack.end(); ++argIt) {
            if (isPrimitive(argIt->type)) {
                out << convertType(argIt->type) << " noundef %" << argIt->reg << ", ";
            }
            else {
                out << convertType(argIt->type + "*") << " noundef byval("
                    << convertType(argIt->type) << ") %"
                    << argIt->reg << ", ";
            }
        }
        // secret sret arg
        out << convertType(n->type + "*") << " sret("
            << convertType(n->type) << ") %"
            << placeholderRegister << ")\n";

        // clear the top of the args from the expr stack
        for (int i = 0; i < numArgs; ++i) {
            exprStack.pop_back();
        }

        // add this to the exprStack
        exprStack.push_back({placeholderRegister, n->type});
    }

    void visit(std::shared_ptr<Node::StackExpr> n) override {
        // allocate array
        std::string arrayType = "[" + n->number + " x " + convertType(n->type) + "]";
        out << "  %" << currentRegister
            << " = alloca " << arrayType
            << "\n";
        ++currentRegister;

        // bitcast array type to ptr type
        std::string ptrType = convertType(n->type) + "*";
        out << "  %" << currentRegister
            << " = bitcast " << arrayType 
            << "* %" << currentRegister-1
            << " to " << convertType(n->type + "*")
            << "\n";

        ++currentRegister;

        exprStack.push_back({currentRegister-1, n->type + "*"});
    }

    void visit(std::shared_ptr<Node::HeapExpr> n) override {

        // determine size of n->type
        out << "  %" << currentRegister
            << " = getelementptr " << convertType(n->type)
            << ", " << convertType(n->type)
            << "* null, i32 1\n";
        ++currentRegister;

        uint32_t sizeRegister = currentRegister;
        ++currentRegister;
        out << "  %" << sizeRegister
            << " = ptrtoint " << convertType(n->type)
            << "* %" << sizeRegister-1
            << " to i64\n";

        // get the number determined by n->expr
        n->expr->accept(shared_from_this());
        uint32_t numberToAllocateRegsiter = exprStack.back().reg;
        exprStack.pop_back();
        
        // multiply to determine how many bytes are needed
        out << "  %" << currentRegister
            << " = mul i64 %" << sizeRegister
            << ", %" << numberToAllocateRegsiter
            << "\n";
        ++currentRegister;

        // malloc i8*, the correct number of bytes
        out << "  %" << currentRegister
            << " = call noalias i8* @malloc(i64 noundef %"
            << currentRegister-1 << ")\n";
        ++currentRegister;

        // cast to correct pointer type
        out << "  %" << currentRegister
            << " = bitcast i8* %" << currentRegister-1
            << " to " << convertType(n->type)
            << "*\n";
        ++currentRegister;

        // put pointer on expr stack
        exprStack.push_back({currentRegister-1, n->type + "*"});
    }

    void visit(std::shared_ptr<Node::IntLiteral> n) override {
        // @NOTE: all int literals are i64
        // put value in register by adding to 0
        out << "  %" << currentRegister
            << " = add i64 0, " << n->value << "\n";
        // register & type stack
        exprStack.push_back({currentRegister, "int"});
        ++currentRegister;
    }

    void visit(std::shared_ptr<Node::FloatLiteral> n) override {}

    void visit(std::shared_ptr<Node::StringLiteral> n) override {}

    void visit(std::shared_ptr<Node::BoolLiteral> n) override {
        // @NOTE: all int literals are i64
        // put value in register by adding to 0
        out << "  %" << currentRegister
            << " = add i1 0, "
            << (n->value == "true" ? "1" : "0")
            << "\n";
        // register & type stack
        exprStack.push_back({currentRegister, "bool"});
        ++currentRegister;
    }
    
    void visit(std::shared_ptr<Node::Variable> n) override {

        // structs always push their allocation pointer and their type
        if (!isPrimitive(n->type)) {
            exprStack.push_back({nameToRegister[n->name], n->type});
            return;
        }

        // primitives
        // if the assignment is literally `name = expression`, with no . or [] on lhs
        if (rootOfLhsOfAssignment) {
            // give the assignment the register of the variable
            exprStack.push_back({nameToRegister[n->name], n->type});   
        }
        else {
            // register & type stack
            exprStack.push_back({currentRegister, n->type});
            // load
            load(nameToRegister[n->name], n->type);
        }
    }

    void visit(std::shared_ptr<Node::ExprList> n) override {
        for (const auto& expr : n->exprs) {
            expr->accept(shared_from_this());
        }
    }

    void visit(std::shared_ptr<Node::Call> n) override {
        // process args
        n->args->accept(shared_from_this());
        // top of expr stack now contains types and registers for arg results

        // output call
        out << "  %" << currentRegister
            << " = call " << convertType(n->type)
            << " @" << n->signature
            << "(";

        // output args
        for (auto argIt = exprStack.end() - n->numArgs; argIt != exprStack.end(); ++argIt) {
            // for primitive arg
            if (isPrimitive(argIt->type)) {
                out << convertType(argIt->type) << " noundef %" << argIt->reg;
            }
            // for struct args
            else {
                out << convertType(argIt->type + "*") << " noundef byval("
                    << convertType(argIt->type) << ") %"
                    << argIt->reg;
            }

            if (argIt != exprStack.end()-1) {
                out << ", ";
            }
        }
        out << ")\n";

        // clear the top of the args from the expr stack
        for (int i = 0; i < n->numArgs; ++i) {
            exprStack.pop_back();
        }

        // add this call to the expr stack
        exprStack.push_back({currentRegister, n->type});

        ++currentRegister;
    }

    void visit(std::shared_ptr<Node::VariableDefn> n) override {
        out << "\n  ; Define " << n->name << ":" << n->type << "\n";

        nameToRegister[n->name] = currentRegister;
        allocation(n->type);
        n->expression->accept(shared_from_this());

        if (isPrimitive(exprStack.back().type)) {
            store(exprStack.back().reg, nameToRegister[n->name], n->type);
        }
        // type (llvm struct)
        else {
            // determine size of struct by pretending there is an instance at address 0 (null)
            out << "  %" << currentRegister
                << " = getelementptr " << convertType(n->type)
                << ", " << convertType(n->type)
                << "* null, i32 1\n";
            ++currentRegister;

            uint32_t sizeRegister = currentRegister;
            ++currentRegister;
            out << "  %" << sizeRegister
                << " = ptrtoint " << convertType(n->type)
                << "* %" << sizeRegister-1
                << " to i64\n";

            // cast lhs allocation to i8*
            out << "  %" << currentRegister
                << " = bitcast " << convertType(n->type)
                << "* %" << nameToRegister[n->name]
                << " to i8*\n";
            ++currentRegister;

            // cast rhs allocation to i8*
            out << "  %" << currentRegister
                << " = bitcast " << convertType(n->type)
                << "* %" << exprStack.back().reg
                << " to i8*\n";
            ++currentRegister;

            // memcpy rhs to lhs
            out << "  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %" << currentRegister-2
                << ", i8* %" << currentRegister-1
                << ", i64 %" << sizeRegister
                << ", i1 false)\n";
        }

        // clear expression stack
        exprStack.pop_back();

        out << "  ; End definition of " << n->name << ":" << n->type << "\n";
    }

    void visit(std::shared_ptr<Node::Assignment> n) override {
        // evaluate rhs
        n->RHS->accept(shared_from_this());
        SubExprInfo rhs = exprStack.back();
        exprStack.pop_back();

        // evaluate lhs
        rootOfLhsOfAssignment = true;
        n->LHS->accept(shared_from_this());
        rootOfLhsOfAssignment = false;
        SubExprInfo lhs = exprStack.back();
        exprStack.pop_back();

        // primitive, store
        if (isPrimitive(rhs.type)) {
            store(rhs.reg, lhs.reg, lhs.type);
        }
        // struct/type memcpy
        else {
            // determine size of struct by pretending there is an instance at address 0 (null)
            out << "  %" << currentRegister
                << " = getelementptr " << convertType(rhs.type)
                << ", " << convertType(rhs.type)
                << "* null, i32 1\n";
            ++currentRegister;

            uint32_t sizeRegister = currentRegister;
            ++currentRegister;
            out << "  %" << sizeRegister
                << " = ptrtoint " << convertType(rhs.type)
                << "* %" << sizeRegister-1
                << " to i64\n";

            // cast lhs allocation to i8*
            out << "  %" << currentRegister
                << " = bitcast " << convertType(rhs.type)
                << "* %" << lhs.reg
                << " to i8*\n";
            ++currentRegister;

            // cast rhs allocation to i8*
            out << "  %" << currentRegister
                << " = bitcast " << convertType(rhs.type)
                << "* %" << rhs.reg
                << " to i8*\n";
            ++currentRegister;

            // memcpy rhs to lhs
            out << "  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %" << currentRegister-2
                << ", i8* %" << currentRegister-1
                << ", i64 %" << sizeRegister
                << ", i1 false)\n";
        }
    }

    void visit(std::shared_ptr<Node::Return> n) override {
        // non-void
        if (n->expr) {
            // process expression
            n->expr->accept(shared_from_this());
            SubExprInfo expr = exprStack.back();
            exprStack.pop_back();
            out << "  ret " << convertType(expr.type)
                << " %" << expr.reg << "\n";
        }
        // void
        else {
            out << "  ret void\n";
        }
    }

    void visit(std::shared_ptr<Node::TypeDefn> n) override {
        currentType = std::make_unique<Type>((*types)[n->name]);
        
        // declare struct
        // @NOTE: violates visitor pattern
        out << "%" << "struct." << n->name << " = type { ";
        for (const auto& member : n->members) {
            member->accept(shared_from_this());
            if (member != n->members.back()) {
                out << ", ";
            }
        }
        out  << " }\n";

        // define constructors
        for (const auto& constructor : n->constructors) {
            constructor->accept(shared_from_this());
        }

        // @TODO: method definitions

        // @TODO: operator overloads

        currentType.reset();
    }

    // constructor
    void visit(std::shared_ptr<Node::ConstructorDefn> n) override {
        
        std::string thisType = currentType->name +  "*";

        // type and name
        out << "define dso_local void @" << n->info->signature << "(";

        // parameters
        for (auto it = n->info->parameters.begin(); it != n->info->parameters.end(); ++it) {
            // primitive parameter
            if (isPrimitive(it->type)) {
                out << convertType(it->type) << " noundef %" << currentRegister;
                nameToRegister.emplace(it->name, currentRegister);
                ++currentRegister;
            }
            // struct parameter (pass by value)
            else {
                out << convertType(it->type + "*") << " noundef byval("
                    << convertType(it->type) << ") %"
                    << currentRegister;
                nameToRegister.emplace(it->name, currentRegister);
                ++currentRegister;
            }
            
            out << ", ";
        }
        
        // secret pointer to struct parameter
        out << convertType(thisType) << " noalias sret(" << convertType(currentType->name) << ") %" << currentRegister;
        nameToRegister.emplace("@this", currentRegister);
        ++currentRegister;

        out << ") {\n";

        // allocations & stores for parameters
        out << "  ; Primitive parameter allocations and stores\n";
        ++currentRegister;
        for (const auto& parameter : n->info->parameters) {
            if (isPrimitive(parameter.type)) {
                allocation(parameter.type);
                store(nameToRegister[parameter.name], currentRegister-1, parameter.type);
                nameToRegister[parameter.name] = currentRegister-1;
            }
        }
        out << "  ; End parameter handling\n";
        
        n->body->accept(shared_from_this());
        out << "}\n";

        // reset registers and nameToRegister map after each function
        currentRegister = 0;
        currentLabelNumber = 0;
        nameToRegister.clear();
    }

    void visit(std::shared_ptr<Node::MemberDecl> n) override {
        out << convertType(n->type);
    }

    void visit(std::shared_ptr<Node::MethodDefn> n) override {}

    void visit(std::shared_ptr<Node::Member> n) override {
            
        // get member offset in type
        uint32_t offset = currentType->members.find(n->name)->second.offset;

        uint32_t memberLocationRegister = currentRegister;

        // get element ptr from "this" struct
        out << "  %" << memberLocationRegister << " = getelementptr inbounds "
            << convertType(currentType->name) << ", "
            << convertType(currentType->name) << "* %"
            << nameToRegister["@this"] << ", i32 0, "
            << "i32 " << offset << "\n";
        ++currentRegister;
        
        // structs always push their allocation pointer and their type
        if (!isPrimitive(n->type)) {
            exprStack.push_back({memberLocationRegister, n->type});
            return;
        }

        // primitives
        // if the assignment is literally `name = expression`, with no . or [] on lhs
        if (rootOfLhsOfAssignment) {
            // give the assignment the register of the variable
            exprStack.push_back({memberLocationRegister, n->type});   
        }
        else {
            // register & type stack
            exprStack.push_back({currentRegister, n->type});
            // load
            load(memberLocationRegister, n->type);
        }
    }

    void visit(std::shared_ptr<Node::MethodCall> n) override {}

    // visit unheap
    void visit(std::shared_ptr<Node::Unheap> n) override {

        // evaluate expr
        n->expr->accept(shared_from_this());
        auto exprInfo = exprStack.back();

        // bitcast to i8*
        out << "  %" << currentRegister
            << " = bitcast " << convertType(exprInfo.type)
            << " %" << exprInfo.reg
            << " to i8*\n";
        
        // call free
        out << "  call void @free(i8* noundef %" << currentRegister << ")\n";

        ++currentRegister;
    }

    void visit(std::shared_ptr<Node::OperatorDefn> n) override {}

    void visit(std::shared_ptr<Node::ConditionalBlock> n) override {}

    // @TODO: reintroduce loop metadata
    void visit(std::shared_ptr<Node::WhileLoop> n) override {
        std::string suffix = std::to_string(currentLabelNumber);
        ++currentLabelNumber;
        std::string conditionLabel = "cond" + suffix;
        std::string bodyLabel = "body" + suffix;
        std::string exitLabel = "exit" + suffix;

        // break to condition
        out << "  br label %" << conditionLabel << "\n\n";

        // condition
        out << conditionLabel << ":\n";
        n->expr->accept(shared_from_this());
        out << "  br i1 %" << currentRegister-1
            << ", label %" << bodyLabel
            << ", label %" << exitLabel
            << "\n\n";

        // clear expr stack
        exprStack.pop_back();

        // body
        out << bodyLabel << ":\n";
        n->stmts->accept(shared_from_this());
        out << "  br label %" << conditionLabel
            << "\n\n";

        // exit
        out << exitLabel <<  ":\n";

    }

    void visit(std::shared_ptr<Node::Break> n) override {}

    void visit(std::shared_ptr<Node::Continue> n) override {}
};

#endif
