#include <vector>
#include <string>

using std::vector;
using std::string;

enum class ObjectType { Void, Int, Bool, Byte, String };
enum class TernaryOperator { TernaryIf };
enum class BinaryOperator { Add, Sub, Mul, Div, And, Or, Equal, NotEqual, LessThan, GreaterThan, LessThanEqual, GreaterThanEqual };
enum class UnaryOperator { Not, Cast };
enum class BranchType {Break, Continue };

class SyntaxNode
{
    virtual vector<SyntaxNode*> GetParent() = 0;
    
    virtual vector<SyntaxNode*> GetChildren() = 0;
};

class Expression : public SyntaxNode
{
    ObjectType return_type;
};

class UnaryExpression : public Expression
{
    Expression* exp;
    UnaryOperator operator_type;
};

class BinaryExpression : public Expression
{
    Expression* left;
    Expression* right;
    BinaryOperator operator_type;
};

class TernaryExpression : public Expression
{
    Expression* left;
    Expression* middle;
    Expression* right;
    TernaryOperator operator_type;
};

class ConstExpression : public Expression
{
    union Litteral 
    {
        int int_val;
        bool bool_val;
        char byte_val;
        char* str_val;
    };

    Litteral value;
};
 
class ArgumentList : public SyntaxNode
{
    vector<Expression*> arguments;
}; 

 class CallExpression : public Expression
{
    ArgumentList* argument_list;
    string identifier;
};

class IdentifierExpression : public Expression
{
    string identifier;
};

class TypeNode : public SyntaxNode
{
    ObjectType type;
};

class Statement : public SyntaxNode
{
    
};

class StatementsList : SyntaxNode
{
    vector<Statement*> statements;
};

class IfStatement : public Statement
{
    Expression* condition;
    Statement* body;
    Expression* else_clause;
};

class WhileStatement : public Statement
{
    Expression* condition;
    Statement* body;
};

class BranchStatement : public Statement
{
    BranchType type;
};

class ReturnStatement : public Statement
{
    Expression* expression;
    ObjectType return_type;
};

class CallStatement : public Statement
{
    CallExpression* call;
};

class AssignStatement : public Statement
{
    string identifier;
    ObjectType type;
    Expression* value;
};

class DeclarationStatement : public Statement
{
    string identifier;
    ObjectType type;
    Expression* value;
};

class BlockStatement : public Statement
{
    StatementsList* statements;
};

class FuncDeclaration : SyntaxNode
{
    TypeNode* return_type;
    string identifier;
    ParameterList* parametes;
    StatementsList* body;
};

class ParameterList : SyntaxNode
{

};

class Parameter : SyntaxNode
{
    ObjectType type;
    string identifier;
};

class FuncList : SyntaxNode
{
    vector<FuncDeclaration*> funcs;
};

class GlobalScope : SyntaxNode
{
    FuncList* funcs;
};