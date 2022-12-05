#include "expression_syntax.hpp"
#include "symbol_table.hpp"
#include "hw3_output.hpp"
#include <stdexcept>

extern int yylineno;

using std::string;
using std::vector;

cast_expression_syntax::cast_expression_syntax(type_syntax* destination_type, expression_syntax* expression) :
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

cast_expression_syntax::~cast_expression_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}

not_expression_syntax::not_expression_syntax(expression_syntax* expression) :
    expression_syntax(fundamental_type::Bool), expression(expression)
{
    if (expression->expression_return_type != fundamental_type::Bool)
    {
        output::errorMismatch(yylineno);
    }

    expression->set_parent(this);
}

vector<syntax_base*> not_expression_syntax::get_children() const
{
    return vector<syntax_base*>{expression};
}

not_expression_syntax::~not_expression_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}

logical_expression_syntax::logical_expression_syntax(expression_syntax* left, expression_syntax* right, logical_operator oper) :
    expression_syntax(fundamental_type::Bool), left(left), right(right), oper(oper)
{
    if (left->expression_return_type != fundamental_type::Bool || right->expression_return_type != fundamental_type::Bool)
    {
        output::errorMismatch(yylineno);
    }

    left->set_parent(this);
    right->set_parent(this);
}

logical_expression_syntax::logical_expression_syntax(expression_syntax* left, expression_syntax* right, string oper_token) :
    logical_expression_syntax(left, right, get_operator_from_token(oper_token))
{
}

logical_operator logical_expression_syntax::get_operator_from_token(string token) const
{
    if (token == "and") return logical_operator::And;
    if (token == "or") return logical_operator::Or;

    throw std::invalid_argument("invalid token");
}

vector<syntax_base*> logical_expression_syntax::get_children() const
{
    return vector<syntax_base*>{left, right};
}

logical_expression_syntax::~logical_expression_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}

arithmetic_expression_syntax::arithmetic_expression_syntax(expression_syntax* left, expression_syntax* right, arithmetic_operator oper) :
    expression_syntax(get_return_type(left, right)), left(left), right(right), oper(oper)
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        output::errorMismatch(yylineno);
    }

    left->set_parent(this);
    right->set_parent(this);
}

arithmetic_expression_syntax::arithmetic_expression_syntax(expression_syntax* left, expression_syntax* right, string oper_token) :
    arithmetic_expression_syntax(left, right, get_operator_from_token(oper_token))
{
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

    output::errorMismatch(yylineno);

    return fundamental_type::Void;
}

arithmetic_operator arithmetic_expression_syntax::get_operator_from_token(string token) const
{
    if (token == "+") return arithmetic_operator::Add;
    if (token == "-") return arithmetic_operator::Sub;
    if (token == "*") return arithmetic_operator::Mul;
    if (token == "/") return arithmetic_operator::Div;

    throw std::invalid_argument("invalid token");
}

vector<syntax_base*> arithmetic_expression_syntax::get_children() const
{
    return vector<syntax_base*>{left, right};
}

arithmetic_expression_syntax::~arithmetic_expression_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}

relational_expression_syntax::relational_expression_syntax(expression_syntax* left, expression_syntax* right, relational_operator oper) :
    expression_syntax(fundamental_type::Bool), left(left), right(right), oper(oper)
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        output::errorMismatch(yylineno);
    }

    left->set_parent(this);
    right->set_parent(this);
}

relational_expression_syntax::relational_expression_syntax(expression_syntax* left, expression_syntax* right, string oper_token) :
    relational_expression_syntax(left, right, get_operator_from_token(oper_token))
{
}

relational_operator relational_expression_syntax::get_operator_from_token(string token) const
{
    if (token == "<") return relational_operator::Less;
    if (token == "<=") return relational_operator::LessEqual;
    if (token == ">") return relational_operator::Greater;
    if (token == ">=") return relational_operator::GreaterEqual;
    if (token == "==") return relational_operator::Equal;
    if (token == "!=") return relational_operator::NotEqual;

    throw std::invalid_argument("invalid token");
}

vector<syntax_base*> relational_expression_syntax::get_children() const
{
    return vector<syntax_base*>{left, right};
}

relational_expression_syntax::~relational_expression_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}

conditional_expression_syntax::conditional_expression_syntax(expression_syntax* true_value, expression_syntax* condition, expression_syntax* false_value) :
    expression_syntax(get_return_type(true_value, false_value)), true_value(true_value), condition(condition), false_value(false_value)
{
    if (true_value->expression_return_type != false_value->expression_return_type)
    {
        if (true_value->is_numeric() == false || false_value->is_numeric() == false)
        {
            output::errorMismatch(yylineno);
        }
    }

    if (condition->expression_return_type != fundamental_type::Bool)
    {
        output::errorMismatch(yylineno);
    }

    true_value->set_parent(this);
    condition->set_parent(this);
    false_value->set_parent(this);
}

vector<syntax_base*> conditional_expression_syntax::get_children() const
{
    return vector<syntax_base*>{true_value, condition, false_value};
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

    output::errorMismatch(yylineno);

    return fundamental_type::Void;
}

conditional_expression_syntax::~conditional_expression_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}

identifier_expression_syntax::identifier_expression_syntax(string identifier) :
    expression_syntax(get_return_type(identifier)), identifier(identifier)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndef(yylineno, identifier);
    }

    if (symbol->sym_type != symbol_type::Var)
    {
        output::errorUndef(yylineno, identifier);
    }
}

vector<syntax_base*> identifier_expression_syntax::get_children() const
{
    return vector<syntax_base*>();
}

fundamental_type identifier_expression_syntax::get_return_type(string identifier) const
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndef(yylineno, identifier);
    }

    return symbol->type;
}

identifier_expression_syntax::~identifier_expression_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}

invocation_expression_syntax::invocation_expression_syntax(string identifier) :
    expression_syntax(get_return_type(identifier)), identifier(identifier), expression_list(nullptr)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndefFunc(yylineno, identifier);
    }

    if (symbol->sym_type != symbol_type::Func)
    {
        output::errorUndefFunc(yylineno, identifier);
    }

    function_symbol* func_symbol = dynamic_cast<function_symbol*>(symbol);

    std::vector<string> params_str;

    if (func_symbol->parameter_types.size() != 0)
    {
        output::errorPrototypeMismatch(yylineno, identifier, params_str);
    }
}

invocation_expression_syntax::invocation_expression_syntax(string identifier, list_syntax<expression_syntax>* expression_list) :
    expression_syntax(get_return_type(identifier)), identifier(identifier), expression_list(expression_list)
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndefFunc(yylineno, identifier);
    }

    if (symbol->sym_type != symbol_type::Func)
    {
        output::errorUndefFunc(yylineno, identifier);
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
        output::errorPrototypeMismatch(yylineno, identifier, params_str);
    }

    for (size_t i = 0; i < elements.size(); i++)
    {
        if (func_symbol->parameter_types[i] != elements[i]->expression_return_type)
        {
            output::errorPrototypeMismatch(yylineno, identifier, params_str);
        }
    }

    expression_list->set_parent(this);
}

vector<syntax_base*> invocation_expression_syntax::get_children() const
{
    return vector<syntax_base*>{expression_list};
}

fundamental_type invocation_expression_syntax::get_return_type(string identifier) const
{
    symbol* symbol = symbol_table::instance().get_symbol(identifier);

    if (symbol == nullptr)
    {
        output::errorUndefFunc(yylineno, identifier);
    }

    return symbol->type;
}

invocation_expression_syntax::~invocation_expression_syntax()
{
    for (syntax_base* child : get_children())
    {
        delete child;
    }
}
