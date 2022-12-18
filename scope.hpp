#ifndef _SCOPE_HPP_
#define _SCOPE_HPP_

#include <unordered_map>
#include <list>
#include <string>
#include "symbol.hpp"
#include "abstract_syntax.hpp"

class scope
{
    friend class symbol_table;

    private:

    std::list<symbol*> symbol_list;
    std::unordered_map<std::string, symbol*> symbol_map;

    int current_offset;
    int param_offset;

    public:

    const bool is_loop_scope;

    scope(int offset, bool is_loop_scope);

    ~scope();

    bool contains_symbol(const std::string& identifier) const;

    const symbol* get_symbol(const std::string& identifier) const;

    const std::list<symbol*>& get_symbols() const;

    bool add_variable(const std::string& identifier, type_kind type);

    bool add_parameter(const std::string& identifier, type_kind type);

    bool add_function(const std::string& identifier, type_kind return_type, const std::vector<type_kind>& parameter_types);
};

#endif