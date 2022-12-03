#ifndef _GENERIC_SYNTAX_HPP_
#define _GENERIC_SYNTAX_HPP_

#include "abstract_syntax.hpp"
#include <vector>
#include <string>
#include <type_traits>

class type_syntax final : public syntax_base
{
    public:

    const fundamental_type type;

    type_syntax(fundamental_type type) : type(type)
    {
    }

    bool is_numeric()
    {
        return type == fundamental_type::Int || type == fundamental_type::Byte;
    }

    bool is_special()
    {
        return type == fundamental_type::String || type == fundamental_type::Void;
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>();
    }

    ~type_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

template<typename element_type> class list_syntax final : public syntax_base
{
    public:

    const std::vector<element_type*> values;

    list_syntax() : values(), syntax_base()
    {
        static_assert(std::is_base_of<syntax_base, element_type>::value, "Must be of type syntax_base");
    }

    list_syntax(element_type* element) : values(element), syntax_base()
    {
        static_assert(std::is_base_of<syntax_base, element_type>::value, "Must be of type syntax_base");

        element->register_parent(this);
    }

    list_syntax(std::vector<element_type*> elements) : values(elements), syntax_base()
    {
        static_assert(std::is_base_of<syntax_base, element_type>::value, "Must be of type syntax_base");

        for (element_type* element : elements)
        {
            element->register_parent(this);
        }
    }

    list_syntax<element_type>* append(element_type* element)
    {
        values.push_back(element);
        return this;
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>(values.begin(), values.end());
    }

    ~list_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

class formal_syntax final : public syntax_base
{
    public:

    type_syntax* const type;
    const std::string identifier;

    formal_syntax(type_syntax* type, std::string identifier) : type(type), identifier(identifier), syntax_base()
    {
        if (type->type == fundamental_type::Void)
        {
            //todo: handle error
        }

        //todo: make sure identifier doesnt shadow anyone else.

        type->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{type};
    }

    ~formal_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
    
};

class function_declaration_syntax final : public syntax_base
{
    public:

    type_syntax* const return_type;
    const std::string identifier;
    list_syntax<formal_syntax>* const formal_list;
    list_syntax<statement_syntax>* const body;

    function_declaration_syntax(type_syntax* return_type, std::string identifier, list_syntax<formal_syntax>* formal_list, list_syntax<statement_syntax>* body) : 
        return_type(return_type), identifier(identifier), formal_list(formal_list), body(body), syntax_base()
    {
        // todo: check that identifier is free

        return_type->register_parent(this);
        formal_list->register_parent(this);
        body->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{return_type, formal_list, body};
    }

    ~function_declaration_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

class root_syntax final : public syntax_base
{
    public:

    list_syntax<function_declaration_syntax>* const function_list;

    root_syntax(list_syntax<function_declaration_syntax>* function_list) : function_list(function_list)
    {
        function_list->register_parent(this);
    }

    std::vector<syntax_base*> children() override
    {
        return std::vector<syntax_base*>{function_list};
    }
    
    ~root_syntax()
    {
        auto nodes = children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

#endif