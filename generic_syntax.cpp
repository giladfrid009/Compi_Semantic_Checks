#include "generic_syntax.hpp"
#include "hw3_output.hpp"

using std::vector;
using std::string;

type_syntax::type_syntax(syntax_token* type_token) : 
    type_token(type_token) ,type(string_to_fundamental_type(type_token->text))
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

vector<syntax_token*> type_syntax::get_tokens() const
{
    return vector<syntax_token*>{type_token};
}

type_syntax::~type_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }

    for (syntax_token* token : get_tokens())
    {
        delete token;
    }
}

formal_syntax::formal_syntax(type_syntax* type, syntax_token* identifier_token) : 
    type(type), identifier_token(identifier_token), identifier(identifier_token->text)
{
    if (type->type == fundamental_type::Void)
    {
        output::errorMismatch(0);
    }

    //todo: make sure identifier doesnt shadow anyone else.

    type->set_parent(this);
}

vector<syntax_base*> formal_syntax::get_children() const
{
    return vector<syntax_base*>{type};
}

vector<syntax_token*> formal_syntax::get_tokens() const
{
    return vector<syntax_token*>{identifier_token};
}

formal_syntax::~formal_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }

    for (syntax_token* token : get_tokens())
    {
        delete token;
    }
}

function_declaration_syntax::function_declaration_syntax(type_syntax* return_type, syntax_token* identifier_token, list_syntax<formal_syntax>* formal_list, list_syntax<statement_syntax>* body) :
    return_type(return_type), identifier_token(identifier_token), identifier(identifier_token->text), formal_list(formal_list), body(body)
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

vector<syntax_token*> function_declaration_syntax::get_tokens() const
{
    return vector<syntax_token*>{identifier_token};
}

function_declaration_syntax::~function_declaration_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }

    for (syntax_token* token : get_tokens())
    {
        delete token;
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

vector<syntax_token*> root_syntax::get_tokens() const
{
    return vector<syntax_token*>();
}

root_syntax::~root_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }

    for (syntax_token* token : get_tokens())
    {
        delete token;
    }
}
