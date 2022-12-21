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

template<typename literal_type> class literal_expression_syntax final: public expression_syntax
{
    public:

    const syntax_token* const value_token;
    const literal_type value;

    literal_expression_syntax(syntax_token* value_token):
        expression_syntax(get_return_type()), value_token(value_token), value(get_literal_value(value_token))
    {
    }

    literal_expression_syntax(const literal_expression_syntax& other) = delete;

    literal_expression_syntax& operator=(const literal_expression_syntax& other) = delete;

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

    ~literal_expression_syntax()
    {
        for (syntax_base* child : get_children())
        {
            delete child;
        }

        delete value_token;
    }
};

template<> inline int literal_expression_syntax<int>::get_literal_value(syntax_token* value_token) const
{
    return std::stoi(value_token->text);
}

template<> inline char literal_expression_syntax<char>::get_literal_value(syntax_token* value_token) const
{
    int value = std::stoi(value_token->text);

    if (value < 0 || value > 255)
    {
        output::error_byte_too_large(value_token->position, value_token->text);
    }

    return static_cast<char>(value);
}

template<> inline std::string literal_expression_syntax<std::string>::get_literal_value(syntax_token* value_token) const
{
    return std::string(value_token->text);
}

template<> inline bool literal_expression_syntax<bool>::get_literal_value(syntax_token* value_token) const
{
    if (value_token->text == "true") return true;
    if (value_token->text == "false") return false;

    throw std::runtime_error("invalid value_token text");
}

class cast_expression_syntax final: public expression_syntax
{
    public:

    const type_syntax* const destination_type;
    const expression_syntax* const expression;

    cast_expression_syntax(type_syntax* destination_type, expression_syntax* expression);

    cast_expression_syntax(const cast_expression_syntax& other) = delete;

    cast_expression_syntax& operator=(const cast_expression_syntax& other) = delete;

    ~cast_expression_syntax();
};

class not_expression_syntax final: public expression_syntax
{
    public:

    const syntax_token* const not_token;
    const expression_syntax* const expression;

    not_expression_syntax(syntax_token* not_token, expression_syntax* expression);

    not_expression_syntax(const not_expression_syntax& other) = delete;

    not_expression_syntax& operator=(const not_expression_syntax& other) = delete;

    ~not_expression_syntax();
};

class logical_expression_syntax final: public expression_syntax
{
    public:

    enum class operator_kind { And, Or };

    const expression_syntax* const left;
    const syntax_token* const oper_token;
    const expression_syntax* const right;
    const operator_kind oper;

    logical_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);

    logical_expression_syntax(const logical_expression_syntax& other) = delete;

    logical_expression_syntax& operator=(const logical_expression_syntax& other) = delete;

    ~logical_expression_syntax();

    static operator_kind parse_operator(std::string str);
};

class arithmetic_expression_syntax final: public expression_syntax
{
    public:

    enum class operator_kind { Add, Sub, Mul, Div };

    const expression_syntax* const left;
    const syntax_token* const oper_token;
    const expression_syntax* const right;
    const operator_kind oper;

    arithmetic_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);

    arithmetic_expression_syntax(const arithmetic_expression_syntax& other) = delete;

    arithmetic_expression_syntax& operator=(const arithmetic_expression_syntax& other) = delete;

    ~arithmetic_expression_syntax();

    static operator_kind parse_operator(std::string str);
};

class relational_expression_syntax final: public expression_syntax
{
    public:

    enum class operator_kind { Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual };

    const expression_syntax* const left;
    const syntax_token* const oper_token;
    const expression_syntax* const right;
    const operator_kind oper;

    relational_expression_syntax(expression_syntax* left, syntax_token* oper_token, expression_syntax* right);

    relational_expression_syntax(const relational_expression_syntax& other) = delete;

    relational_expression_syntax& operator=(const relational_expression_syntax& other) = delete;

    ~relational_expression_syntax();

    static operator_kind parse_operator(std::string str);
};

class conditional_expression_syntax final: public expression_syntax
{
    public:

    const expression_syntax* const true_value;
    const syntax_token* const if_token;
    const expression_syntax* const condition;
    const syntax_token* const else_token;
    const expression_syntax* const false_value;

    conditional_expression_syntax(expression_syntax* true_value, syntax_token* if_token, expression_syntax* condition, syntax_token* const else_token, expression_syntax* false_value);

    conditional_expression_syntax(const conditional_expression_syntax& other) = delete;

    conditional_expression_syntax& operator=(const conditional_expression_syntax& other) = delete;

    ~conditional_expression_syntax();

    private:

    static type_kind get_return_type(expression_syntax* left, expression_syntax* right);
};

class identifier_expression_syntax final: public expression_syntax
{
    public:

    const syntax_token* const identifier_token;
    const std::string identifier;

    identifier_expression_syntax(syntax_token* identifier_token);

    identifier_expression_syntax(const identifier_expression_syntax& other) = delete;

    identifier_expression_syntax& operator=(const identifier_expression_syntax& other) = delete;

    ~identifier_expression_syntax();

    private:

    static type_kind get_return_type(std::string identifier);
};

class invocation_expression_syntax final: public expression_syntax
{
    public:

    const syntax_token* const identifier_token;
    const std::string identifier;
    const list_syntax<expression_syntax>* const expression_list;

    invocation_expression_syntax(syntax_token* identifier_token);

    invocation_expression_syntax(syntax_token* identifier_token, list_syntax<expression_syntax>* expression_list);

    invocation_expression_syntax(const invocation_expression_syntax& other) = delete;

    invocation_expression_syntax& operator=(const invocation_expression_syntax& other) = delete;

    ~invocation_expression_syntax();

    private:

    static type_kind get_return_type(std::string identifier);
};

#endif