#ifndef SYNTAXNODE_HPP
#define SYNTAXNODE_HPP

#include <vector>
#include <string>

enum class Types { Void, Int, Bool, Byte, String };

enum class LogicalOperator { And, Or };

enum class ArithmeticOperator { Add, Sub, Mul, Div };

enum class EqualityOperator { Equal, NotEqual };

enum class ComparisonOperator { Less, Greater, LessEqual, GreaterEqual };

enum class BranchType { Break, Continue };

class AST_Node
{
    public:
};

class TypeSyntax : public AST_Node
{
    public:

    Types Type;
};

// expressions

class ExpressionSyntax : public AST_Node
{
    public:

    Types ReturnType;

    ExpressionSyntax(Types type)
    {
        ReturnType = type;

        if (type == Types::Void)
        {
            //todo: handle error
        }
    }
};

class ExpressionListSyntax : public AST_Node
{
    public:

    std::vector<ExpressionSyntax*> Arguments;
};

class CastExpressionSyntax : public ExpressionSyntax
{
    public:

    TypeSyntax* Type;
    ExpressionSyntax* Expression;

    CastExpressionSyntax(TypeSyntax* typeSyntax, ExpressionSyntax* expression) : ExpressionSyntax(typeSyntax->Type)
    {
        Type = typeSyntax;
        Expression = expression;

        Types src = Expression->ReturnType;
        Types dst = Type->Type;

        if (src != Types::Int && src != Types::Byte)
        {
            // todo: handle illigal cast
        }

        if (dst != Types::Int && dst != Types::Byte)
        {
            // todo: handle illigal cast
        }
    }
};

class NotExpressionSyntax : public ExpressionSyntax
{
    public:

    ExpressionSyntax* Expression;

    NotExpressionSyntax(ExpressionSyntax* expression) : ExpressionSyntax(expression->ReturnType)
    {
        Expression = expression;

        if (Expression->ReturnType != Types::Bool)
        {
            // todo: handle illigal type
        }
    }
};

class LogicalExpressionSyntax : public ExpressionSyntax
{
    public:

    ExpressionSyntax* Left;
    ExpressionSyntax* Right;
    LogicalOperator Operator;

    LogicalExpressionSyntax(ExpressionSyntax* left, ExpressionSyntax* right, LogicalOperator oper) : ExpressionSyntax(Types::Bool)
    {
        Left = left;
        Right = right;
        Operator = oper;

        if (left->ReturnType != Types::Bool || right->ReturnType != Types::Bool)
        {
            // todo: handle error
        }
    }
};

class ArithmeticExpressionSyntax : public ExpressionSyntax
{
    public:

    ExpressionSyntax* Left;
    ExpressionSyntax* Right;
    ArithmeticOperator Operator;

    ArithmeticExpressionSyntax(ExpressionSyntax* left, ExpressionSyntax* right, ArithmeticOperator oper) : ExpressionSyntax(GetReturnType(left, right))
    {
        Left = left;
        Right = right;
        Operator = oper;

        if (IsNumeric(left) == false)
        {
            // todo: handle error
        }

        if (IsNumeric(right) == false)
        {
            // todo: handle error
        }
    }

    bool IsNumeric(ExpressionSyntax* expression)
    {
        Types type = expression->ReturnType;
        return type == Types::Byte || type == Types::Int;
    }

    Types GetReturnType(ExpressionSyntax* left, ExpressionSyntax* right)
    {
        if (IsNumeric(left) && IsNumeric(right))
        {
            if (left->ReturnType == Types::Int || right->ReturnType == Types::Int)
            {
                return Types::Int;
            }

            return Types::Byte;
        }

        return Types::Void;
    }
};

class EqualityExpressionSyntax : public ExpressionSyntax
{
    ExpressionSyntax* Left;
    ExpressionSyntax* Right;
    EqualityOperator Operator;

    EqualityExpressionSyntax(ExpressionSyntax* left, ExpressionSyntax* right, EqualityOperator oper) : ExpressionSyntax(Types::Bool)
    {
        Left = left;
        Right = right;
        Operator = oper;

        if ((IsNumeric(left) == false || IsNumeric(right) == false) && left->ReturnType != right->ReturnType)
        {  
            //todo: handle error        
        }
    }

