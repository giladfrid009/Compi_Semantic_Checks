#ifndef AST_NODES_HPP
#define AST_NODES_HPP

#include "syntax_base.hpp"

#include <vector>
#include <string>
#include <type_traits>

enum class fundamental_type { Void, Int, Bool, Byte, String };

enum class logical_operator { And, Or };

enum class arithmetic_operator { Add, Sub, Mul, Div };

enum class relational_operator { Less, Greater, LessEqual, GreaterEqual, Equal, NotEqual };

enum class branch_type { Break, Continue };

// base

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

    virtual ~expression_syntax() = default;

    virtual std::vector<syntax_base*> children() = 0;
};

class statement_syntax : public syntax_base
{
    public:

    virtual ~statement_syntax() = default;

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

    ~type_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

template<typename element_type> class list_syntax final : public syntax_base
{
    public:

    const std::vector<element_type*> values;

    list_syntax() : values(), syntax_base()
    {
        static_assert(std::is_base_of<syntax_base, element_type>::value, "Must be of type syntax_base");
    }

    list_syntax(element_type* element) : values(element), syntax_base()
    {
        static_assert(std::is_base_of<syntax_base, element_type>::value, "Must be of type syntax_base");

        element->register_parent(this);
    }

    list_syntax(std::vector<element_type*> elements) : values(elements), syntax_base()
    {
        static_assert(std::is_base_of<syntax_base, element_type>::value, "Must be of type syntax_base");

        for (element_type* element : elements)
        {
            element->register_parent(this);
        }
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

    ~list_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

class formal_syntax final : public syntax_base
{
    public:

    type_syntax* const type;
    const std::string identifier;

    formal_syntax(type_syntax* type, std::string identifier) : type(type), identifier(identifier), syntax_base()
    {
        if (type->type == fundamental_type::Void)
        {
            //todo: handle error
        }

        //todo: make sure identifier doesnt shadow anyone else.

        type->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{type};
    }

    ~formal_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
    
};

class function_declaration_syntax final : public syntax_base
{
    public:

    type_syntax* const return_type;
    const std::string identifier;
    list_syntax<formal_syntax>* const formal_list;
    list_syntax<statement_syntax>* const body;

    function_declaration_syntax(type_syntax* return_type, std::string identifier, list_syntax<formal_syntax>* formal_list, list_syntax<statement_syntax>* body) : 
        return_type(return_type), identifier(identifier), formal_list(formal_list), body(body), syntax_base()
    {
        // todo: check that identifier is free

        return_type->register_parent(this);
        formal_list->register_parent(this);
        body->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{return_type, formal_list, body};
    }

    ~function_declaration_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

class root_syntax final : public syntax_base
{
    public:

    list_syntax<function_declaration_syntax>* const function_list;

    root_syntax(list_syntax<function_declaration_syntax>* function_list) : function_list(function_list)
    {
        function_list->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{function_list};
    }
    
    ~root_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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

    ~cast_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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

    ~not_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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
    ~logical_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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

    ~arithmetic_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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

    ~relational_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{true_value, condition, false_value};
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

    ~conditional_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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
    
    ~literal_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

class identifier_expression_syntax final : public expression_syntax
{
    public:

    const std::string identifier;

    identifier_expression_syntax(std::string identifier) : identifier(identifier), expression_syntax(get_return_type(identifier))
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

    ~identifier_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

class invocation_expression_syntax final : public expression_syntax
{
    public:

    const std::string identifier;
    list_syntax<expression_syntax>* const expression_list;

    invocation_expression_syntax(std::string identifier) :
        identifier(identifier), expression_list(nullptr), expression_syntax(get_return_type(identifier))
    {
        // todo: check that identifier exists    
    }

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
    
    ~invocation_expression_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

// statements

class if_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const condition;
    statement_syntax* const body;
    expression_syntax* const else_clause;

    if_statement_syntax(expression_syntax* condition, statement_syntax* body) : 
        condition(condition), body(body), else_clause(nullptr), statement_syntax()
    {
        if (condition->expression_return_type != fundamental_type::Bool)
        {
            // todo: handle error
        }

        condition->register_parent(this);
        body->register_parent(this);
    }

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

    ~if_statement_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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

    ~while_statement_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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

    ~branch_statement_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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
    
    ~return_statement_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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
    
    ~expression_statement_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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
    
    ~assignment_statement_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

class declaration_statement_syntax final : public statement_syntax
{
    public:

    type_syntax* const type;
    const std::string identifier;
    expression_syntax* const value;

    declaration_statement_syntax(type_syntax* type, std::string identifier) : 
        type(type), identifier(identifier), value(nullptr), statement_syntax()
    {
        if (type->is_special())
        {
            //todo: handle illigal type
        }

        type->register_parent(this);
    }
    
    declaration_statement_syntax(type_syntax* type, std::string identifier, expression_syntax* value) : 
        type(type), identifier(identifier), value(value), statement_syntax()
    {
        if (type->is_special() || value->is_special())
        {
            //todo: handle illigal type
        }

        if (type->type != value->expression_return_type)
        {
            if (type->type != fundamental_type::Int || value->expression_return_type != fundamental_type::Byte)
            {
                // todo: handle error
            }
        }

        type->register_parent(this);
        value->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{value};
    }

    ~declaration_statement_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
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

    block_statement_syntax(list_syntax<statement_syntax>* statement_list) : statement_list(statement_list), statement_syntax()
    {
        statement_list->register_parent(this);
    }

    ~block_statement_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

#endif