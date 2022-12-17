#include "expression_syntax.hpp"
#include "symbol_table.hpp"
#include "hw3_output.hpp"
#include <stdexcept>

using std::string;
using std::vector;

cast_expression_syntax::cast_expression_syntax(type_syntax* destination_type, expression_syntax* expression):
    expression_syntax(destination_type->kind), destination_type(destination_type), expression(expression)
{
    if (expression->is_numeric() == false || destination_type->is_numeric() == false)
    {
        output::error_mismatch(destination_type->type_token->position);
    }

    destination_type->set_parent(this);
    expression->set_parent(this);
}

vector<syntax_base*> cast_expression_syntax::get_children() const
{
    return vector<syntax_base*>{destination_type, expression};
}

vector<syntax_token*> cast_expression_syntax::get_tokens() const
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

not_expression_syntax::not_expression_syntax(syntax_token* not_token, expression_syntax* expression):
    expression_syntax(type_kind::Bool), not_token(not_token), expression(expression)
{
    if (expression->return_type != type_kind::Bool)
    {
        output::error_mismatch(not_token->position);
    }

    expression->set_parent(this);
}

vector<syntax_base*> not_expression_syntax::get_children() const
{
    return vector<syntax_base*>{expression};
}

vector<syntax_token*> not_expression_syntax::get_tokens() const
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

logical_expression_syntax::logical_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right):
    expression_syntax(type_kind::Bool), left(left), oper_token(oper_token), right(right), oper(parse_operator(oper_token->text))
{
    if (left->return_type != type_kind::Bool || right->return_type != type_kind::Bool)
    {
        output::error_mismatch(oper_token->position);
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

logical_expression_syntax::operator_kind logical_expression_syntax::parse_operator(string str)
{
    if (str == "and") return operator_kind::And;
    if (str == "or") return operator_kind::Or;

    throw std::invalid_argument("unknown oper");
}

arithmetic_expression_syntax::arithmetic_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right):
    expression_syntax(types::cast_up(left->return_type, right->return_type)), left(left), oper_token(oper_token), right(right), oper(parse_operator(oper_token->text))
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        output::error_mismatch(oper_token->position);
    }

    left->set_parent(this);
    right->set_parent(this);
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

arithmetic_expression_syntax::operator_kind arithmetic_expression_syntax::parse_operator(string str)
{
    if (str == "+") return operator_kind::Add;
    if (str == "-") return operator_kind::Sub;
    if (str == "*") return operator_kind::Mul;
    if (str == "/") return operator_kind::Div;

    throw std::invalid_argument("unknown oper");
}

relational_expression_syntax::relational_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right):
    expression_syntax(type_kind::Bool), left(left), oper_token(oper_token), right(right), oper(parse_operator(oper_token->text))
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        output::error_mismatch(oper_token->position);
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

relational_expression_syntax::operator_kind relational_expression_syntax::parse_operator(string str)
{
    if (str == "<") return operator_kind::Less;
    if (str == "<=") return operator_kind::LessEqual;
    if (str == ">") return operator_kind::Greater;
    if (str == ">=") return operator_kind::GreaterEqual;
    if (str == "==") return operator_kind::Equal;
    if (str == "!=") return operator_kind::NotEqual;

    throw std::invalid_argument("unknown oper");
}

conditional_expression_syntax::conditional_expression_syntax(expression_syntax* true_value, syntax_token* if_token, expression_syntax* condition, syntax_token* const else_token, expression_syntax* false_value):
    expression_syntax(types::cast_up(true_value->return_type, false_value->return_type)), true_value(true_value), if_token(if_token), condition(condition), else_token(else_token), false_value(false_value)
{
    if (return_type == type_kind::Void)
    {
        output::error_mismatch(if_token->position);
    }

    if (condition->return_type != type_kind::Bool)
    {
        output::error_mismatch(if_token->position);
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

identifier_expression_syntax::identifier_expression_syntax(syntax_token* identifier_token):
    expression_syntax(get_return_type(identifier_token->text)), identifier_token(identifier_token), identifier(identifier_token->text)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->kind != symbol_kind::Variable)
    {
        output::error_undef(identifier_token->position, identifier);
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

type_kind identifier_expression_syntax::get_return_type(string identifier)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->kind != symbol_kind::Variable)
    {
        return type_kind::Invalid;
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

invocation_expression_syntax::invocation_expression_syntax(syntax_token* identifier_token):
    expression_syntax(get_return_type(identifier_token->text)), identifier_token(identifier_token), identifier(identifier_token->text), expression_list(nullptr)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->kind != symbol_kind::Function)
    {
        output::error_undef_func(identifier_token->position, identifier);
    }

    vector<type_kind> parameter_types = static_cast<function_symbol*>(symbol)->parameter_types;

    vector<string> params_str;

    for (type_kind type : parameter_types)
    {
        params_str.push_back(types::to_string(type));
    }

    if (parameter_types.size() != 0)
    {
        output::error_prototype_mismatch(identifier_token->position, identifier, params_str);
    }
}

invocation_expression_syntax::invocation_expression_syntax(syntax_token* identifier_token, list_syntax<expression_syntax>* expression_list):
    expression_syntax(get_return_type(identifier_token->text)), identifier_token(identifier_token), identifier(identifier_token->text), expression_list(expression_list)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->kind != symbol_kind::Function)
    {
        output::error_undef_func(identifier_token->position, identifier);
    }

    vector<type_kind> parameter_types = static_cast<function_symbol*>(symbol)->parameter_types;

    auto elements = expression_list->get_elements();

    vector<string> params_str;

    for (type_kind type : parameter_types)
    {
        params_str.push_back(types::to_string(type));
    }

    if (parameter_types.size() != elements.size())
    {
        output::error_prototype_mismatch(identifier_token->position, identifier, params_str);
    }

    for (size_t i = 0; i < elements.size(); i++)
    {
        if (parameter_types[i] != elements[i]->return_type)
        {
            if (types::is_implictly_convertible(elements[i]->return_type, parameter_types[i]) == false)
            {
                output::error_prototype_mismatch(identifier_token->position, identifier, params_str);
            }
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

type_kind invocation_expression_syntax::get_return_type(string identifier)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->kind != symbol_kind::Function)
    {
        return type_kind::Invalid;
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
