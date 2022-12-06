#include "symbol.hpp"
#include <vector>

using std::string;
using std::vector;

symbol::symbol(string name, fundamental_type type, symbol_type sym_type, int serial_number) : 
    sym_type(sym_type), name(name), type(type), serial_number(serial_number)
{
    
}

variable_symbol::variable_symbol(string name, fundamental_type type, int offset, int serial_number) :
    symbol(name, type, symbol_type::Var, serial_number), offset(offset)
{

}

function_symbol::function_symbol(string name, fundamental_type return_type, vector<fundamental_type> parameter_types, int serial_number) :
    symbol(name, return_type, symbol_type::Func, serial_number), parameter_types(parameter_types)
{
    for (auto param : parameter_types)
    {
        if (param == fundamental_type::Void)
        {
            // handle error
        }
    }
}