#include "abstract_syntax.hpp"

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
        //todo: handle illigal type
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
