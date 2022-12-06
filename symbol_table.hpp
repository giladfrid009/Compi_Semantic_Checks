#ifndef _SYMBOL_TABLE_HPP_
#define _SYMBOL_TABLE_HPP_

#include <string>
#include <list>
#include "scope.hpp"

class symbol_table
{
    std::list<scope> scopes;

    private:

    symbol_table();

    public:

    static symbol_table& instance();

    void open_scope(int initial_offset = 0, bool is_implicit = false);

    void close_scope();

    bool contains_symbol(std::string symbol_name) const;

    symbol* get_symbol(std::string symbol_name) const;

    bool add_variable(std::string name, fundamental_type type);

    bool add_function(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types);

    bool add_function(std::string name, fundamental_type return_type);

    const std::list<scope>& get_scopes() const;
};

#endif