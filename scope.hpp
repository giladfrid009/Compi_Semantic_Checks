#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <unordered_map>
#include <list>
#include <string>
#include "symbol.hpp"
#include "abstract_syntax.hpp"

enum class scope_owner { If, While, Func, None };

class scope
{
    friend class symbol_table;

    std::list<symbol*> symbol_list;
    std::unordered_map<std::string, symbol*> symbol_map;

    int current_offset;

    public:

    const scope_owner owner;

    scope(int offset, scope_owner owner);

    ~scope();

    bool contains_symbol(std::string symbol_name) const;

    symbol* get_symbol(std::string symbol_name) const;

    const std::list<symbol*>& get_symbols() const;

    bool add_variable(std::string name, fundamental_type type);

    bool add_function(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types);

    void print_symbols() const;
};

#endif