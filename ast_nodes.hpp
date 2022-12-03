#ifndef AST_NODES_HPP
#define AST_NODES_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>

enum class fundamental_type { Void, Int, Bool, Byte, String };

enum class logical_operator { And, Or };

enum class arithmetic_operator { Add, Sub, Mul, Div };

enum class relational_operator { Less, Greater, LessEqual, GreaterEqual, Equal, NotEqual };

enum class branch_type { Break, Continue };

// base

class syntax_base
{
    private:

    syntax_base* parent_syntax = nullptr;

    protected:

    syntax_base* parent()
    {
        return parent_syntax;
    }

    virtual ~syntax_base() = 0;

    virtual std::vector<syntax_base*> children() = 0;

    public:

    void register_parent(syntax_base* new_parent)
    {
        parent_syntax = new_parent;
    }
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

    list_syntax(element_type* element) : list_syntax()
    {
        element->register_parent(this);

        values = std::vector<element_type*>{ element };
    }

    list_syntax(std::vector<element_type*> elements) : list_syntax()
    {
        for_each(elements.first(), elements.last(), [this](element_type* e) {e->register_parent(this);});

        values = elements;
    }

    list_syntax<element_type>* append(element_type* element)
    {
        values.push_back(element);
        return this;
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
        
        destination_type->register_parent(this);
        expression->register_parent(this);
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
        
        expression->register_parent(this);
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

        left->register_parent(this);
        right->register_parent(this);
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

        left->register_parent(this);
        right->register_parent(this);
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

        left->register_parent(this);
        right->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{left, right};
    }
};

class conditional_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const true_value;
    expression_syntax* const condition;
    expression_syntax* const false_value;

    conditional_expression_syntax(expression_syntax* true_value, expression_syntax* condition, expression_syntax* false_value) :
        true_value(true_value), condition(condition), false_value(false_value), expression_syntax(get_return_type(true_value, false_value))
    {
        if (true_value->expression_return_type != false_value->expression_return_type)
        {
            if (true_value->is_numeric() == false || false_value->is_numeric() == false)
            {
                // todo: handle error
            }
        }

        true_value->register_parent(this);
        condition->register_parent(this);
        false_value->register_parent(this);
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

        if (left->expression_return_type == right->expression_return_type)
        {
            return left->expression_return_type;
        }

        return fundamental_type::Void;
    }
};

template<typename literal_type> class literal_expression_syntax final : public expression_syntax
{
    public:

    const literal_type value;

    literal_expression_syntax(literal_type value) : value(value), expression_syntax(get_return_type(value))
    {
        
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }

    fundamental_type get_return_type(literal_type value)
    {
        if (std::is_same_v<literal_type, char>)
        {
            return fundamental_type::Byte;
        }
        
        if (std::is_same_v<literal_type, int>)
        {
            return fundamental_type::Byte;
        }

        if (std::is_same_v<literal_type, bool>)
        {
            return fundamental_type::Bool;
        }

        if (std::is_same_v<literal_type, std::string>)
        {
            return fundamental_type::String;
        }

        if (std::is_same_v<literal_type, void>)
        {
            return fundamental_type::Void;
        }

        // todo: throw an exception

        return fundamental_type::Void;
    }
};

class identifier_expression_syntax final : public expression_syntax
{
    public:

    const std::string identifier;

    identifier_expression_syntax(std::string identifier) : 
        identifier(identifier), expression_syntax(get_return_type(identifier))
    {
        //todo: add checks if identifier exists in symbol table
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }

    fundamental_type get_return_type(std::string identifier)
    {
        //todo: implement with symbol table
        return fundamental_type::Void;
    }
};

class invocation_expression_syntax final : public expression_syntax
{
    public:

    const std::string identifier;
    list_syntax<expression_syntax>* const expression_list;

    invocation_expression_syntax(std::string identifier, list_syntax<expression_syntax>* expression_list) :
        identifier(identifier), expression_list(expression_list), expression_syntax(get_return_type(identifier))
    {
        // todo: check that identifier exists
        // todo: check that expression_list matches signature
        
        expression_list->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{expression_list};
    }

    fundamental_type get_return_type(std::string identifier)
    {
        //todo: implement with symbol table
        return fundamental_type::Void;
    }
};

// statements

class if_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const condition;
    statement_syntax* const body;
    expression_syntax* const else_clause;

    if_statement_syntax(expression_syntax* condition, statement_syntax* body, expression_syntax* else_clause) : 
        condition(condition), body(body), else_clause(else_clause), statement_syntax()
    {
        if (condition->expression_return_type != fundamental_type::Bool)
        {
            // todo: handle error
        }

        condition->register_parent(this);
        body->register_parent(this);
        else_clause->register_parent(this);
    }

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

    while_statement_syntax(expression_syntax* condition, statement_syntax* body) : 
        condition(condition), body(body), statement_syntax()
    {
        if (condition->expression_return_type != fundamental_type::Bool)
        {
            // todo: handle error
        }

        condition->register_parent(this);
        body->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{condition, body};
    }
};

class branch_statement_syntax final : public statement_syntax
{
    public:

    const branch_type type;
    
    branch_statement_syntax(branch_type type) : type(type), statement_syntax()
    {
        
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }
};

class return_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const expression;

    return_statement_syntax() : expression(nullptr), statement_syntax()
    {

    }

    return_statement_syntax(expression_syntax* expression) : expression(expression), statement_syntax()
    {
        expression->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{expression};
    }
};

class expression_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const expression;

    expression_statement_syntax(expression_syntax* expression) : expression(expression), statement_syntax()
    {
        expression->register_parent(this);
    }

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

    assignment_statement_syntax(std::string identifier, expression_syntax* value) : 
        identifier(identifier), value(value), statement_syntax()
    {
        // todo: verify that value type matches identifier type
        
        value->register_parent(this);
    }

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