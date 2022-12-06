
#ifndef _SYNTAX_TOKEN_HPP_
#define _SYNTAX_TOKEN_HPP_

#include <string>

class syntax_token
{
    public:

    const int type; 

    const int definition_line;
    
    const std::string text;

    syntax_token(int type, int definition_line, std::string text) : 
        type(type), definition_line(definition_line), text(text)
    {

    }

    virtual ~syntax_token() = default;
};

class numeric_syntax_token final : public syntax_token
{
    public:

    const int value;

    numeric_syntax_token(int type, int definition_line, std::string text, int value) : 
        syntax_token(type, definition_line, text), value(value)
    {
        
    }

    ~numeric_syntax_token() = default;
};

#endif