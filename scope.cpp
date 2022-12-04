#include "scope.hpp"

scope::scope(int initial_offset)
{
    current_offset = initial_offset;
}

scope::~scope()
{
    for (auto key_val : symbols)
    {
        delete key_val.second;
    }
}

bool scope::contains_symbol(std::string symbol_name) const
{
    return symbols.find(symbol_name) != symbols.end();
}

symbol* scope::get_symbol(std::string symbol_name) const
{
    if (contains_symbol(symbol_name) == false)
    {
        return nullptr;
    }

    return symbols.at(symbol_name);
}

bool scope::add_variable(std::string name, fundamental_type type)
{
    if (contains_symbol(name))
    {
        return false;
    }

    symbols[name] = new variable_symbol(name, type, current_offset);

    current_offset += 1;
}

bool scope::add_function(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types)
{
    if (contains_symbol(name))
    {
        return false;
    }

    symbols[name] = new function_symbol(name, return_type, parameter_types);
}
