#include "symbol.hpp"
#include <vector>

using std::string;
using std::vector;

symbol::symbol(string name, type_kind type, int offset, symbol_kind kind):
    kind(kind), name(name), offset(offset), type(type)
{

}

variable_symbol::variable_symbol(string name, type_kind type, int offset):
    symbol(name, type, offset, symbol_kind::Variable)
{

}

function_symbol::function_symbol(string name, type_kind return_type, vector<type_kind> parameter_types):
    symbol(name, return_type, 0, symbol_kind::Function), parameter_types(parameter_types)
{

}