#include "syntax_base.hpp"

syntax_base* syntax_base::parent()
{
    return parent_syntax;
}

void syntax_base::register_parent(syntax_base* new_parent)
{
    parent_syntax = new_parent;
}