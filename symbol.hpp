#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>
#include <vector>
#include "abstract_syntax.hpp"

enum class symbol_kind { Variable, Function };

class symbol
{
    public:

    const symbol_kind kind;

    const std::string identifier;

    const int offset;

    const type_kind type;

    protected:

    symbol(const std::string& identifier, type_kind type, int offset, symbol_kind kind);

    public:

    virtual ~symbol() = default;
};

class variable_symbol: public symbol
{
    public:

    variable_symbol(const std::string& identifier, type_kind type, int offset);
};

class function_symbol: public symbol
{
    public:

    const std::vector<type_kind> parameter_types;

    function_symbol(const std::string& identifier, type_kind return_type, const std::vector<type_kind>& parameter_types);
};

#endif