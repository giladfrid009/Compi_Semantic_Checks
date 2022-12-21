#include "symbol.hpp"
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;

symbol::symbol(const string& name, type_kind type, int offset, symbol_kind kind):
    kind(kind), name(name), offset(offset), type(type)
{

}

variable_symbol::variable_symbol(const string& name, type_kind type, int offset):
    symbol(name, type, offset, symbol_kind::Variable)
{

}

string variable_symbol::to_string() const
{
    stringstream res;

    res << name << " " << types::to_string(type) << " " << offset;

    return res.str();
}

function_symbol::function_symbol(const string& name, type_kind return_type, const vector<type_kind>& parameter_types):
    symbol(name, return_type, 0, symbol_kind::Function), parameter_types(parameter_types)
{

}

string function_symbol::to_string() const
{
    stringstream res;

    res << name << " " << "(";

    for (size_t i = 0; i < parameter_types.size(); i++)
    {
        res << types::to_string(parameter_types[i]);

        if (i + 1 < parameter_types.size())
        {
            res << ",";
        }
    }

    res << ")" << "->" << types::to_string(type) << " " << offset;
    
    return res.str();
}