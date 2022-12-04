#include "generic_syntax.hpp"

using std::vector;
using std::string;

type_syntax::type_syntax(fundamental_type type) : type(type)
{
}

bool type_syntax::is_numeric() const
{
    return type == fundamental_type::Int || type == fundamental_type::Byte;
}

bool type_syntax::is_special() const
{
    return type == fundamental_type::String || type == fundamental_type::Void;
}

vector<syntax_base*> type_syntax::get_children() const
{
    return vector<syntax_base*>();
}

type_syntax::~type_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

formal_syntax::formal_syntax(type_syntax* type, string identifier) : type(type), identifier(identifier)
{
    if (type->type == fundamental_type::Void)
    {
        //todo: handle error
    }

    //todo: make sure identifier doesnt shadow anyone else.

    type->set_parent(this);
}

vector<syntax_base*> formal_syntax::get_children() const
{
    return vector<syntax_base*>{type};
}

formal_syntax::~formal_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

function_declaration_syntax::function_declaration_syntax(type_syntax* return_type, string identifier, list_syntax<formal_syntax>* formal_list, list_syntax<statement_syntax>* body) :
    return_type(return_type), identifier(identifier), formal_list(formal_list), body(body)
{
    // todo: check that identifier is free

    return_type->set_parent(this);
    formal_list->set_parent(this);
    body->set_parent(this);
}

vector<syntax_base*> function_declaration_syntax::get_children() const
{
    return vector<syntax_base*>{return_type, formal_list, body};
}

function_declaration_syntax::~function_declaration_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

root_syntax::root_syntax(list_syntax<function_declaration_syntax>* function_list) : function_list(function_list)
{
    function_list->set_parent(this);
}

vector<syntax_base*> root_syntax::get_children() const
{
    return vector<syntax_base*>{function_list};
}

root_syntax::~root_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}
