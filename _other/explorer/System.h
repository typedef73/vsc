#pragma once
#include <string>

typedef std::string string;
typedef std::wstring wstring;

string wstring_to_string(wstring hbuf);
wstring string_to_wstring(string s);
void remove_dir(const wchar_t* folder);
bool StartProgramm(string str);
double string_to_double(string s);
string path(bool b = 0, std::string p = "");
std::string Find(std::wstring Name);

string getType(string data);
void getData(std::vector<string>& data, string path);
string getFolderSize(std::wstring Name);

void coding(string& s);
void decoding(string& s);

string Coding(string s);
string DeCoding(string s);