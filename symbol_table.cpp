#include "symbol_table.hpp"

void symbol_table::open_scope(int initial_offset)
{
    scopes.push_front(scope(initial_offset));
}

void symbol_table::close_scope()
{
    scopes.pop_front();
}

bool symbol_table::contains_symbol(std::string symbol_name) const
{
    for (const scope& sc : scopes)
    {
        if (sc.contains_symbol(symbol_name))
        {
            return true;
        }
    }

    return false;
}

symbol* symbol_table::get_symbol(std::string symbol_name) const
{
    for (const scope& sc : scopes)
    {
        if (sc.contains_symbol(symbol_name))
        {
            return sc.get_symbol(symbol_name);
        }
    }

    return nullptr;
}

bool symbol_table::add_variable(std::string name, fundamental_type type)
{
    if (scopes.size() == 0)
    {
        return false;
    }

    return scopes.front().add_variable(name, type);
}

bool symbol_table::add_function(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types)
{
    if (scopes.size() == 0)
    {
        return false;
    }

    return scopes.front().add_function(name, return_type, parameter_types);
}

bool symbol_table::add_function(std::string name, fundamental_type return_type)
{
    return add_function(name, return_type, std::vector<fundamental_type>());
}