#ifndef _EXPRESSION_SYNTAX_HPP_
#define _EXPRESSION_SYNTAX_HPP_

#include "abstract_syntax.hpp"
#include "generic_syntax.hpp"
#include <vector>
#include <string>

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

#endif