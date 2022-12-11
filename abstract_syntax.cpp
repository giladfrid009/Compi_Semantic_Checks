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
        
        default: throw std::invalid_argument("unknown fundamental_type");
    }
}

fundamental_type string_to_fundamental_type(string str)
{
    if (str == "bool") return fundamental_type::Bool;
    if (str == "int") return fundamental_type::Int;
    if (str == "byte") return fundamental_type::Byte;
    if (str == "string") return fundamental_type::String;
    if (str == "void") return fundamental_type::Void;

    throw std::invalid_argument("unknown type");
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
