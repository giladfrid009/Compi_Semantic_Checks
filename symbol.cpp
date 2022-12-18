#include "symbol.hpp"
#include <vector>

using std::string;
using std::vector;

symbol::symbol(const string& identifier, type_kind type, int offset, symbol_kind kind):
    kind(kind), identifier(identifier), offset(offset), type(type)
{

}

variable_symbol::variable_symbol(const string& identifier, type_kind type, int offset):
    symbol(identifier, type, offset, symbol_kind::Variable)
{

}

function_symbol::function_symbol(const string& identifier, type_kind return_type, const vector<type_kind>& parameter_types):
    symbol(identifier, return_type, 0, symbol_kind::Function), parameter_types(parameter_types)
{

}