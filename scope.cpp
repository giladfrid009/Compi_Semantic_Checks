#include "scope.hpp"
#include "hw3_output.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

using std::string;
using std::vector;

scope::scope(int initial_offset) : symbols(), current_offset(initial_offset), current_serial(0)
{
}

scope::~scope()
{
    for (auto& key_val : symbols)
    {
        delete key_val.second;
    }
}

bool scope::contains_symbol(string symbol_name) const
{
    return symbols.find(symbol_name) != symbols.end();
}

symbol* scope::get_symbol(string symbol_name) const
{
    if (contains_symbol(symbol_name) == false)
    {
        return nullptr;
    }

    return symbols.at(symbol_name);
}

bool scope::add_variable(string name, fundamental_type type)
{
    if (contains_symbol(name))
    {
        return false;
    }

    symbols[name] = new variable_symbol(name, type, current_offset, current_serial);

    current_offset += 1;
    current_serial += 1;

    return true;
}

bool scope::add_function(string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types)
{
    if (contains_symbol(name))
    {
        return false;
    }

    symbols[name] = new function_symbol(name, return_type, parameter_types, current_serial);

    current_serial += 1;

    return true;
}

void scope::print_symbols() const
{
    vector<symbol*> symbols_sorted;

    for (auto& kv : symbols)
    {
        symbols_sorted.push_back(kv.second);
    }

    std::sort(symbols_sorted.begin(), symbols_sorted.end(), [] (symbol* a, symbol* b) { return a->serial_number < b->serial_number ;});

    for (symbol* sym : symbols_sorted)
    {
        if (sym->sym_type == symbol_type::Var)
        {
            variable_symbol* var_sym = dynamic_cast<variable_symbol*>(sym);

            output::printID(var_sym->name, var_sym->offset, fundamental_type_to_string(var_sym->type));
        }
        else if (sym->sym_type == symbol_type::Func)
        {
            function_symbol* func_sym = dynamic_cast<function_symbol*>(sym);

            vector<string> param_types;

            for (fundamental_type ft : func_sym->parameter_types)
            {
                param_types.push_back(fundamental_type_to_string(ft));
            }

            string func_type = output::makeFunctionType(fundamental_type_to_string(func_sym->type), param_types);

            output::printID(func_sym->name, 0, func_type);
        }
        else
        {
            throw std::runtime_error("unknown symbol type");
        }
    }
}
