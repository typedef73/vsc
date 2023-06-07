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
    case  L'À':  b = 'À'; break;
    case  L'Á':  b = 'Á'; break;
    case  L'Â':  b = 'Â'; break;
    case  L'Ã':  b = 'Ã'; break;
    case  L'Ä':  b = 'Ä'; break;
    case  L'Å':  b = 'Å'; break;
    case  L'¨':  b = '¨'; break;
    case  L'Æ':  b = 'Æ'; break;
    case  L'Ç':  b = 'Ç'; break;
    case  L'È':  b = 'È'; break;
    case  L'É':  b = 'É'; break;
    case  L'Ê':  b = 'Ê'; break;
    case  L'Ë':  b = 'Ë'; break;
    case  L'Ì':  b = 'Ì'; break;
    case  L'Í':  b = 'Í'; break;
    case  L'Î':  b = 'Î'; break;
    case  L'Ï':  b = 'Ï'; break;
    case  L'Ð':  b = 'Ð'; break;
    case  L'Ñ':  b = 'Ñ'; break;
    case  L'Ò':  b = 'Ò'; break;
    case  L'Ó':  b = 'Ó'; break;
    case  L'Ô':  b = 'Ô'; break;
    case  L'Õ':  b = 'Õ'; break;
    case  L'Ö':  b = 'Ö'; break;
    case  L'×':  b = '×'; break;
    case  L'Ø':  b = 'Ø'; break;
    case  L'Ù':  b = 'Ù'; break;
    case  L'Ú':  b = 'Ú'; break;
    case  L'Û':  b = 'Û'; break;
    case  L'Ü':  b = 'Ü'; break;
    case  L'Ý':  b = 'Ý'; break;
    case  L'Þ':  b = 'Þ'; break;
    case  L'ß':  b = 'ß'; break;
    case  L'à':  b = 'à'; break;
    case  L'á':  b = 'á'; break;
    case  L'â':  b = 'â'; break;
    case  L'ã':  b = 'ã'; break;
    case  L'ä':  b = 'ä'; break;
    case  L'å':  b = 'å'; break;
    case  L'¸':  b = '¸'; break;
    case  L'æ':  b = 'æ'; break;
    case  L'ç':  b = 'ç'; break;
    case  L'è':  b = 'è'; break;
    case  L'é':  b = 'é'; break;
    case  L'ê':  b = 'ê'; break;
    case  L'ë':  b = 'ë'; break;
    case  L'ì':  b = 'ì'; break;
    case  L'í':  b = 'í'; break;
    case  L'î':  b = 'î'; break;
    case  L'ï':  b = 'ï'; break;
    case  L'ð':  b = 'ð'; break;
    case  L'ñ':  b = 'ñ'; break;
    case  L'ò':  b = 'ò'; break;
    case  L'ó':  b = 'ó'; break;
    case  L'ô':  b = 'ô'; break;
    case  L'õ':  b = 'õ'; break;
    case  L'ö':  b = 'ö'; break;
    case  L'÷':  b = '÷'; break;
    case  L'ø':  b = 'ø'; break;
    case  L'ù':  b = 'ù'; break;
    case  L'ú':  b = 'ú'; break;
    case  L'û':  b = 'û'; break;
    case  L'ü':  b = 'ü'; break;
    case  L'ý':  b = 'ý'; break;
    case  L'þ':  b = 'þ'; break;
    case  L'ÿ':  b = 'ÿ'; break;
        //---//
    case  L'²':  b = '²'; break;
    case  L'³':  b = '³'; break;
    case  L'¯':  b = '¯'; break;
    case  L'¿':  b = '¿'; break;
    case  L'¥':  b = '¥'; break;
    case  L'´':  b = '´'; break;
    case  L'¹':  b = '¹'; break;
    case  L'ª':  b = 'ª'; break;
    case  L'º':  b = 'º'; break;
    case  L'—':  b = '—'; break;
    }
    return b;
}
wchar_t Convert_2(char a) {
    wchar_t b = L'?';
    switch (a) {
    case 'À':  b = L'À'; break;
    case 'Á':  b = L'Á'; break;
    case 'Â':  b = L'Â'; break;
    case 'Ã':  b = L'Ã'; break;
    case 'Ä':  b = L'Ä'; break;
    case 'Å':  b = L'Å'; break;
    case '¨':  b = L'¨'; break;
    case 'Æ':  b = L'Æ'; break;
    case 'Ç':  b = L'Ç'; break;
    case 'È':  b = L'È'; break;
    case 'É':  b = L'É'; break;
    case 'Ê':  b = L'Ê'; break;
    case 'Ë':  b = L'Ë'; break;
    case 'Ì':  b = L'Ì'; break;
    case 'Í':  b = L'Í'; break;
    case 'Î':  b = L'Î'; break;
    case 'Ï':  b = L'Ï'; break;
    case 'Ð':  b = L'Ð'; break;
    case 'Ñ':  b = L'Ñ'; break;
    case 'Ò':  b = L'Ò'; break;
    case 'Ó':  b = L'Ó'; break;
    case 'Ô':  b = L'Ô'; break;
    case 'Õ':  b = L'Õ'; break;
    case 'Ö':  b = L'Ö'; break;
    case '×':  b = L'×'; break;
    case 'Ø':  b = L'Ø'; break;
    case 'Ù':  b = L'Ù'; break;
    case 'Ú':  b = L'Ú'; break;
    case 'Û':  b = L'Û'; break;
    case 'Ü':  b = L'Ü'; break;
    case 'Ý':  b = L'Ý'; break;
    case 'Þ':  b = L'Þ'; break;
    case 'ß':  b = L'ß'; break;
    case 'à':  b = L'à'; break;
    case 'á':  b = L'á'; break;
    case 'â':  b = L'â'; break;
    case 'ã':  b = L'ã'; break;
    case 'ä':  b = L'ä'; break;
    case 'å':  b = L'å'; break;
    case '¸':  b = L'¸'; break;
    case 'æ':  b = L'æ'; break;
    case 'ç':  b = L'ç'; break;
    case 'è':  b = L'è'; break;
    case 'é':  b = L'é'; break;
    case 'ê':  b = L'ê'; break;
    case 'ë':  b = L'ë'; break;
    case 'ì':  b = L'ì'; break;
    case 'í':  b = L'í'; break;
    case 'î':  b = L'î'; break;
    case 'ï':  b = L'ï'; break;
    case 'ð':  b = L'ð'; break;
    case 'ñ':  b = L'ñ'; break;
    case 'ò':  b = L'ò'; break;
    case 'ó':  b = L'ó'; break;
    case 'ô':  b = L'ô'; break;
    case 'õ':  b = L'õ'; break;
    case 'ö':  b = L'ö'; break;
    case '÷':  b = L'÷'; break;
    case 'ø':  b = L'ø'; break;
    case 'ù':  b = L'ù'; break;
    case 'ú':  b = L'ú'; break;
    case 'û':  b = L'û'; break;
    case 'ü':  b = L'ü'; break;
    case 'ý':  b = L'ý'; break;
    case 'þ':  b = L'þ'; break;
    case 'ÿ':  b = L'ÿ'; break;
        //---//
    case '²':  b = L'²'; break;
    case '³':  b = L'³'; break;
    case '¯':  b = L'¯'; break;
    case '¿':  b = L'¿'; break;
    case '¥':  b = L'¥'; break;
    case '´':  b = L'´'; break;
    case '¹':  b = L'¹'; break;
    case 'ª':  b = L'ª'; break;
    case 'º':  b = L'º'; break; 
    case '—':  b = L'—'; break;
    }
    return b;
}