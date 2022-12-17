#ifndef _236360_3_
#define _236360_3_

#include <vector>
#include <string>
using namespace std;

namespace output
{
    void end_scope();

    void print_id(const string& id, int offset, const string& type);

    /* Do not save the string returned from this function in a data structure
        as it is not dynamically allocated and will be destroyed(!) at the end of the calling scope.
    */
    string make_function_type(const string& ret_type, vector<string>& arg_types);

    [[noreturn]] void error_lex(int lineno);

    [[noreturn]] void error_syn(int lineno);

    [[noreturn]] void error_undef(int lineno, const string& id);

    [[noreturn]] void error_def(int lineno, const string& id);

    [[noreturn]] void error_undef_func(int lineno, const string& id);

    [[noreturn]] void error_mismatch(int lineno);

    [[noreturn]] void error_prototype_mismatch(int lineno, const string& id, vector<string>& arg_types);

    [[noreturn]] void error_unexpected_break(int lineno);

    [[noreturn]] void error_unexpected_continue(int lineno);

    [[noreturn]] void error_main_missing();

    [[noreturn]] void error_byte_too_large(int lineno, const string& value);
}

#endif