#include "symbol.hpp"
#include <vector>

using std::string;
using std::vector;

symbol::symbol(string name, fundamental_type type, symbol_type sym_type) : 
    sym_type(sym_type), name(name), type(type)
{
    
}

variable_symbol::variable_symbol(string name, fundamental_type type, int offset) :
    symbol(name, type, symbol_type::Var), offset(offset)
{

}

function_symbol::function_symbol(string name, fundamental_type return_type, vector<fundamental_type> parameter_types) :
    symbol(name, return_type, symbol_type::Func), parameter_types(parameter_types)
{
    
}