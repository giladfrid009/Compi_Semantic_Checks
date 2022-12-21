#ifndef _236360_3_
#define _236360_3_

#include <vector>
#include <string>

namespace output
{
    void end_scope();

    void print_id(const std::string& id, int offset, const std::string& type);

    /* Do not save the string returned from this function in a data structure
        as it is not dynamically allocated and will be destroyed(!) at the end of the calling scope.
    */
    std::string make_function_type(const std::string& ret_type, std::vector<std::string>& arg_types);

    [[noreturn]] void error_lex(int lineno);

    [[noreturn]] void error_syn(int lineno);

    [[noreturn]] void error_undef(int lineno, const std::string& id);

    [[noreturn]] void error_def(int lineno, const std::string& id);

    [[noreturn]] void error_undef_func(int lineno, const std::string& id);

    [[noreturn]] void error_mismatch(int lineno);

    [[noreturn]] void error_prototype_mismatch(int lineno, const std::string& id, std::vector<std::string>& arg_types);

    [[noreturn]] void error_unexpected_break(int lineno);

    [[noreturn]] void error_unexpected_continue(int lineno);

    [[noreturn]] void error_main_missing();

    [[noreturn]] void error_byte_too_large(int lineno, const std::string& value);
}

#endif