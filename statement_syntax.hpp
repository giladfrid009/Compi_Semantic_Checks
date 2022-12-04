#ifndef _STATEMENT_SYNTAX_HPP_
#define _STATEMENT_SYNTAX_HPP_

#include "abstract_syntax.hpp"
#include "expression_syntax.hpp"
#include "generic_syntax.hpp"
#include <vector>
#include <string>

class if_statement_syntax final : public statement_syntax
{
	public:

	expression_syntax* const condition;
	statement_syntax* const body;
	statement_syntax* const else_clause;

	if_statement_syntax(expression_syntax* condition, statement_syntax* body);

	if_statement_syntax(expression_syntax* condition, statement_syntax* body, statement_syntax* else_clause);

	std::vector<syntax_base*> get_children() const override;

	~if_statement_syntax();
};

class while_statement_syntax final : public statement_syntax
{
	public:

	expression_syntax* const condition;
	statement_syntax* const body;

	while_statement_syntax(expression_syntax* condition, statement_syntax* body);

	std::vector<syntax_base*> get_children() const override;

	~while_statement_syntax();
};

class branch_statement_syntax final : public statement_syntax
{
	public:

	const branch_type type;

	branch_statement_syntax(branch_type type);

	std::vector<syntax_base*> get_children() const override;

	~branch_statement_syntax();
};

class return_statement_syntax final : public statement_syntax
{
	public:

	expression_syntax* const expression;

	return_statement_syntax();

	return_statement_syntax(expression_syntax* expression);

	std::vector<syntax_base*> get_children() const override;

	~return_statement_syntax();
};

class expression_statement_syntax final : public statement_syntax
{
	public:

	expression_syntax* const expression;

	expression_statement_syntax(expression_syntax* expression);

	std::vector<syntax_base*> get_children() const override;

	~expression_statement_syntax();
};

class assignment_statement_syntax final : public statement_syntax
{
	public:

	const std::string identifier;
	expression_syntax* const value;

	assignment_statement_syntax(std::string identifier, expression_syntax* value);

	std::vector<syntax_base*> get_children() const override;

	~assignment_statement_syntax();
};

class declaration_statement_syntax final : public statement_syntax
{
	public:

	type_syntax* const type;
	const std::string identifier;
	expression_syntax* const value;

	declaration_statement_syntax(type_syntax* type, std::string identifier);

	declaration_statement_syntax(type_syntax* type, std::string identifier, expression_syntax* value);

	std::vector<syntax_base*> get_children() const override;

	~declaration_statement_syntax();
};

class block_statement_syntax final : public statement_syntax
{
	public:

	list_syntax<statement_syntax>* const statement_list;

	block_statement_syntax(list_syntax<statement_syntax>* statement_list);

	std::vector<syntax_base*> get_children() const override;

	~block_statement_syntax();
};

#endif