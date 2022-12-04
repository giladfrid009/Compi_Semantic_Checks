#include "expression_syntax.hpp"
#include <stdexcept>

using std::string;
using std::vector;

cast_expression_syntax::cast_expression_syntax(type_syntax* destination_type, expression_syntax* expression) :
    expression_syntax(destination_type->type), destination_type(destination_type), expression(expression)
{
    if (expression->is_numeric() == false)
    {
        // todo: handle illigal cast
    }

    if (destination_type->is_numeric() == false)
    {
        // todo: handle illigal cast
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
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

not_expression_syntax::not_expression_syntax(expression_syntax* expression) :
    expression_syntax(fundamental_type::Bool), expression(expression)
{
    if (expression->expression_return_type != fundamental_type::Bool)
    {
        // todo: handle illigal return_type
    }

    expression->set_parent(this);
}

vector<syntax_base*> not_expression_syntax::get_children() const
{
    return vector<syntax_base*>{expression};
}

not_expression_syntax::~not_expression_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

logical_expression_syntax::logical_expression_syntax(expression_syntax* left, expression_syntax* right, logical_operator oper) :
    expression_syntax(fundamental_type::Bool), left(left), right(right), oper(oper)
{
    if (left->expression_return_type != fundamental_type::Bool || right->expression_return_type != fundamental_type::Bool)
    {
        // todo: handle error
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
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

arithmetic_expression_syntax::arithmetic_expression_syntax(expression_syntax* left, expression_syntax* right, arithmetic_operator oper) :
    expression_syntax(get_return_type(left, right)), left(left), right(right), oper(oper)
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        // todo: handle error
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
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

relational_expression_syntax::relational_expression_syntax(expression_syntax* left, expression_syntax* right, relational_operator oper) :
    expression_syntax(fundamental_type::Bool), left(left), right(right), oper(oper)
{
    if (left->is_numeric() == false || right->is_numeric() == false)
    {
        //todo: handle error        
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
    auto nodes = get_children();

    for (syntax_base* child : nodes)
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
            // todo: handle error
        }
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

    return fundamental_type::Void;
}

conditional_expression_syntax::~conditional_expression_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

identifier_expression_syntax::identifier_expression_syntax(string identifier) : 
    expression_syntax(get_return_type(identifier)), identifier(identifier) 
{
    //todo: add checks if identifier exists in symbol table
}

vector<syntax_base*> identifier_expression_syntax::get_children() const
{
    return vector<syntax_base*>();
}

fundamental_type identifier_expression_syntax::get_return_type(string identifier) const
{
    //todo: implement with symbol table
    return fundamental_type::Void;
}

identifier_expression_syntax::~identifier_expression_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}

invocation_expression_syntax::invocation_expression_syntax(string identifier) :
    expression_syntax(get_return_type(identifier)), identifier(identifier), expression_list(nullptr)
{
    // todo: check that identifier exists    
}

invocation_expression_syntax::invocation_expression_syntax(string identifier, list_syntax<expression_syntax>* expression_list) :
    expression_syntax(get_return_type(identifier)), identifier(identifier), expression_list(expression_list)
{
    // todo: check that identifier exists
    // todo: check that expression_list matches signature

    expression_list->set_parent(this);
}

vector<syntax_base*> invocation_expression_syntax::get_children() const
{
    return vector<syntax_base*>{expression_list};
}

fundamental_type invocation_expression_syntax::get_return_type(string identifier) const
{
    //todo: implement with symbol table
    return fundamental_type::Void;
}

invocation_expression_syntax::~invocation_expression_syntax()
{
    auto nodes = get_children();

    for (syntax_base* child : nodes)
    {
        delete child;
    }
}
