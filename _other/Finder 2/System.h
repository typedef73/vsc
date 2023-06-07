#pragma once
#include <string>

typedef std::string string;
typedef std::wstring wstring;

string wstring_to_string(wstring hbuf);
wstring string_to_wstring(string s);
double string_to_double(string s);
string path(bool b = 0, std::string p = "");

string getType(string data);