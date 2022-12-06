#ifndef _EXPRESSION_SYNTAX_HPP_
#define _EXPRESSION_SYNTAX_HPP_

#include "syntax_token.hpp"
#include "abstract_syntax.hpp"
#include "generic_syntax.hpp"
#include <vector>
#include <string>
#include <type_traits>
#include <stdexcept>

template<typename literal_type> class literal_expression_syntax final : public expression_syntax
{
    public:

    const literal_type value;
    syntax_token* const value_token;

    literal_expression_syntax(literal_type value, syntax_token* value_token) : 
        expression_syntax(get_return_type()), value(value), value_token(value_token)
    {
    }

    std::vector<syntax_base*> get_children() const override
    {
        return std::vector<syntax_base*>();
    }

    std::vector<syntax_token*> get_tokens() const override
    {
        return std::vector<syntax_token*>{value_token};
    }

    literal_expression_syntax(const literal_expression_syntax& other) = delete;

    literal_expression_syntax& operator=(const literal_expression_syntax& other) = delete;

    fundamental_type get_return_type() const
    {
        if (std::is_same<literal_type, char>::value) return fundamental_type::Byte;
        if (std::is_same<literal_type, int>::value) return fundamental_type::Byte;
        if (std::is_same<literal_type, bool>::value) return fundamental_type::Bool;
        if (std::is_same<literal_type, std::string>::value) return fundamental_type::String;
        if (std::is_same<literal_type, void>::value) return fundamental_type::Void;

        throw std::runtime_error("invalid literal_type");
    }

    ~literal_expression_syntax()
    {
        auto nodes = get_children();

        for (syntax_base* child : nodes)
        {
            delete child;
        }
    }
};

class cast_expression_syntax final : public expression_syntax
{
    public:

    type_syntax* const destination_type;
    expression_syntax* const expression;

    cast_expression_syntax(type_syntax* destination_type, expression_syntax* expression);

    cast_expression_syntax(const cast_expression_syntax& other) = delete;

    cast_expression_syntax& operator=(const cast_expression_syntax& other) = delete;

    std::vector<syntax_base*> get_children() const override;

    std::vector<syntax_token*> get_tokens() const override;

    ~cast_expression_syntax();
};

class not_expression_syntax final : public expression_syntax
{
    public:

    syntax_token* const not_token;
    expression_syntax* const expression;

    not_expression_syntax(syntax_token* not_token, expression_syntax* expression);

    not_expression_syntax(const not_expression_syntax& other) = delete;

    not_expression_syntax& operator=(const not_expression_syntax& other) = delete;

    std::vector<syntax_base*> get_children() const override;

    std::vector<syntax_token*> get_tokens() const override;

    ~not_expression_syntax();
};

class logical_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const left;
    syntax_token* const oper_token;
    expression_syntax* const right;
    const logical_operator oper;

    logical_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);

    logical_expression_syntax(const logical_expression_syntax& other) = delete;

    logical_expression_syntax& operator=(const logical_expression_syntax& other) = delete;

    std::vector<syntax_base*> get_children() const override;

    std::vector<syntax_token*> get_tokens() const override;

    ~logical_expression_syntax();
};

class arithmetic_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const left;
    syntax_token* const oper_token;
    expression_syntax* const right;
    const arithmetic_operator oper;

    arithmetic_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);

    arithmetic_expression_syntax(const arithmetic_expression_syntax& other) = delete;

    arithmetic_expression_syntax& operator=(const arithmetic_expression_syntax& other) = delete;

    fundamental_type get_return_type(expression_syntax* left, expression_syntax* right);

    std::vector<syntax_base*> get_children() const override;

    std::vector<syntax_token*> get_tokens() const override;

    ~arithmetic_expression_syntax();
};

class relational_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const left;
    syntax_token* const oper_token;
    expression_syntax* const right;
    const relational_operator oper;

    relational_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);

    relational_expression_syntax(const relational_expression_syntax& other) = delete;

    relational_expression_syntax& operator=(const relational_expression_syntax& other) = delete;

    std::vector<syntax_base*> get_children() const override;

    std::vector<syntax_token*> get_tokens() const override;

    ~relational_expression_syntax();
};

class conditional_expression_syntax final : public expression_syntax
{
    public:

    expression_syntax* const true_value;
    syntax_token* const if_token;
    expression_syntax* const condition;
    syntax_token* const else_token;
    expression_syntax* const false_value;

    conditional_expression_syntax(expression_syntax* true_value, syntax_token* if_token, expression_syntax* condition, syntax_token* const else_token, expression_syntax* false_value);

    conditional_expression_syntax(const conditional_expression_syntax& other) = delete;

    conditional_expression_syntax& operator=(const conditional_expression_syntax& other) = delete;

    fundamental_type get_return_type(expression_syntax* left, expression_syntax* right) const;

    std::vector<syntax_base*> get_children() const override;

    std::vector<syntax_token*> get_tokens() const override;

    ~conditional_expression_syntax();
};

class identifier_expression_syntax final : public expression_syntax
{
    public:

    syntax_token* const identifier_token;
    const std::string identifier;

    identifier_expression_syntax(syntax_token* identifier_token);

    identifier_expression_syntax(const identifier_expression_syntax& other) = delete;

    identifier_expression_syntax& operator=(const identifier_expression_syntax& other) = delete;

    fundamental_type get_return_type(std::string identifier) const;
    
    std::vector<syntax_base*> get_children() const override;

    std::vector<syntax_token*> get_tokens() const override;

    ~identifier_expression_syntax();
};

class invocation_expression_syntax final : public expression_syntax
{
    public:

    syntax_token* const identifier_token;
    const std::string identifier;
    list_syntax<expression_syntax>* const expression_list;

    invocation_expression_syntax(syntax_token* identifier_token);

    invocation_expression_syntax(syntax_token* identifier_token, list_syntax<expression_syntax>* expression_list);

    invocation_expression_syntax(const invocation_expression_syntax& other) = delete;

    invocation_expression_syntax& operator=(const invocation_expression_syntax& other) = delete;

    fundamental_type get_return_type(std::string identifier) const;

    std::vector<syntax_base*> get_children() const override;
    
    std::vector<syntax_token*> get_tokens() const override;

    ~invocation_expression_syntax();
};

#endif