    bool IsNumeric(ExpressionSyntax* expression)
    {
        Types type = expression->ReturnType;
        return type == Types::Byte || type == Types::Int;
    }
};

class ComparisonExpressionSyntax : public ExpressionSyntax
{
    public:

    ExpressionSyntax* Left;
    ExpressionSyntax* Right;
    ComparisonOperator Operator;

    ComparisonExpressionSyntax(ExpressionSyntax* left, ExpressionSyntax* right, ComparisonOperator oper) : ExpressionSyntax(Types::Bool)
    {
        Left = left;
        Right = right;
        Operator = oper;

        if (IsNumeric(left) == false || IsNumeric(right) == false)
        {  
            //todo: handle error        
        }
    }

    bool IsNumeric(ExpressionSyntax* expression)
    {
        Types type = expression->ReturnType;
        return type == Types::Byte || type == Types::Int;
    }
};

class ConditionalExpressionSyntax : public ExpressionSyntax
{
    public:

    ExpressionSyntax* Condition;
    ExpressionSyntax* TrueValue;
    ExpressionSyntax* FalseValue;
};

class LiteralExpressionSyntax : public ExpressionSyntax
{
    public:

    union Literal
    {
        int AsInt;
        bool AsBool;
        char AsByte;
        std::string* AsStr;
    };

    Literal Value;
    Types Type;
};

class IdentifierExpressionSyntax : public ExpressionSyntax
{
    public:

    std::string Identifier;
};

class CallExpressionSyntax : public ExpressionSyntax
{
    public:

    std::string Identifier;
    ExpressionListSyntax* ArgumentList;
};

// end expressions

// statements

class StatementSyntax : public AST_Node
{
    public:

};

class StatementsListSyntax : public AST_Node
{
    public:

    std::vector<StatementSyntax*> Statements;
};

class IfStatementSyntax : public StatementSyntax
{
    public:

    ExpressionSyntax* Condition;
    StatementSyntax* Body;
    ExpressionSyntax* ElseClause;
};

class WhileStatementSyntax : public StatementSyntax
{
    public:

    ExpressionSyntax* Condition;
    StatementSyntax* Body;
};

class BranchStatementSyntax : public StatementSyntax
{
    public:

    BranchType Type;
};

class ReturnStatementSyntax : public StatementSyntax
{
    public:

    ExpressionSyntax* Expression;
};

class ExpressionStatementSyntax : public StatementSyntax
{
    public:

    ExpressionSyntax* Expression;
};

class AssignStatementSyntax : public StatementSyntax
{
    public:

    std::string Identifier;
    ExpressionSyntax* Value;
};

class DeclarationStatementSyntax : public StatementSyntax
{
    public:

    std::string Identifier;
    Types Type;
    ExpressionSyntax* Value;
};

class BlockStatementSyntax : public StatementSyntax
{
    public:

    StatementsListSyntax* StatementList;
};

// end statements

// generic types

class FormalSyntax : public AST_Node
{
    public:

    Types Type;
    std::string Identifier;
};

class FormalListSyntax : public AST_Node
{
    public:

    std::vector<FormalSyntax*> Formals;
};

class FunctionDeclarationSyntax : public AST_Node
{
    public:

    TypeSyntax* ReturnType;
    std::string Identifier;
    FormalListSyntax* ParameterList;
    StatementsListSyntax* Body;
};

class FunctionListSyntax : public AST_Node
{
    public:

    std::vector<FunctionDeclarationSyntax*> Functions;

    FunctionListSyntax()
    {
        Functions = std::vector<FunctionDeclarationSyntax*>();
    }

    FunctionListSyntax(FunctionDeclarationSyntax* function)
    {
        Functions = std::vector<FunctionDeclarationSyntax*> {function};
    }

    FunctionListSyntax(std::vector<FunctionDeclarationSyntax*> functions)
    {
        Functions = functions;
    }
};

class GlobalScopeSyntax : public AST_Node
{
    public:

    FunctionListSyntax* FunctionList;

    GlobalScopeSyntax(FunctionListSyntax* functionList)
    {
        FunctionList = functionList;
    }
};

#endif