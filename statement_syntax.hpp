#ifndef _statement_syntax_
#define _statement_syntax_

#include "syntax_base.hpp"
#include <vector>

enum class branch_type { Break, Continue };

class statement_syntax : public syntax_base
{
    public:

    virtual ~statement_syntax() = default;

    virtual std::vector<syntax_base*> children() = 0;
};

#endif