#include "abstract_syntax.hpp"
#include "hw3_output.hpp"
#include <stdexcept>

extern int yylineno;

std::string fundamental_type_to_string(fundamental_type type)
{
    switch(type)
    {
        case (fundamental_type::Bool): return "bool";
        case (fundamental_type::Int): return "int";
        case (fundamental_type::Byte): return "byte";
        case (fundamental_type::String): return "string";
        case (fundamental_type::Void): return "void";
    }

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
    if (is_special())
    {
        output::errorMismatch(yylineno);
    }
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
