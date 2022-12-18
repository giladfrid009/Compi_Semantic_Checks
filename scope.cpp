#include "scope.hpp"
#include "hw3_output.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

using std::string;
using std::vector;
using std::list;

scope::scope(int offset, bool is_loop_scope):
    symbol_list(), symbol_map(), current_offset(offset), param_offset(offset - 1), is_loop_scope(is_loop_scope)
{
}

scope::~scope()
{
    for (const symbol* sym : symbol_list)
    {
        delete sym;
    }
}

bool scope::contains_symbol(const string& identifier) const
{
    return symbol_map.find(identifier) != symbol_map.end();
}

const symbol* scope::get_symbol(const string& identifier) const
{
    if (contains_symbol(identifier) == false)
    {
        return nullptr;
    }

    return symbol_map.at(identifier);
}

const list<const symbol*>& scope::get_symbols() const
{
    return symbol_list;
}

bool scope::add_variable(const string& identifier, type_kind type)
{
    if (contains_symbol(identifier))
    {
        return false;
    }

    symbol* new_symbol = new variable_symbol(identifier, type, current_offset);
    symbol_list.push_back(new_symbol);
    symbol_map[identifier] = new_symbol;

    current_offset += 1;

    return true;
}

bool scope::add_parameter(const string& identifier, type_kind type)
{
    if (contains_symbol(identifier))
    {
        return false;
    }

    symbol* new_symbol = new variable_symbol(identifier, type, param_offset);
    symbol_list.push_back(new_symbol);
    symbol_map[identifier] = new_symbol;

    param_offset -= 1;

    return true;
}

bool scope::add_function(const string& identifier, type_kind return_type, const vector<type_kind>& parameter_types)
{
    if (contains_symbol(identifier))
    {
        return false;
    }

    symbol* new_symbol = new function_symbol(identifier, return_type, parameter_types);
    symbol_list.push_back(new_symbol);
    symbol_map[identifier] = new_symbol;

    return true;
}
