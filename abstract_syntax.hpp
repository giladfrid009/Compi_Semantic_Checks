#ifndef _ABSTRACT_SYNTAX_HPP_
#define _ABSTRACT_SYNTAX_HPP_

#include <vector>

enum class logical_operator { And, Or };

enum class arithmetic_operator { Add, Sub, Mul, Div };

enum class relational_operator { Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual };

enum class fundamental_type { Void, Int, Bool, Byte, String };

enum class branch_type { Break, Continue };

class syntax_base
{
	private:

	syntax_base* parent = nullptr;

	public:

	syntax_base();

	syntax_base* get_parent() const;

	void set_parent(syntax_base* new_parent);

	virtual ~syntax_base() = default;

	virtual std::vector<syntax_base*> get_children() const = 0;
};

class expression_syntax : public syntax_base
{
	public:

	const fundamental_type expression_return_type;

	expression_syntax(fundamental_type return_type);

	bool is_numeric() const;

	bool is_special() const;

	virtual ~expression_syntax() = default;

	virtual std::vector<syntax_base*> get_children() const = 0;
};

class statement_syntax : public syntax_base
{
	public:

	virtual ~statement_syntax() = default;

	virtual std::vector<syntax_base*> get_children() const = 0;
};

#endif