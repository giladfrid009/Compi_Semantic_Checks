#ifndef AST_NODES_HPP
#define AST_NODES_HPP

#include <vector>
#include <string>

enum class fundamental_type { Void, Int, Bool, Byte, String };

enum class logical_operator { And, Or };

enum class arithmetic_operator { Add, Sub, Mul, Div };

enum class relational_operator { Less, Greater, LessEqual, GreaterEqual, Equal, NotEqual };

enum class branch_type { Break, Continue };

// base

class syntax_base
{
    protected:

    virtual ~syntax_base() = 0;

    virtual std::vector<syntax_base*> children() = 0;
};

class expression_syntax : public syntax_base
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

    virtual ~expression_syntax() = 0;

    virtual std::vector<syntax_base*> children() = 0;
};

class statement_syntax : public syntax_base
{
    public:

    virtual ~statement_syntax() = 0;

    virtual std::vector<syntax_base*> children() = 0;
};

// generic

class type_syntax final : public syntax_base
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

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }
};

template<typename element_type> class list_syntax final : public syntax_base
{
    public:

    std::vector<element_type*> values;

    list_syntax()
    {
        static_assert(std::is_base_of<syntax_base, element_type>::value, "Must be of type syntax_base");
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>(values.begin(), values.end());
    }
};

class formal_syntax final : public syntax_base
{
    public:

    const fundamental_type type;
    const std::string identifier;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }
};

class function_declaration_syntax final : public syntax_base
{
    public:

    type_syntax* const return_type;
    const std::string identifier;
    list_syntax<formal_syntax>* const formal_list;
    list_syntax<statement_syntax>* const body;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{return_type, formal_list, body};
    }
};

class root_syntax final : public syntax_base
{
    public:

    list_syntax<function_declaration_syntax>* const function_list;

    root_syntax(list_syntax<function_declaration_syntax>* function_list) : function_list(function_list)
    {
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{function_list};
    }
};

// expressions

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

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{destination_type, expression};
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

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{expression};
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

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{left, right};
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

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{left, right};
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

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{left, right};
    }
};

class conditional_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const condition;
    expression_syntax* const true_value;
    const expression_syntax* false_value;
};

template<typename literal_type> class literal_expression_syntax final : public expression_syntax
{
    public:

    const literal_type value;
    const fundamental_type type;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }
};

class identifier_expression_syntax final : public expression_syntax
{
    public:

    const std::string identifier;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }
};

class invocation_expression_syntax final : public expression_syntax
{
    public:

    const std::string identifier;
    list_syntax<expression_syntax>* const expression_list;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{expression_list};
    }
};

// statements

class if_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const condition;
    statement_syntax* const body;
    expression_syntax* const else_clause;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{condition, body, else_clause};
    }
};

class while_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const condition;
    statement_syntax* const body;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{condition, body};
    }
};

class branch_statement_syntax final : public statement_syntax
{
    public:

    const branch_type type;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }
};

class return_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const expression;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{expression};
    }
};

class expression_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const expression;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{expression};
    }
};

class assignment_statement_syntax final : public statement_syntax
{
    public:

    const std::string identifier;
    expression_syntax* const value;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{value};
    }
};

class declaration_statement_syntax final : public statement_syntax
{
    public:

    const std::string identifier;
    const fundamental_type type;
    expression_syntax* const value;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{value};
    }
};

class block_statement_syntax final : public statement_syntax
{
    public:

    list_syntax<statement_syntax>* const statement_list;

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{statement_list};
    }
};

#endif