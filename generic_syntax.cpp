#include "generic_syntax.hpp"
#include "hw3_output.hpp"
#include "symbol.hpp"
#include "symbol_table.hpp"

using std::vector;
using std::string;

type_syntax::type_syntax(syntax_token* type_token):
    type_token(type_token), kind(types::from_string(type_token->text))
{
}

bool type_syntax::is_numeric() const
{
    return types::is_numeric(kind);
}

bool type_syntax::is_special() const
{
    return types::is_special(kind);
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

parameter_syntax::parameter_syntax(type_syntax* type, syntax_token* identifier_token):
    type(type), identifier_token(identifier_token), identifier(identifier_token->text)
{
    if (type->kind == type_kind::Void)
    {
        output::error_mismatch(identifier_token->definition_line);
    }

    if (symbol_table::instance().contains_symbol(identifier))
    {
        output::error_def(identifier_token->definition_line, identifier);
    }

    type->set_parent(this);
}

vector<syntax_base*> parameter_syntax::get_children() const
{
    return vector<syntax_base*>{type};
}

vector<syntax_token*> parameter_syntax::get_tokens() const
{
    return vector<syntax_token*>{identifier_token};
}

parameter_syntax::~parameter_syntax()
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

function_declaration_syntax::function_declaration_syntax(type_syntax* return_type, syntax_token* identifier_token, list_syntax<parameter_syntax>* parameter_list, list_syntax<statement_syntax>* body):
    return_type(return_type), identifier_token(identifier_token), identifier(identifier_token->text), parameter_list(parameter_list), body(body)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->kind != symbol_kind::Function)
    {
        throw std::logic_error("function should be defined.");
    }

    function_symbol* func_symbol = static_cast<function_symbol*>(symbol);

    auto elements = parameter_list->get_elements();

    if (func_symbol->parameter_types.size() != elements.size())
    {
        throw std::logic_error("function parameter length mismatch.");
    }

    for (size_t i = 0; i < elements.size(); i++)
    {
        if (func_symbol->parameter_types[i] != elements[i]->type->kind)
        {
            throw std::logic_error("function parameter type mismatch.");
        }
    }

    return_type->set_parent(this);
    parameter_list->set_parent(this);
    body->set_parent(this);
}

vector<syntax_base*> function_declaration_syntax::get_children() const
{
    return vector<syntax_base*>{return_type, parameter_list, body};
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

root_syntax::root_syntax(list_syntax<function_declaration_syntax>* function_list): function_list(function_list)
{
    symbol* main_sym = symbol_table::instance().get_symbol("main");

    if (main_sym == nullptr || main_sym->kind != symbol_kind::Function)
    {
        output::error_main_missing();
    }

    function_symbol* func_sym = static_cast<function_symbol*>(main_sym);

    if (func_sym->type != type_kind::Void || func_sym->parameter_types.size() != 0)
    {
        output::error_main_missing();
    }

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
