#ifndef _EXPRESSION_SYNTAX_HPP_
#define _EXPRESSION_SYNTAX_HPP_

#include "syntax_token.hpp"
#include "abstract_syntax.hpp"
#include "generic_syntax.hpp"
#include "output.hpp"
#include <vector>
#include <string>
#include <type_traits>
#include <stdexcept>

template<typename literal_type> class literal_expression final: public expression_syntax
{
    public:

    const syntax_token* const value_token;
    const literal_type value;

    literal_expression(syntax_token* value_token):
        expression_syntax(get_return_type()), value_token(value_token), value(get_literal_value(value_token))
    {
    }

    literal_expression(const literal_expression& other) = delete;

    literal_expression& operator=(const literal_expression& other) = delete;

    type_kind get_return_type() const
    {
        if (std::is_same<literal_type, char>::value) return type_kind::Byte;
        if (std::is_same<literal_type, int>::value) return type_kind::Int;
        if (std::is_same<literal_type, bool>::value) return type_kind::Bool;
        if (std::is_same<literal_type, std::string>::value) return type_kind::String;

        throw std::runtime_error("invalid literal_type");
    }

    inline literal_type get_literal_value(syntax_token* value_token) const
    {
        throw std::runtime_error("invalid literal_type");
    }

    ~literal_expression()
    {
        for (syntax_base* child : get_children())
        {
            delete child;
        }

        delete value_token;
    }
};

template<> inline int literal_expression<int>::get_literal_value(syntax_token* value_token) const
{
    return std::stoi(value_token->text);
}

template<> inline char literal_expression<char>::get_literal_value(syntax_token* value_token) const
{
    int value = std::stoi(value_token->text);

    if (value < 0 || value > 255)
    {
        output::error_byte_too_large(value_token->position, value_token->text);
    }

    return static_cast<char>(value);
}

template<> inline std::string literal_expression<std::string>::get_literal_value(syntax_token* value_token) const
{
    return std::string(value_token->text);
}

template<> inline bool literal_expression<bool>::get_literal_value(syntax_token* value_token) const
{
    if (value_token->text == "true") return true;
    if (value_token->text == "false") return false;

    throw std::runtime_error("invalid value_token text");
}

class cast_expression final: public expression_syntax
{
    public:

    const type_syntax* const destination_type;
    const expression_syntax* const expression;

    cast_expression(type_syntax* destination_type, expression_syntax* expression);
    ~cast_expression();

    cast_expression(const cast_expression& other) = delete;
    cast_expression& operator=(const cast_expression& other) = delete;
};

class not_expression final: public expression_syntax
{
    public:

    const syntax_token* const not_token;
    const expression_syntax* const expression;

    not_expression(syntax_token* not_token, expression_syntax* expression);
    ~not_expression();

    not_expression(const not_expression& other) = delete;
    not_expression& operator=(const not_expression& other) = delete;
};

class logical_expression final: public expression_syntax
{
    public:

    enum class operator_kind { And, Or };

    const expression_syntax* const left;
    const syntax_token* const oper_token;
    const expression_syntax* const right;
    const operator_kind oper;

    logical_expression(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);
    ~logical_expression();

    logical_expression(const logical_expression& other) = delete;
    logical_expression& operator=(const logical_expression& other) = delete;

    static operator_kind parse_operator(std::string str);
};

class arithmetic_expression final: public expression_syntax
{
    public:

    enum class operator_kind { Add, Sub, Mul, Div };

    const expression_syntax* const left;
    const syntax_token* const oper_token;
    const expression_syntax* const right;
    const operator_kind oper;

    arithmetic_expression(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);
    ~arithmetic_expression();

    arithmetic_expression(const arithmetic_expression& other) = delete;
    arithmetic_expression& operator=(const arithmetic_expression& other) = delete;

    static operator_kind parse_operator(std::string str);
};

class relational_expression final: public expression_syntax
{
    public:

    enum class operator_kind { Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual };

    const expression_syntax* const left;
    const syntax_token* const oper_token;
    const expression_syntax* const right;
    const operator_kind oper;

    relational_expression(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);
    ~relational_expression();

    relational_expression(const relational_expression& other) = delete;
    relational_expression& operator=(const relational_expression& other) = delete;

    static operator_kind parse_operator(std::string str);
};

class conditional_expression final: public expression_syntax
{
    public:

    const expression_syntax* const true_value;
    const syntax_token* const if_token;
    const expression_syntax* const condition;
    const syntax_token* const else_token;
    const expression_syntax* const false_value;

    conditional_expression(expression_syntax* true_value, syntax_token* if_token, expression_syntax* condition, syntax_token* const else_token, expression_syntax* false_value);
    ~conditional_expression();

    conditional_expression(const conditional_expression& other) = delete;
    conditional_expression& operator=(const conditional_expression& other) = delete;

    private:

    static type_kind get_return_type(expression_syntax* left, expression_syntax* right);
};

class identifier_expression final: public expression_syntax
{
    public:

    const syntax_token* const identifier_token;
    const std::string identifier;

    identifier_expression(syntax_token* identifier_token);
    ~identifier_expression();

    identifier_expression(const identifier_expression& other) = delete;
    identifier_expression& operator=(const identifier_expression& other) = delete;

    private:

    static type_kind get_return_type(std::string identifier);
};

class invocation_expression final: public expression_syntax
{
    public:

    const syntax_token* const identifier_token;
    const std::string identifier;
    const list_syntax<expression_syntax>* const expression_list;

    invocation_expression(syntax_token* identifier_token);
    invocation_expression(syntax_token* identifier_token, list_syntax<expression_syntax>* expression_list);
    ~invocation_expression();

    invocation_expression(const invocation_expression& other) = delete;
    invocation_expression& operator=(const invocation_expression& other) = delete;

    private:

    static type_kind get_return_type(std::string identifier);
};

#endif