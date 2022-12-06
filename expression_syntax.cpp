#include "expression_syntax.hpp"
#include "symbol_table.hpp"
#include "hw3_output.hpp"
#include <stdexcept>

using std::string;
using std::vector;

cast_expression_syntax::cast_expression_syntax(type_syntax* destination_type, expression_syntax* expression) :
    expression_syntax(destination_type->type), destination_type(destination_type), expression(expression)
{
    if (expression->is_numeric() == false || destination_type->is_numeric() == false)
    {
        output::errorMismatch(0);
    }

    destination_type->set_parent(this);
    expression->set_parent(this);
}

vector<syntax_base*> cast_expression_syntax::get_children() const
{
    return vector<syntax_base*>{destination_type, expression};
}

std::vector<syntax_token*> cast_expression_syntax::get_tokens() const
{
    return vector<syntax_token*>();
}

cast_expression_syntax::~cast_expression_syntax()
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

not_expression_syntax::not_expression_syntax(syntax_token* not_token, expression_syntax* expression) :
    expression_syntax(fundamental_type::Bool), not_token(not_token), expression(expression)
{
    if (expression->expression_return_type != fundamental_type::Bool)
    {
        output::errorMismatch(0);
    }

    expression->set_parent(this);
}

vector<syntax_base*> not_expression_syntax::get_children() const
{
    return vector<syntax_base*>{expression};
}

std::vector<syntax_token*> not_expression_syntax::get_tokens() const
{
    return vector<syntax_token*>{not_token};
}

not_expression_syntax::~not_expression_syntax()
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

logical_expression_syntax::logical_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right) :
    expression_syntax(fundamental_type::Bool), left(left), oper_token(oper_token), right(right), oper(string_to_logical_operator(oper_token->text))
{
    if (left->expression_return_type != fundamental_type::Bool || right->expression_return_type != fundamental_type::Bool)
    {
        output::errorMismatch(0);
    }

    left->set_parent(this);
    right->set_parent(this);
}

vector<syntax_base*> logical_expression_syntax::get_children() const
{
    return vector<syntax_base*>{left, right};
}

vector<syntax_token*> logical_expression_syntax::get_tokens() const
{
    return vector<syntax_token*>{oper_token};
}

logical_expression_syntax::~logical_expression_syntax()
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

arithmetic_expression_syntax::arithmetic_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right) :
    expression_syntax(get_return_type(left, right)), left(left), oper_token(oper_token), right(right), oper(string_to_arithmetic_operator(oper_token->text))
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        output::errorMismatch(0);
    }

    left->set_parent(this);
    right->set_parent(this);
}

fundamental_type arithmetic_expression_syntax::get_return_type(expression_syntax* left, expression_syntax* right)
{
    if (left->is_numeric() && right->is_numeric())
    {
        if (left->expression_return_type == fundamental_type::Int || right->expression_return_type == fundamental_type::Int)
        {
            return fundamental_type::Int;
        }

        return fundamental_type::Byte;
    }

    output::errorMismatch(0);

    return fundamental_type::Void;
}

vector<syntax_base*> arithmetic_expression_syntax::get_children() const
{
    return vector<syntax_base*>{left, right};
}

vector<syntax_token*> arithmetic_expression_syntax::get_tokens() const
{
    return vector<syntax_token*>{oper_token};
}

arithmetic_expression_syntax::~arithmetic_expression_syntax()
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

relational_expression_syntax::relational_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right) :
    expression_syntax(fundamental_type::Bool), left(left), oper_token(oper_token), right(right), oper(string_to_relational_operator(oper_token->text))
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        output::errorMismatch(0);
    }

    left->set_parent(this);
    right->set_parent(this);
}

vector<syntax_base*> relational_expression_syntax::get_children() const
{
    return vector<syntax_base*>{left, right};
}

vector<syntax_token*> relational_expression_syntax::get_tokens() const
{
    return vector<syntax_token*>{oper_token};
}

relational_expression_syntax::~relational_expression_syntax()
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

