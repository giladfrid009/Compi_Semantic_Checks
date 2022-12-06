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

    const int serial_number;

    symbol(std::string name, fundamental_type type, symbol_type sym_type, int serial_number);

    virtual ~symbol() = default;
};

class variable_symbol : public symbol
{
    public:

    const int offset;

    variable_symbol(std::string name, fundamental_type type, int offset, int serial_number);
};

class function_symbol : public symbol
{
    public:

    const std::vector<fundamental_type> parameter_types;

    function_symbol(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types, int serial_number);
};

#endif