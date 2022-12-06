#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <unordered_map>
#include <string>
#include "symbol.hpp"
#include "abstract_syntax.hpp"

class scope
{
    std::unordered_map<std::string, symbol*> symbols;

    public:

    int current_offset;

    int current_serial;

    const bool is_implicit;

    scope(int initial_offset = 0, bool is_implicit = false);

    ~scope();

    bool contains_symbol(std::string symbol_name) const;

    symbol* get_symbol(std::string symbol_name) const;

    bool add_variable(std::string name, fundamental_type type);

    bool add_function(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types);

    void print_symbols() const;
};

#endif