#include <vector>
#include <string>

using std::vector;
using std::string;

enum class ObjectType { Void, Int, Bool, Byte, String };
enum class TernaryOperator { TernaryIf };
enum class BinaryOperator { Add, Sub, Mul, Div, And, Or, Equal, NotEqual, LessThan, GreaterThan, LessThanEqual, GreaterThanEqual };
enum class UnaryOperator { Not };
enum class BranchType { Break, Continue };

class SyntaxNode
{
    virtual vector<SyntaxNode*> GetParent() = 0;

    virtual vector<SyntaxNode*> GetChildren() = 0;
};

class Expression : public SyntaxNode
{
    public:

    ObjectType ReturnType;

    Expression(ObjectType type)
    {
        ReturnType = type;
    }
};

class CastExpression : public Expression
{
    public:

    TypeNode* typeNode;
    Expression* exp;

    CastExpression(TypeNode* typeNode, Expression* exp) : Expression(typeNode->type)
    {
        ObjectType srcType = exp->ReturnType;
        ObjectType dstType = typeNode->type;

        if (dstType != ObjectType::Int && dstType != ObjectType::Byte)
        {
            // todo: handle illigal cast
        }

        if (srcType != ObjectType::Int && srcType != ObjectType::Byte)
        {
            // todo: handle illigal cast
        }

        this->typeNode = typeNode;
        this->exp = exp;
    }
};

class UnaryExpression : public Expression
{
    public:

    Expression* exp;
    UnaryOperator operator_type;

    UnaryExpression(Expression* expression, UnaryOperator op) : Expression(expression->ReturnType)
    {
        if (operator_type == UnaryOperator::Not)
        {
            if (exp->ReturnType != ObjectType::Bool)
            {
                // todo: handle error
            }

            if (ReturnType != ObjectType::Bool)
            {
                // todo: handle error
            }

            exp = expression;
            operator_type = op;
        }
    }
};

class BinaryExpression : public Expression
{
    public:

    Expression* left;
    Expression* right;
    BinaryOperator operator_type;

    BinaryExpression(Expression* l, Expression* r, BinaryOperator op)
    {
        if (BinaryExpression::IsLogicalOperator(op))
        {
            if (l->ReturnType != ObjectType::Bool || r->ReturnType != ObjectType::Bool)
            {
                //todo: handle error
            }
        }

        if (BinaryExpression::IsNumericOperator(op))
        {

        }
    }

    static bool IsLogicalOperator(BinaryOperator op)
    {
        return op == BinaryOperator::Or || op == BinaryOperator::And;
    }

    static bool IsNumericOperator(BinaryOperator op)
    {
        return op == BinaryOperator::Add || op == BinaryOperator::Sub || op == BinaryOperator::Mul || op == BinaryOperator::Div;
    }

    static bool IsNumericType(ObjectType type)
    {
        return type == ObjectType::Byte || type == ObjectType::Int;
    }

    static ObjectType GetReturnType(ObjectType left, ObjectType right)
    {

    }
};

class TernaryExpression : public Expression
{
    public:

    Expression* Left;
    Expression* Middle;
    Expression* Right;
    TernaryOperator Operator;
};

class ConstExpression : public Expression
{
    public:

    Litteral Value;
    ObjType ValueType;

    union Litteral
    {
        int int_val;
        bool bool_val;
        char byte_val;
        char* str_val;
    };
};

class ArgumentList : public SyntaxNode
{
    public:

    vector<Expression*> arguments;
};

class CallExpression : public Expression
{
    public:

    ArgumentList* argument_list;
    string identifier;
};

class IdentifierExpression : public Expression
{
    public:

    string identifier;
};

class TypeNode : public SyntaxNode
{
    public:

    ObjectType type;
};

class Statement : public SyntaxNode
{
    public:

};

class StatementsList : SyntaxNode
{
    public:

    vector<Statement*> statements;
};

class IfStatement : public Statement
{
    public:

    Expression* condition;
    Statement* body;
    Expression* else_clause;
};

class WhileStatement : public Statement
{
    public:

    Expression* condition;
    Statement* body;
};

class BranchStatement : public Statement
{
    public:

    BranchType type;
};

class ReturnStatement : public Statement
{
    public:

    Expression* expression;
    ObjectType return_type;
};

class CallStatement : public Statement
{
    public:

    CallExpression* call;
};

class AssignStatement : public Statement
{
    public:

    string identifier;
    ObjectType type;
    Expression* value;
};

class DeclarationStatement : public Statement
{
    public:

    string Identifier;
    ObjectType Type;
    Expression* Value;
};

class BlockStatement : public Statement
{
    public:

    StatementsList* StatementList;
};

class FuncDeclaration : SyntaxNode
{
    public:

    TypeNode* ReturnType;
    string Identifier;
    ParameterList* ParameterList;
    StatementsList* Body;
};

class ParameterList : SyntaxNode
{
    public:

};

class Parameter : SyntaxNode
{
    public:

    ObjectType Type;
    string Identifier;
};

class FunctionList : SyntaxNode
{
    public:

    vector<FuncDeclaration*> Functions;
};

class GlobalScope : SyntaxNode
{
    public:

    FunctionList* FunctionList;
};