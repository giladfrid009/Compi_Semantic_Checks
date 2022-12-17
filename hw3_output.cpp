#include <iostream>
#include "hw3_output.hpp"
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

string value_lists_to_string(const std::vector<string>& values);
string type_list_to_string(const std::vector<string>& arg_types);

void output::end_scope()
{
    cout << "---end scope---" << endl;
}

void output::print_id(const string& id, int offset, const string& type)
{
    cout << id << " " << type << " " << offset << endl;
}

string type_list_to_string(const std::vector<string>& arg_types)
{
    stringstream res;
    res << "(";
    for (size_t i = 0; i < arg_types.size(); ++i)
    {
        res << arg_types[i];
        if (i + 1 < arg_types.size())
            res << ",";
    }
    res << ")";
    return res.str();
}

string value_lists_to_string(const std::vector<string>& values)
{
    stringstream res;
    res << "{";
    for (size_t i = 0; i < values.size(); ++i)
    {
        res << values[i];
        if (i + 1 < values.size())
            res << ",";
    }
    res << "}";
    return res.str();
}

string output::make_function_type(const string& ret_type, std::vector<string>& arg_types)
{
    stringstream res;
    res << type_list_to_string(arg_types) << "->" << ret_type;
    return res.str();
}

void output::error_lex(int lineno)
{
    cout << "line " << lineno << ":" << " lexical error" << endl;
    exit(0);
}

void output::error_syn(int lineno)
{
    cout << "line " << lineno << ":" << " syntax error" << endl;
    exit(0);
}

void output::error_undef(int lineno, const string& id)
{
    cout << "line " << lineno << ":" << " variable " << id << " is not defined" << endl;
    exit(0);
}

void output::error_def(int lineno, const string& id)
{
    cout << "line " << lineno << ":" << " identifier " << id << " is already defined" << endl;
    exit(0);
}

void output::error_undef_func(int lineno, const string& id)
{
    cout << "line " << lineno << ":" << " function " << id << " is not defined" << endl;
    exit(0);
}

void output::error_mismatch(int lineno)
{
    cout << "line " << lineno << ":" << " type mismatch" << endl;
    exit(0);
}

void output::error_prototype_mismatch(int lineno, const string& id, std::vector<string>& arg_types)
{
    cout << "line " << lineno << ": prototype mismatch, function " << id << " expects arguments " << type_list_to_string(arg_types) << endl;
    exit(0);
}

void output::error_unexpected_break(int lineno)
{
    cout << "line " << lineno << ":" << " unexpected break statement" << endl;
    exit(0);
}

void output::error_unexpected_continue(int lineno)
{
    cout << "line " << lineno << ":" << " unexpected continue statement" << endl;
    exit(0);
}

void output::error_main_missing()
{
    cout << "Program has no 'void main()' function" << endl;
    exit(0);
}

void output::error_byte_too_large(int lineno, const string& value)
{
    cout << "line " << lineno << ": byte value " << value << " out of range" << endl;
    exit(0);
}
