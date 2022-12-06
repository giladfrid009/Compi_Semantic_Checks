#include "abstract_syntax.hpp"
#include "hw3_output.hpp"
#include <stdexcept>
#include <string>

std::string fundamental_type_to_string(fundamental_type type)
{
    switch(type)
    {
        case (fundamental_type::Bool): return "BOOL";
        case (fundamental_type::Int): return "INT";
        case (fundamental_type::Byte): return "BYTE";
        case (fundamental_type::String): return "STRING";
        case (fundamental_type::Void): return "VOID";
        default: throw std::invalid_argument("unknown type");
    }
}

fundamental_type string_to_fundamental_type(string type)
{
    if (type == "bool") return fundamental_type::Bool;
    if (type == "int") return fundamental_type::Int;
    if (type == "byte") return fundamental_type::Byte;
    if (type == "string") return fundamental_type::String;
    if (type == "void") return fundamental_type::Void;

    throw std::invalid_argument("unknown type");
}

logical_operator string_to_logical_operator(string oper)
{
    if (oper == "and") return logical_operator::And;
    if (oper == "or") return logical_operator::Or;

    throw std::invalid_argument("unknown oper");
}

arithmetic_operator string_to_arithmetic_operator(string oper)
{
    if (oper == "+") return arithmetic_operator::Add;
    if (oper == "-") return arithmetic_operator::Sub;
    if (oper == "*") return arithmetic_operator::Mul;
    if (oper == "/") return arithmetic_operator::Div;

    throw std::invalid_argument("unknown oper");
}

relational_operator string_to_relational_operator(string oper)
{
    if (oper == "<") return relational_operator::Less;
    if (oper == "<=") return relational_operator::LessEqual;
    if (oper == ">") return relational_operator::Greater;
    if (oper == ">=") return relational_operator::GreaterEqual;
    if (oper == "==") return relational_operator::Equal;
    if (oper == "!=") return relational_operator::NotEqual;

    throw std::invalid_argument("unknown oper");
}

branch_type string_to_branch_type(string oper)
{
    if (oper == "break") return branch_type::Break;
    if (oper == "continue") return branch_type::Continue;

    throw std::invalid_argument("unknown oper");
}

syntax_base::syntax_base()
{
}

syntax_base* syntax_base::get_parent() const
{
    return parent;
}

void syntax_base::set_parent(syntax_base* new_parent)
{
    parent = new_parent;
}

expression_syntax::expression_syntax(fundamental_type return_type) : expression_return_type(return_type)
{
    
}

bool expression_syntax::is_numeric() const
{
    return expression_return_type == fundamental_type::Int || expression_return_type == fundamental_type::Byte;
}

bool expression_syntax::is_special() const
{
    return expression_return_type == fundamental_type::String || expression_return_type == fundamental_type::Void;
}

statement_syntax::statement_syntax()
{
}