conditional_expression_syntax::conditional_expression_syntax(expression_syntax* true_value, syntax_token* if_token, expression_syntax* condition, syntax_token* const else_token, expression_syntax* false_value) :
    expression_syntax(get_return_type(true_value, false_value)), true_value(true_value), if_token(if_token), condition(condition), else_token(else_token), false_value(false_value)
{
    if (true_value->expression_return_type != false_value->expression_return_type)
    {
        if (true_value->is_numeric() == false || false_value->is_numeric() == false)
        {
            output::errorMismatch(0);
        }
    }

    if (condition->expression_return_type != fundamental_type::Bool)
    {
        output::errorMismatch(0);
    }

    true_value->set_parent(this);
    condition->set_parent(this);
    false_value->set_parent(this);
}

vector<syntax_base*> conditional_expression_syntax::get_children() const
{
    return vector<syntax_base*>{true_value, condition, false_value};
}

vector<syntax_token*> conditional_expression_syntax::get_tokens() const
{
    return vector<syntax_token*>{if_token, else_token};
}

fundamental_type conditional_expression_syntax::get_return_type(expression_syntax* left, expression_syntax* right) const
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

    output::errorMismatch(0);

    return fundamental_type::Void;
}

conditional_expression_syntax::~conditional_expression_syntax()
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

identifier_expression_syntax::identifier_expression_syntax(syntax_token* identifier_token) :
    expression_syntax(get_return_type(identifier_token->text)), identifier_token(identifier_token), identifier(identifier_token->text)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndef(0, identifier);
    }

    if (symbol->sym_type != symbol_type::Var)
    {
        output::errorUndef(0, identifier);
    }
}

vector<syntax_base*> identifier_expression_syntax::get_children() const
{
    return vector<syntax_base*>();
}

vector<syntax_token*> identifier_expression_syntax::get_tokens() const
{
    return vector<syntax_token*>{identifier_token};
}

fundamental_type identifier_expression_syntax::get_return_type(string identifier) const
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndef(0, identifier);
    }

    return symbol->type;
}

identifier_expression_syntax::~identifier_expression_syntax()
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

invocation_expression_syntax::invocation_expression_syntax(syntax_token* identifier_token) :
    expression_syntax(get_return_type(identifier_token->text)), identifier_token(identifier_token), identifier(identifier_token->text), expression_list(nullptr)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndefFunc(0, identifier);
    }

    if (symbol->sym_type != symbol_type::Func)
    {
        output::errorUndefFunc(0, identifier);
    }

    function_symbol* func_symbol = dynamic_cast<function_symbol*>(symbol);

    std::vector<string> params_str;

    if (func_symbol->parameter_types.size() != 0)
    {
        output::errorPrototypeMismatch(0, identifier, params_str);
    }
}

invocation_expression_syntax::invocation_expression_syntax(syntax_token* identifier_token, list_syntax<expression_syntax>* expression_list) :
    expression_syntax(get_return_type(identifier_token->text)), identifier_token(identifier_token), identifier(identifier_token->text), expression_list(expression_list)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndefFunc(0, identifier);
    }

    if (symbol->sym_type != symbol_type::Func)
    {
        output::errorUndefFunc(0, identifier);
    }

    function_symbol* func_symbol = dynamic_cast<function_symbol*>(symbol);

    auto elements = expression_list->get_elements();

    std::vector<string> params_str;

    for (expression_syntax* expr : elements)
    {
        params_str.push_back(fundamental_type_to_string(expr->expression_return_type));
    }

    if (func_symbol->parameter_types.size() != elements.size())
    {
        output::errorPrototypeMismatch(0, identifier, params_str);
    }

    for (size_t i = 0; i < elements.size(); i++)
    {
        if (func_symbol->parameter_types[i] != elements[i]->expression_return_type)
        {
            output::errorPrototypeMismatch(0, identifier, params_str);
        }
    }

    expression_list->set_parent(this);
}

vector<syntax_base*> invocation_expression_syntax::get_children() const
{
    return vector<syntax_base*>{expression_list};
}

vector<syntax_token*> invocation_expression_syntax::get_tokens() const
{
    return vector<syntax_token*>{identifier_token};
}

fundamental_type invocation_expression_syntax::get_return_type(string identifier) const
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndefFunc(0, identifier);
    }

    return symbol->type;
}

invocation_expression_syntax::~invocation_expression_syntax()
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
