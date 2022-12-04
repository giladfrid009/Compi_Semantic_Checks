#ifndef _ABSTRACT_SYNTAX_HPP_
#define _ABSTRACT_SYNTAX_HPP_

#include <string>
#include <vector>

enum class logical_operator { And, Or };

enum class arithmetic_operator { Add, Sub, Mul, Div };

enum class relational_operator { Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual };

enum class fundamental_type { Void, Int, Bool, Byte, String };

class syntax_base
{
    private:

    syntax_base* parent = nullptr;

    public:

    syntax_base()
    {
    }

    syntax_base* get_parent()
    {
        return parent;
    }

    void set_parent(syntax_base* new_parent)
    {
        parent = new_parent;
    }

    virtual ~syntax_base() = default;

    virtual std::vector<syntax_base*> get_children() = 0;
};

class expression_syntax : public syntax_base
{
    public:

    const fundamental_type expression_return_type;

    expression_syntax(fundamental_type return_type) : expression_return_type(return_type)
    {
        if (is_special())
        {
            //todo: handle illigal type
        }
    }

    bool is_numeric()
    {
        return expression_return_type == fundamental_type::Int || expression_return_type == fundamental_type::Byte;
    }

    bool is_special()
    {
        return expression_return_type == fundamental_type::String || expression_return_type == fundamental_type::Void;
    }

    virtual ~expression_syntax() = default;

    virtual std::vector<syntax_base*> get_children() = 0;
};

enum class branch_type { Break, Continue };

class statement_syntax : public syntax_base
{
    public:

    virtual ~statement_syntax() = default;

    virtual std::vector<syntax_base*> get_children() = 0;
};

#endif