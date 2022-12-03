#ifndef AST_NODES_HPP
#define AST_NODES_HPP

#include <vector>
#include <string>

enum class fundamental_type { Void, Int, Bool, Byte, String };

enum class logical_operator { And, Or };

enum class arithmetic_operator { Add, Sub, Mul, Div };

enum class relational_operator { Less, Greater, LessEqual, GreaterEqual, Equal, NotEqual };

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

    const fundamental_type type;

    type_syntax(fundamental_type type) : type(type)
    {
    }

    bool is_numeric()
    {
        return type == fundamental_type::Int || type == fundamental_type::Byte;
    }

    bool is_special()
    {
        return type == fundamental_type::String || type == fundamental_type::Void;
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

    const fundamental_type expression_return_type;

    expression_syntax(fundamental_type return_type) : expression_return_type(return_type)
    {
        if (is_special())
        {
            //todo: handle illigal type
        }
    }

    bool is_numeric()
    {
        return expression_return_type == fundamental_type::Int || expression_return_type == fundamental_type::Byte;
    }

    bool is_special()
    {
        return expression_return_type == fundamental_type::String || expression_return_type == fundamental_type::Void;
    }

    virtual ~expression_syntax() = default;
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

    type_syntax* const destination_type;
    expression_syntax* const expression;

    cast_expression_syntax(type_syntax* destination_type, expression_syntax* expression) :
        destination_type(destination_type), expression(expression), expression_syntax(destination_type->type)
    {
        if (expression->is_numeric() == false)
        {
            // todo: handle illigal cast
        }

        if (destination_type->is_numeric() == false)
        {
            // todo: handle illigal cast
        }
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{destination_type, expression};
    }
};

class not_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const expression;

    not_expression_syntax(expression_syntax* expression) :
        expression(expression), expression_syntax(fundamental_type::Bool)
    {
        if (expression->expression_return_type != fundamental_type::Bool)
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
        left(left), right(right), oper(oper), expression_syntax(fundamental_type::Bool)
    {
        if (left->expression_return_type != fundamental_type::Bool || right->expression_return_type != fundamental_type::Bool)
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
        if (left->is_numeric() == false || right->is_numeric() == false)
        {
            // todo: handle error
        }
    }

    fundamental_type get_return_type(expression_syntax* left, expression_syntax* right)
    {
        if (left->is_numeric() && right->is_numeric())
        {
            if (left->expression_return_type == fundamental_type::Int || right->expression_return_type == fundamental_type::Int)
            {
                return fundamental_type::Int;
            }

            return fundamental_type::Byte;
        }

        return fundamental_type::Void;
    }

    std::vector<ast_node*> children() override
    {
        return std::vector<ast_node*>{left, right};
    }
};

class relational_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const left;
    expression_syntax* const right;
    const relational_operator oper;

    relational_expression_syntax(expression_syntax* left, expression_syntax* right, relational_operator oper) :
        left(left), right(right), oper(oper), expression_syntax(fundamental_type::Bool)
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
    const fundamental_type type;

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

class invocation_expression_syntax final : public expression_syntax
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

    virtual ~statement_syntax() = default;
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

    const branch_type type;

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
    const fundamental_type type;
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

    const fundamental_type type;
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