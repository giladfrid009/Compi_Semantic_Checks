#ifndef _syntax_base_
#define _syntax_base_

#include <vector>

enum class fundamental_type { Void, Int, Bool, Byte, String };

class syntax_base
{
    private:

    syntax_base* parent_syntax = nullptr;

    public:

    virtual std::vector<syntax_base*> children() = 0;

    syntax_base* parent();

    void register_parent(syntax_base* new_parent);

    virtual ~syntax_base() = default;
};

#endif