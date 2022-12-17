#ifndef _SYMBOL_TABLE_HPP_
#define _SYMBOL_TABLE_HPP_

#include <string>
#include <list>
#include "scope.hpp"

class symbol_table
{
    std::list<scope> scope_list;

    private:

    symbol_table();

    public:

    static symbol_table& instance();

    void open_scope(bool is_loop_scope = false);

    void close_scope();

    const scope& current_scope() const;

    bool contains_symbol(std::string symbol_name) const;

    symbol* get_symbol(std::string symbol_name) const;

    bool add_variable(std::string name, type_kind type);

    bool add_parameter(std::string name, type_kind type);

    bool add_function(std::string name, type_kind return_type, std::vector<type_kind> parameter_types);

    bool add_function(std::string name, type_kind return_type);

    const std::list<scope>& get_scopes() const;
};

#endif