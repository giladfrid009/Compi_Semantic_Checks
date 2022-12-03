#ifndef AST_NODES_HPP
#define AST_NODES_HPP

#include <vector>
#include <string>

enum class types { Void, Int, Bool, Byte, String };

enum class logical_operator { And, Or };

enum class arithmetic_operator { Add, Sub, Mul, Div };

enum class equality_operator { Equal, NotEqual };

enum class comparison_operator { Less, Greater, LessEqual, GreaterEqual };

enum class branch_type { Break, Continue };

class ast_node
{
    protected:

    virtual std::vector<ast_node*> children() = 0;

    virtual ~ast_node() = default;
};

class type_syntax final : public ast_node
{
    public:

    const types type;

    type_syntax(types type) : type(type)
    {
    }

    bool is_numeric()
    {
        return type == types::Int || type == types::Byte;
    }

    bool is_special()
    {
        return type == types::String || type == types::Void;
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>();
    }
};

// expressions

class expression_syntax : public ast_node
{
    public:

    const types return_type;

    expression_syntax(types return_type) : return_type(return_type)
    {
        if (is_special())
        {
            //todo: handle illigal type
        }
    }

    bool is_numeric()
    {
        return return_type == types::Int || return_type == types::Byte;
    }

    bool is_special()
    {
        return return_type == types::String || return_type == types::Void;
    }
};

class expression_list_syntax final : public ast_node
{
    public:

    std::vector<expression_syntax*> arguments;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>(arguments.begin(), arguments.end());
    }
};

class cast_expression_syntax final : public expression_syntax
{
    public:

    type_syntax* const dest_type;
    expression_syntax* const expression;

    cast_expression_syntax(type_syntax* dest_type, expression_syntax* expression) :
        dest_type(dest_type), expression(expression), expression_syntax(dest_type->type)
    {
        if (expression->is_numeric() == false)
        {
            // todo: handle illigal cast
        }

        if (dest_type->is_numeric() == false)
        {
            // todo: handle illigal cast
        }
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{dest_type, expression};
    }
};

class not_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const expression;

    not_expression_syntax(expression_syntax* expression) :
        expression(expression), expression_syntax(expression->return_type)
    {
        if (expression->return_type != types::Bool)
        {
            // todo: handle illigal return_type
        }
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{expression};
    }
};

class logical_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const left;
    expression_syntax* const right;
    const logical_operator oper;

    logical_expression_syntax(expression_syntax* left, expression_syntax* right, logical_operator oper) :
        left(left), right(right), oper(oper), expression_syntax(types::Bool)
    {
        if (left->return_type != types::Bool || right->return_type != types::Bool)
        {
            // todo: handle error
        }
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{left, right};
    }
};

class arithmetic_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const left;
    expression_syntax* const right;
    const arithmetic_operator oper;

    arithmetic_expression_syntax(expression_syntax* left, expression_syntax* right, arithmetic_operator oper) :
        left(left), right(right), oper(oper), expression_syntax(get_return_type(left, right))
    {
        if (left->is_numeric() == false)
        {
            // todo: handle error
        }

        if (right->is_numeric() == false)
        {
            // todo: handle error
        }
    }

    types get_return_type(expression_syntax* left, expression_syntax* right)
    {
        if (left->is_numeric() && right->is_numeric())
        {
            if (left->return_type == types::Int || right->return_type == types::Int)
            {
                return types::Int;
            }

            return types::Byte;
        }

        return types::Void;
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{left, right};
    }
};

class equality_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const left;
    expression_syntax* const right;
    const equality_operator oper;

    equality_expression_syntax(expression_syntax* left, expression_syntax* right, equality_operator oper) :
        left(left), right(right), oper(oper), expression_syntax(types::Bool)
    {
        if ((left->is_numeric() == false || right->is_numeric() == false) && left->return_type != right->return_type)
        {
            //todo: handle error        
        }
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{left, right};
    }
};

class comparison_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const left;
    expression_syntax* const right;
    const comparison_operator oper;

    comparison_expression_syntax(expression_syntax* left, expression_syntax* right, comparison_operator oper) :
        left(left), right(right), oper(oper), expression_syntax(types::Bool)
    {
        if (left->is_numeric() == false || right->is_numeric() == false)
        {
            //todo: handle error        
        }
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{left, right};
    }
};

class conditional_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const condition;
    expression_syntax* const true_value;
    const expression_syntax* false_value;
};

class literal_expression_syntax final : public expression_syntax
{
    public:

    union literal
    {
        int as_int;
        bool as_bool;
        char as_byte;
        std::string* as_str;
    };

    const literal value;
    const types return_type;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>();
    }
};

class identifier_expression_syntax final : public expression_syntax
{
    public:

    const std::string identifier;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>();
    }
};

class call_expression_syntax final : public expression_syntax
{
    public:

    const std::string identifier;
    expression_list_syntax* const expression_list;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{expression_list};
    }
};

// end expressions

// statements

class statement_syntax : public ast_node
{
    public:
};

class statement_list_syntax final : public ast_node
{
    public:

    const std::vector<statement_syntax*> statements;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>(statements.begin(), statements.end());
    }
};

class if_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const condition;
    statement_syntax* const body;
    expression_syntax* const else_clause;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{condition, body, else_clause};
    }
};

class while_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const condition;
    statement_syntax* const body;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{condition, body};
    }
};

class branch_statement_syntax final : public statement_syntax
{
    public:

    const branch_type return_type;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>();
    }
};

class return_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const expression;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{expression};
    }
};

class expression_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const expression;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{expression};
    }
};

class assignment_statement_syntax final : public statement_syntax
{
    public:

    const std::string identifier;
    expression_syntax* const value;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{value};
    }
};

class declaration_statement_syntax final : public statement_syntax
{
    public:

    const std::string identifier;
    const types return_type;
    expression_syntax* const value;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{value};
    }
};

class block_statement_syntax final : public statement_syntax
{
    public:

    statement_list_syntax* const statement_list;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{statement_list};
    }
};

// end statements

// generic types

class formal_syntax final : public ast_node
{
    public:

    const types return_type;
    const std::string identifier;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>();
    }
};

class formal_list_syntax final : public ast_node
{
    public:

    const std::vector<formal_syntax*> formals;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>(formals.begin(), formals.end());
    }
};

class function_declaration_syntax final : public ast_node
{
    public:

    type_syntax* const return_type;
    const std::string identifier;
    formal_list_syntax* const formal_list;
    statement_list_syntax* const body;

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{return_type, formal_list, body};
    }
};

class function_list_syntax final : public ast_node
{
    public:

    const std::vector<function_declaration_syntax*> functions;

    function_list_syntax()
    {
    }

    function_list_syntax(function_declaration_syntax* function) : functions(std::vector<function_declaration_syntax*> {function})
    {
    }

    function_list_syntax(std::vector<function_declaration_syntax*> functions) : functions(functions)
    {
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>(functions.begin(), functions.end());
    }
};

class global_syntax final : public ast_node
{
    public:

    function_list_syntax* const function_list;

    global_syntax(function_list_syntax* function_list) : function_list(function_list)
    {
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{function_list};
    }
};

#endif