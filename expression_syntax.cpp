#include "expression_syntax.hpp"
#include "symbol_table.hpp"
#include "hw3_output.hpp"
#include <stdexcept>

using std::string;
using std::vector;

extern int yylineno;

cast_expression_syntax::cast_expression_syntax(type_syntax* destination_type, expression_syntax* expression):
    expression_syntax(destination_type->type), destination_type(destination_type), expression(expression)
{
    if (expression->is_numeric() == false || destination_type->is_numeric() == false)
    {
        output::errorMismatch(yylineno);
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
    expression_syntax(fundamental_type::Bool), not_token(not_token), expression(expression)
{
    if (expression->return_type != fundamental_type::Bool)
    {
        output::errorMismatch(not_token->definition_line);
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
    expression_syntax(fundamental_type::Bool), left(left), oper_token(oper_token), right(right), oper(parse_operator(oper_token->text))
{
    if (left->return_type != fundamental_type::Bool || right->return_type != fundamental_type::Bool)
    {
        output::errorMismatch(oper_token->definition_line);
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

logical_expression_syntax::logical_operator logical_expression_syntax::parse_operator(string str)
{
    if (str == "and") return logical_operator::And;
    if (str == "or") return logical_operator::Or;

    throw std::invalid_argument("unknown oper");
}

arithmetic_expression_syntax::arithmetic_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right):
    expression_syntax(types::cast_up(left->return_type, right->return_type)), left(left), oper_token(oper_token), right(right), oper(parse_operator(oper_token->text))
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        output::errorMismatch(oper_token->definition_line);
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

arithmetic_expression_syntax::arithmetic_operator arithmetic_expression_syntax::parse_operator(string str)
{
    if (str == "+") return arithmetic_operator::Add;
    if (str == "-") return arithmetic_operator::Sub;
    if (str == "*") return arithmetic_operator::Mul;
    if (str == "/") return arithmetic_operator::Div;

    throw std::invalid_argument("unknown oper");
}

relational_expression_syntax::relational_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right):
    expression_syntax(fundamental_type::Bool), left(left), oper_token(oper_token), right(right), oper(parse_operator(oper_token->text))
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        output::errorMismatch(oper_token->definition_line);
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

relational_expression_syntax::relational_operator relational_expression_syntax::parse_operator(string str)
{
    if (str == "<") return relational_operator::Less;
    if (str == "<=") return relational_operator::LessEqual;
    if (str == ">") return relational_operator::Greater;
    if (str == ">=") return relational_operator::GreaterEqual;
    if (str == "==") return relational_operator::Equal;
    if (str == "!=") return relational_operator::NotEqual;

    throw std::invalid_argument("unknown oper");
}

conditional_expression_syntax::conditional_expression_syntax(expression_syntax* true_value, syntax_token* if_token, expression_syntax* condition, syntax_token* const else_token, expression_syntax* false_value):
    expression_syntax(types::cast_up(true_value->return_type, false_value->return_type)), true_value(true_value), if_token(if_token), condition(condition), else_token(else_token), false_value(false_value)
{
    if (return_type == fundamental_type::Void)
    {
        output::errorMismatch(if_token->definition_line);
    }

    if (condition->return_type != fundamental_type::Bool)
    {
        output::errorMismatch(if_token->definition_line);
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

    if (symbol == nullptr || symbol->sym_type != symbol_type::Var)
    {
        output::errorUndef(identifier_token->definition_line, identifier);
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

fundamental_type identifier_expression_syntax::get_return_type(string identifier)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->sym_type != symbol_type::Var)
    {
        return fundamental_type::Invalid;
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

    if (symbol == nullptr || symbol->sym_type != symbol_type::Func)
    {
        output::errorUndefFunc(identifier_token->definition_line, identifier);
    }

    vector<fundamental_type> parameter_types = static_cast<function_symbol*>(symbol)->parameter_types;

    vector<string> params_str;

    for (fundamental_type type : parameter_types)
    {
        params_str.push_back(types::to_string(type));
    }

    if (parameter_types.size() != 0)
    {
        output::errorPrototypeMismatch(identifier_token->definition_line, identifier, params_str);
    }
}

invocation_expression_syntax::invocation_expression_syntax(syntax_token* identifier_token, list_syntax<expression_syntax>* expression_list):
    expression_syntax(get_return_type(identifier_token->text)), identifier_token(identifier_token), identifier(identifier_token->text), expression_list(expression_list)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->sym_type != symbol_type::Func)
    {
        output::errorUndefFunc(identifier_token->definition_line, identifier);
    }

    vector<fundamental_type> parameter_types = static_cast<function_symbol*>(symbol)->parameter_types;

    auto elements = expression_list->get_elements();

    vector<string> params_str;

    for (fundamental_type type : parameter_types)
    {
        params_str.push_back(types::to_string(type));
    }

    if (parameter_types.size() != elements.size())
    {
        output::errorPrototypeMismatch(identifier_token->definition_line, identifier, params_str);
    }

    for (size_t i = 0; i < elements.size(); i++)
    {
        if (parameter_types[i] != elements[i]->return_type)
        {
            if (types::is_convertible(elements[i]->return_type, parameter_types[i]) == false)
            {
                output::errorPrototypeMismatch(identifier_token->definition_line, identifier, params_str);
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

fundamental_type invocation_expression_syntax::get_return_type(string identifier)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr || symbol->sym_type != symbol_type::Func)
    {
        return fundamental_type::Invalid;
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
