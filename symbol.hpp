#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>
#include <vector>
#include "abstract_syntax.hpp"

enum class symbol_type { Var, Func };

class symbol
{
    public:

    const symbol_type sym_type;

    const std::string name;

    const fundamental_type type;

    symbol(std::string name, fundamental_type type, symbol_type sym_type);
};

class variable_symbol : public symbol
{
    public:

    const int offset;

    variable_symbol(std::string name, fundamental_type type, int offset);
};

class function_symbol : public symbol
{
    public:

    const std::vector<fundamental_type> parameter_types;

    function_symbol(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types);
};

#endif