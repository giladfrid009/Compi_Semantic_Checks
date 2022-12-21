#include "generic_syntax.hpp"
#include "output.hpp"
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

type_syntax::~type_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }

    delete type_token;
}

parameter_syntax::parameter_syntax(type_syntax* type, syntax_token* identifier_token):
    type(type), identifier_token(identifier_token), identifier(identifier_token->text)
{
    if (type->kind == type_kind::Void)
    {
        output::error_mismatch(identifier_token->position);
    }

    if (symbol_table::instance().contains_symbol(identifier))
    {
        output::error_def(identifier_token->position, identifier);
    }

    push_back_child(type);
}

parameter_syntax::~parameter_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }

    delete identifier_token;
}

function_declaration_syntax::function_declaration_syntax(type_syntax* return_type, syntax_token* identifier_token, list_syntax<parameter_syntax>* parameter_list, list_syntax<statement_syntax>* body):
    return_type(return_type), identifier_token(identifier_token), identifier(identifier_token->text), parameter_list(parameter_list), body(body)
{
    const symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->kind != symbol_kind::Function)
    {
        throw std::logic_error("function should be defined.");
    }

    const function_symbol* func_symbol = static_cast<const function_symbol*>(symbol);

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

    push_back_child(return_type);
    push_back_child(parameter_list);
    push_back_child(body);
}

function_declaration_syntax::~function_declaration_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }

    delete identifier_token;
}

root_syntax::root_syntax(list_syntax<function_declaration_syntax>* function_list): function_list(function_list)
{
    const symbol* main_sym = symbol_table::instance().get_symbol("main");

    if (main_sym == nullptr || main_sym->kind != symbol_kind::Function)
    {
        output::error_main_missing();
    }

    const function_symbol* func_sym = static_cast<const function_symbol*>(main_sym);

    if (func_sym->type != type_kind::Void || func_sym->parameter_types.size() != 0)
    {
        output::error_main_missing();
    }

    push_back_child(function_list);
}

root_syntax::~root_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}
