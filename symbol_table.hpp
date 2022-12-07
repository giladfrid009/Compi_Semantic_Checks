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

    void open_scope(int offset = 0, scope_owner owner = scope_owner::None);

    void close_scope();

    const scope& current_scope() const;

    bool contains_symbol(std::string symbol_name) const;

    symbol* get_symbol(std::string symbol_name) const;

    bool add_variable(std::string name, fundamental_type type);

    bool add_function(std::string name, fundamental_type return_type, std::vector<fundamental_type> parameter_types);

    bool add_function(std::string name, fundamental_type return_type);

    const std::list<scope>& get_scopes() const;
};

#endif