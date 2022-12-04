#include "symbol.hpp"
#include <vector>

symbol::symbol(std::string name, fundamental_type type, symbol_type sym_type) : sym_type(sym_type), name(name), type(type)
{

}

variable_symbol::variable_symbol(std::string name, fundamental_type type, int offset) : 
    offset(offset), symbol(name, type, symbol_type::Var)
{

}

function_symbol::function_symbol(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types) : 
    parameter_types(parameter_types), symbol(name, return_type, symbol_type::Func)
{
    for (auto param : parameter_types)
    {
        if (param == fundamental_type::Void)
        {
            // handle error
        }
    }
}