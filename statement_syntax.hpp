#ifndef _STATEMENT_SYNTAX_HPP_
#define _STATEMENT_SYNTAX_HPP_

#include "abstract_syntax.hpp"
#include "expression_syntax.hpp"
#include "generic_syntax.hpp"
#include <vector>
#include <string>

class if_statement_syntax final : public statement_syntax
{
    public:

    expression_syntax* const condition;
    statement_syntax* const body;
    statement_syntax* const else_clause;

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

    if_statement_syntax(expression_syntax* condition, statement_syntax* body, statement_syntax* else_clause) : 
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