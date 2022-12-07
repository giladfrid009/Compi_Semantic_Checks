#include "scope.hpp"
#include "hw3_output.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

using std::string;
using std::vector;

scope::scope(int offset, scope_owner owner) : 
    symbol_list(), symbol_map(), current_offset(offset), owner(owner)
{
}

scope::~scope()
{
    for (symbol* sym : symbol_list)
    {
        delete sym;
    }
}

bool scope::contains_symbol(string symbol_name) const
{
    return symbol_map.find(symbol_name) != symbol_map.end();
}

symbol* scope::get_symbol(string symbol_name) const
{
    if (contains_symbol(symbol_name) == false)
    {
        return nullptr;
    }

    return symbol_map.at(symbol_name);
}

const std::list<symbol*>& scope::get_symbols() const
{
    return symbol_list;
}

bool scope::add_variable(string name, fundamental_type type)
{
    if (contains_symbol(name))
    {
        return false;
    }

    symbol* new_symbol = new variable_symbol(name, type, current_offset);
    symbol_list.push_back(new_symbol);
    symbol_map[name] = new_symbol;

    current_offset += 1;

    return true;
}

bool scope::add_function(string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types)
{
    if (contains_symbol(name))
    {
        return false;
    }

    symbol* new_symbol = new function_symbol(name, return_type, parameter_types);
    symbol_list.push_back(new_symbol);
    symbol_map[name] = new_symbol;

    return true;
}
