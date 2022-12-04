#include "statement_syntax.hpp"

using std::string;
using std::vector;

if_statement_syntax::if_statement_syntax(expression_syntax* condition, statement_syntax* body) :
    condition(condition), body(body), else_clause(nullptr)
{
    if (condition->expression_return_type != fundamental_type::Bool)
    {
        // todo: handle error
    }

    condition->set_parent(this);
    body->set_parent(this);
}

if_statement_syntax::if_statement_syntax(expression_syntax* condition, statement_syntax* body, statement_syntax* else_clause) :
    condition(condition), body(body), else_clause(else_clause)
{
    if (condition->expression_return_type != fundamental_type::Bool)
    {
        // todo: handle error
    }

    condition->set_parent(this);
    body->set_parent(this);
    else_clause->set_parent(this);
}

vector<syntax_base*> if_statement_syntax::get_children() const
{
    return vector<syntax_base*>{condition, body, else_clause};
}

if_statement_syntax::~if_statement_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

while_statement_syntax::while_statement_syntax(expression_syntax* condition, statement_syntax* body) : condition(condition), body(body)
{
    if (condition->expression_return_type != fundamental_type::Bool)
    {
        // todo: handle error
    }

    condition->set_parent(this);
    body->set_parent(this);
}

vector<syntax_base*> while_statement_syntax::get_children() const
{
    return vector<syntax_base*>{condition, body};
}

while_statement_syntax::~while_statement_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

branch_statement_syntax::branch_statement_syntax(branch_type type) : type(type)
{
}

vector<syntax_base*> branch_statement_syntax::get_children() const
{
    return vector<syntax_base*>();
}

branch_statement_syntax::~branch_statement_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

return_statement_syntax::return_statement_syntax() : expression(nullptr)
{
}

return_statement_syntax::return_statement_syntax(expression_syntax* expression) : expression(expression)
{
    expression->set_parent(this);
}

vector<syntax_base*> return_statement_syntax::get_children() const
{
    return vector<syntax_base*>{expression};
}

return_statement_syntax::~return_statement_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

expression_statement_syntax::expression_statement_syntax(expression_syntax* expression) : expression(expression)
{
    expression->set_parent(this);
}

vector<syntax_base*> expression_statement_syntax::get_children() const
{
    return vector<syntax_base*>{expression};
}

expression_statement_syntax::~expression_statement_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

assignment_statement_syntax::assignment_statement_syntax(string identifier, expression_syntax* value) : identifier(identifier), value(value)
{
    // todo: verify that value type matches identifier type

    value->set_parent(this);
}

vector<syntax_base*> assignment_statement_syntax::get_children() const
{
    return vector<syntax_base*>{value};
}

assignment_statement_syntax::~assignment_statement_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

declaration_statement_syntax::declaration_statement_syntax(type_syntax* type, string identifier) :
    type(type), identifier(identifier), value(nullptr)
{
    if (type->is_special())
    {
        //todo: handle illigal type
    }

    type->set_parent(this);
}

declaration_statement_syntax::declaration_statement_syntax(type_syntax* type, string identifier, expression_syntax* value) :
    type(type), identifier(identifier), value(value)
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

    type->set_parent(this);
    value->set_parent(this);
}

vector<syntax_base*> declaration_statement_syntax::get_children() const
{
    return vector<syntax_base*>{value};
}

declaration_statement_syntax::~declaration_statement_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

block_statement_syntax::block_statement_syntax(list_syntax<statement_syntax>* statement_list) : statement_list(statement_list)
{
    statement_list->set_parent(this);
}

vector<syntax_base*> block_statement_syntax::get_children() const
{
    return vector<syntax_base*>{statement_list};
}

block_statement_syntax::~block_statement_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}
