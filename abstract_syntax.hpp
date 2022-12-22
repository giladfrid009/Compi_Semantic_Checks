#ifndef _ABSTRACT_SYNTAX_HPP_
#define _ABSTRACT_SYNTAX_HPP_

#include "syntax_token.hpp"
#include "types.hpp"
#include <vector>
#include <string>
#include <list>

class syntax_base
{
    private:

    std::list<syntax_base*> children;
    syntax_base* parent = nullptr;

    protected:

    syntax_base();

    public:

    syntax_base(const syntax_base& other) = delete;

    syntax_base& operator=(const syntax_base& other) = delete;

    const syntax_base* get_parent() const;

    const std::list<syntax_base*>& get_children() const;

    virtual ~syntax_base() = default;

    protected:

    void push_back_child(syntax_base* child);

    void push_front_child(syntax_base* child);
};

class expression_syntax: public syntax_base
{
    public:

    const type_kind return_type;

    protected:

    expression_syntax(type_kind return_type);

    public:

    expression_syntax(const expression_syntax& other) = delete;

    expression_syntax& operator=(const expression_syntax& other) = delete;

    bool is_numeric() const;

    bool is_special() const;

    virtual ~expression_syntax() = default;
};

class statement_syntax: public syntax_base
{
    protected:

    statement_syntax();

    public:

    virtual ~statement_syntax() = default;

    statement_syntax(const statement_syntax& other) = delete;

    statement_syntax& operator=(const statement_syntax& other) = delete;
};

#endif