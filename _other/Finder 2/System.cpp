#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#pragma warning(disable : 4996)
#pragma comment(lib,"netapi32.lib")

typedef std::string string;
typedef std::wstring wstring;
typedef std::ifstream ifstream;

char Convert(wchar_t a);
wchar_t Convert_2(char a);

string wstring_to_string(wstring hbuf) {
    for (int i = 0; i < hbuf.size(); i++) {
        if (Convert(hbuf[i]) != '?') hbuf[i] = Convert(hbuf[i]);
    }
    string s(hbuf.begin(), hbuf.end());
    return s;
}
wstring string_to_wstring(string s) {
    wstring ws;
    for (int i = 0; i < s.size(); i++) {
        if (Convert_2(s[i]) != '?') ws += Convert_2(s[i]);
        else ws += s[i];
    }
    return ws;
}
double string_to_double(string s) {
    double c = 0;
    int a = 0, b = 0;
    bool mode = 0;
    int it = 0;
    for (char i : s) {
        if (i == '.') mode = 1;
        int h = i - 48;
        if (h < 0 || h > 9) { continue; }
        else {
            if (!mode) a = a * 10 + h;
            else { b = b * 10 + h; it++; }
        }
    }
    c = double(b) / pow(10, it);
    c += a;
    if (s[0] == '-') c *= -1;

    return c;
}

string path(bool b = 0, std::string p = "") {
    static std::string str;
    static std::string str2;

    if (p != "") {
        str = p;

        for (; p.back() != '\\';) p.pop_back();
        str2 = p;
        return "";
    }
    else {
        if (b) return str;
        else return str2;
    }
}

string getType(string data) {
    string f = "";
    for (int j = data.size() - 1; j >= 0; j--) {
        if (data[j] == '.') { break; }
        f += data[j];
    }
    string type = "";
    for (int j = f.size() - 1; j >= 0; j--) {
        type += f[j];
    }
    return type;
}

char Convert(wchar_t a) {
    char b = '?';
    switch (a) {
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
        //---//
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    case  L'�':  b = '�'; break;
    }
    return b;
}
wchar_t Convert_2(char a) {
    wchar_t b = L'?';
    switch (a) {
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
        //---//
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break;
    case '�':  b = L'�'; break; 
    case '�':  b = L'�'; break;
    }
    return b;
}