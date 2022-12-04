#ifndef _EXPRESSION_SYNTAX_HPP_
#define _EXPRESSION_SYNTAX_HPP_

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

	literal_expression_syntax(literal_type value) : value(value), expression_syntax(get_return_type(value))
	{
	}

	std::vector<syntax_base*> get_children() const override
	{
		return std::vector<syntax_base*>();
	}

	fundamental_type get_return_type(literal_type value) const
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

	std::vector<syntax_base*> get_children() const override;

	~cast_expression_syntax();
};

class not_expression_syntax final : public expression_syntax
{
	public:

	expression_syntax* const expression;

	not_expression_syntax(expression_syntax* expression);

	std::vector<syntax_base*> get_children() const override;

	~not_expression_syntax();
};

class logical_expression_syntax final : public expression_syntax
{
	public:

	expression_syntax* const left;
	expression_syntax* const right;
	const logical_operator oper;

	logical_expression_syntax(expression_syntax* left, expression_syntax* right, logical_operator oper);

	logical_expression_syntax(expression_syntax* left, expression_syntax* right, std::string oper_token);

	logical_operator get_operator_from_token(std::string token) const;

	std::vector<syntax_base*> get_children() const override;

	~logical_expression_syntax();
};

class arithmetic_expression_syntax final : public expression_syntax
{
	public:

	expression_syntax* const left;
	expression_syntax* const right;
	const arithmetic_operator oper;

	arithmetic_expression_syntax(expression_syntax* left, expression_syntax* right, arithmetic_operator oper);

	arithmetic_expression_syntax(expression_syntax* left, expression_syntax* right, std::string oper_token);

	fundamental_type get_return_type(expression_syntax* left, expression_syntax* right);

	arithmetic_operator get_operator_from_token(std::string token) const;

	std::vector<syntax_base*> get_children() const override;

	~arithmetic_expression_syntax();
};

class relational_expression_syntax final : public expression_syntax
{
	public:

	expression_syntax* const left;
	expression_syntax* const right;
	const relational_operator oper;

	relational_expression_syntax(expression_syntax* left, expression_syntax* right, relational_operator oper);

	relational_expression_syntax(expression_syntax* left, expression_syntax* right, std::string oper_token);

	relational_operator get_operator_from_token(std::string token) const;

	std::vector<syntax_base*> get_children() const override;

	~relational_expression_syntax();
};

class conditional_expression_syntax final : public expression_syntax
{
	public:

	expression_syntax* const true_value;
	expression_syntax* const condition;
	expression_syntax* const false_value;

	conditional_expression_syntax(expression_syntax* true_value, expression_syntax* condition, expression_syntax* false_value);

	std::vector<syntax_base*> get_children() const override;

	fundamental_type get_return_type(expression_syntax* left, expression_syntax* right) const;

	~conditional_expression_syntax();
};

class identifier_expression_syntax final : public expression_syntax
{
	public:

	const std::string identifier;

	identifier_expression_syntax(std::string identifier);

	std::vector<syntax_base*> get_children() const override;

	fundamental_type get_return_type(std::string identifier) const;

	~identifier_expression_syntax();
};

class invocation_expression_syntax final : public expression_syntax
{
	public:

	const std::string identifier;
	list_syntax<expression_syntax>* const expression_list;

	invocation_expression_syntax(std::string identifier);

	invocation_expression_syntax(std::string identifier, list_syntax<expression_syntax>* expression_list);

	std::vector<syntax_base*> get_children() const override;

	fundamental_type get_return_type(std::string identifier) const;

	~invocation_expression_syntax();
};

#endif