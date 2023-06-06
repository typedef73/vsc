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
char Symbol_Ttable(int n);
int Symbol_Ttable_2(char n);

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
void remove_dir(const wchar_t* folder) {
    std::wstring search_path = std::wstring(folder) + L"/*.*";
    std::wstring s_p = std::wstring(folder) + L"/";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(fd.cFileName, L".") != 0 && wcscmp(fd.cFileName, L"..") != 0)
                {
                    remove_dir((wchar_t*)(s_p + fd.cFileName).c_str());
                }
            }
            else {
                DeleteFile((s_p + fd.cFileName).c_str());
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
        _wrmdir(folder);
    }
}
bool StartProgramm(string str) {  
    HWND hWnd = GetConsoleWindow();
    wstring ws = string_to_wstring(str);

    return ShellExecute(hWnd, L"open", ws.c_str(), NULL, NULL, SW_SHOWNORMAL);
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
    }
    else {
        if (b) return str;
        else return str2;
    }
}

wstring getDate(wstring Name) {
    HANDLE hFile1;
    FILETIME ftCreate;
    SYSTEMTIME stUTC, stLocal;
    wstring date = L"";

    hFile1 = CreateFile(Name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile1 == INVALID_HANDLE_VALUE) {
        return L"error";
    }

    if (!GetFileTime(hFile1, &ftCreate, NULL, NULL)) {
        return L"error";
    }

    FileTimeToSystemTime(&ftCreate, &stUTC);

    if (std::to_wstring(stUTC.wDay).size() == 1) date += '0';
    date += std::to_wstring(stUTC.wDay) + L".";
    if (std::to_wstring(stUTC.wMonth).size() == 1) date += '0';
    date += std::to_wstring(stUTC.wMonth) + L".";

    date += std::to_wstring(stUTC.wYear) + L" ";
   
    CloseHandle(hFile1);
    return date;
}
wstring getSize(wstring Name) {
    struct stat fi;    
    stat(wstring_to_string(Name).c_str(), &fi);

    float size = fi.st_size;

    int n = 0;
    for (;;) {
        if (size > 1024) {
            size /= 1024; n++;
            if (n >= 3) break;
        }
        else break;
    }
    size = floor(size);
    wstring s = std::to_wstring(int(size)) + L" ";
    if (n == 0) s += L"b";
    if (n == 1) s += L"Kb";
    if (n == 2) s += L"Mb";
    if (n >= 3) s += L"Gb";

    return s;
}
std::string Find(std::wstring Name) {
    wstring name_help = Name;
    Name += L"\\*";    
    std::wstring wdata;
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    LPCWSTR nam = Name.c_str();
    hf = FindFirstFile(nam, &FindFileData);
    if (hf != INVALID_HANDLE_VALUE) {
        do {
            if (wcscmp(FindFileData.cFileName, L".") != 0 && wcscmp(FindFileData.cFileName, L"..") != 0) {
               wstring name = name_help + L"\\" + FindFileData.cFileName;

                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { wdata += '<'; wdata += FindFileData.cFileName; wdata += '\n'; }
                else {
                    if (getDate(name) != L"error") {
                        wdata += FindFileData.cFileName;
                        wdata += '|'; wdata += getDate(name);
                        wdata += '|'; wdata += getSize(name);
                        wdata += '\n';
                    }
                }
            }
        } while (FindNextFile(hf, &FindFileData) != 0);
        FindClose(hf);
    }
    else return "?\n";
    if (wdata.size() == 0) return ">\n";
    string data = wstring_to_string(wdata);
    return data;
}
void getFolderSize_helper(std::wstring Name, double& data) {
    wstring name_help = Name;
    Name += L"\\*";
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    LPCWSTR nam = Name.c_str();
    hf = FindFirstFile(nam, &FindFileData);
    if (hf != INVALID_HANDLE_VALUE) {
        do {
            if (wcscmp(FindFileData.cFileName, L".") != 0 && wcscmp(FindFileData.cFileName, L"..") != 0) {
                wstring name = name_help + L"\\" + FindFileData.cFileName;

                if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { getFolderSize_helper(name, data); }
                else {
                    struct stat fi;
                    stat(wstring_to_string(name).c_str(), &fi);
                    data += fi.st_size;
                }
            }
        } while (FindNextFile(hf, &FindFileData) != 0);
        FindClose(hf);
    }
    return;
}
string getFolderSize(std::wstring Name) {
    double size;
    getFolderSize_helper(Name, size);

    int n = 0;
    for (;;) {
        if (size > 1024) {
            size /= 1024; n++;
            if (n >= 3) break;
        }
        else break;
    }
    string s = std::to_string(int(floor(size)));
    if (n == 0) s += "b";
    if (n == 1) s += "Kb";
    if (n == 2) s += "Mb";
    if (n >= 3) s += "Gb";

    return s;
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

void getData(std::vector<string>& data, string path) {
    data.clear();
    string sdata = Find(string_to_wstring(path));
    string h = "";
    for (int i = 0; i < sdata.size(); i++) {
        if (sdata[i] == '\n') {
            data.push_back(h);
            h.clear();
            continue;
        }
        h += sdata[i];
    }

}

string Coding(string s) {
    using namespace::std;
    int N_coding = 64;

    string s2 = "";

    vector<int> help_int;
    vector<bool> help_minus;

    for (int i = 0; i < s.size(); i++) {
        int c = int(s[i]);
        if (c >= 0) {
            help_int.push_back(c);
            help_minus.push_back(0);
        }
        else {
            help_int.push_back(-c);
            help_minus.push_back(1);
        }
    }


    for (int i = 0; i < help_int.size(); i++) {
        int a = help_int[i] / N_coding;
        int b = help_int[i] - a * N_coding;

        if (help_minus[i]) s2 += '-';

        s2 += Symbol_Ttable(a);
        s2 += Symbol_Ttable(b);
    }

    return s2;
}
string DeCoding(string s) {
    using namespace::std;
    int N_coding = 64;
    vector<string> help_string;
    vector<bool> help_minus;

    int p = 2;
    if (s[0] == '-') p = 3;

    for (int i = 0; i < s.size(); i += p) {
        if (i != 0) {
            if (s[i - p] == '-') { help_minus.push_back(1); }
            else { help_minus.push_back(0); }
        }

        if (s[i] == '-')  p = 3;
        else  p = 2;

        help_string.push_back("");
        for (int j = i; j < i + p; j++) {
            if (s[j] != '-') help_string[help_string.size() - 1] += s[j];
        }
    }
    if (s[s.size() - p] == '-') { help_minus.push_back(1); }
    else { help_minus.push_back(0); }


    string s2 = "";

    for (int i = 0; i < help_string.size(); i++) {
        int a = Symbol_Ttable_2(help_string[i][0]);
        int b = Symbol_Ttable_2(help_string[i][1]);
        char c = (a * N_coding + b);

        if (help_minus[i])
            c *= -1;

        s2 += c;
    }

    return s2;
}
void coding(string& s) {
    string str = s;
    s.clear();
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == 0) { s += "~0"; continue; }	// /0
        if (str[i] == 10) { s += "~1"; continue; }  // /n
        if (str[i] == 13) { s += "~2"; continue; }  // "Возрат коретки"
        if (str[i] == 26) { s += "~3"; continue; }	// "Стелочка"
        if (str[i] == 37) { s += "~4"; continue; }	// %
        if (str[i] == 38) { s += "~5"; continue; }	// &
        if (str[i] == 43) { s += "~6"; continue; }	// + 
        if (str[i] == 126) { s += "~7"; continue; } // ~ (Вспомогательный)
        if (str[i] == 127) { s += "~8"; continue; }	// "Пятиугольник"

        s += str[i];
    }
}
void decoding(string& s) {
    string str = s;
    s.clear();
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != '~') { s += str[i]; }
        else {
            string h = "";
            h += str[i]; i++; h += str[i];
            char c = '?';
            if (h == "~0") { c = 0; }	// /0
            if (h == "~1") { c = 10; }  // /n
            if (h == "~2") { c = 13; }  // "Возрат коретки"
            if (h == "~3") { c = 26; }	// "Стелочка"
            if (h == "~4") { c = 37; }	// %
            if (h == "~5") { c = 38; }	// &
            if (h == "~6") { c = 43; }	// + 
            if (h == "~7") { c = 126; } // ~ (Вспомогательный)
            if (h == "~8") { c = 127; }	// "Пятиугольник"

            s += c;
        }
    }
}

int Symbol_Ttable_2(char n) {
    int c = -1;

    switch (n) {
    case  '0':  c = 0; break;  // Цифры 0 - 9
    case  '1':  c = 1; break;
    case  '2':  c = 2; break;
    case  '3':  c = 3; break;
    case  '4':  c = 4; break;
    case  '5':  c = 5; break;
    case  '6':  c = 6; break;
    case  '7':  c = 7; break;
    case  '8':  c = 8; break;
    case  '9':  c = 9; break;
    case  'a':  c = 10; break;  // Маленькие буквы 10 - 35
    case  'b':  c = 11; break;
    case  'c':  c = 12; break;
    case  'd':  c = 13; break;
    case  'e':  c = 14; break;
    case  'f':  c = 15; break;
    case  'g':  c = 16; break;
    case  'h':  c = 17; break;
    case  'i':  c = 18; break;
    case  'j':  c = 19; break;
    case  'k':  c = 20; break;
    case  'l':  c = 21; break;
    case  'm':  c = 22; break;
    case  'n':  c = 23; break;
    case  'o':  c = 24; break;
    case  'p':  c = 25; break;
    case  'q':  c = 26; break;
    case  'r':  c = 27; break;
    case  's':  c = 28; break;
    case  't':  c = 29; break;
    case  'u':  c = 30; break;
    case  'v':  c = 31; break;
    case  'w':  c = 32; break;
    case  'x':  c = 33; break;
    case  'y':  c = 34; break;
    case  'z':  c = 35; break;
    case  'A':  c = 36; break;  // Большие буквы 36 - 61 
    case  'B':  c = 37; break;
    case  'C':  c = 38; break;
    case  'D':  c = 39; break;
    case  'E':  c = 40; break;
    case  'F':  c = 41; break;
    case  'G':  c = 42; break;
    case  'H':  c = 43; break;
    case  'I':  c = 44; break;
    case  'J':  c = 45; break;
    case  'K':  c = 46; break;
    case  'L':  c = 47; break;
    case  'M':  c = 48; break;
    case  'N':  c = 49; break;
    case  'O':  c = 50; break;
    case  'P':  c = 51; break;
    case  'Q':  c = 52; break;
    case  'R':  c = 53; break;
    case  'S':  c = 54; break;
    case  'T':  c = 55; break;
    case  'U':  c = 56; break;
    case  'V':  c = 57; break;
    case  'W':  c = 58; break;
    case  'X':  c = 59; break;
    case  'Y':  c = 60; break;
    case  'Z':  c = 61; break;
    case  '_':  c = 62; break;  // Доп. символы 62 - 63
    case  '~':  c = 63; break;
    }

    return c;
}
char Symbol_Ttable(int n) {
    char c = '?';

    switch (n) {
    case  0:  c = '0'; break;  // Цифры 0 - 9
    case  1:  c = '1'; break;
    case  2:  c = '2'; break;
    case  3:  c = '3'; break;
    case  4:  c = '4'; break;
    case  5:  c = '5'; break;
    case  6:  c = '6'; break;
    case  7:  c = '7'; break;
    case  8:  c = '8'; break;
    case  9:  c = '9'; break;
    case  10:  c = 'a'; break;  // Маленькие буквы 10 - 35
    case  11:  c = 'b'; break;
    case  12:  c = 'c'; break;
    case  13:  c = 'd'; break;
    case  14:  c = 'e'; break;
    case  15:  c = 'f'; break;
    case  16:  c = 'g'; break;
    case  17:  c = 'h'; break;
    case  18:  c = 'i'; break;
    case  19:  c = 'j'; break;
    case  20:  c = 'k'; break;
    case  21:  c = 'l'; break;
    case  22:  c = 'm'; break;
    case  23:  c = 'n'; break;
    case  24:  c = 'o'; break;
    case  25:  c = 'p'; break;
    case  26:  c = 'q'; break;
    case  27:  c = 'r'; break;
    case  28:  c = 's'; break;
    case  29:  c = 't'; break;
    case  30:  c = 'u'; break;
    case  31:  c = 'v'; break;
    case  32:  c = 'w'; break;
    case  33:  c = 'x'; break;
    case  34:  c = 'y'; break;
    case  35:  c = 'z'; break;
    case  36:  c = 'A'; break;  // Большие буквы 36 - 61 
    case  37:  c = 'B'; break;
    case  38:  c = 'C'; break;
    case  39:  c = 'D'; break;
    case  40:  c = 'E'; break;
    case  41:  c = 'F'; break;
    case  42:  c = 'G'; break;
    case  43:  c = 'H'; break;
    case  44:  c = 'I'; break;
    case  45:  c = 'J'; break;
    case  46:  c = 'K'; break;
    case  47:  c = 'L'; break;
    case  48:  c = 'M'; break;
    case  49:  c = 'N'; break;
    case  50:  c = 'O'; break;
    case  51:  c = 'P'; break;
    case  52:  c = 'Q'; break;
    case  53:  c = 'R'; break;
    case  54:  c = 'S'; break;
    case  55:  c = 'T'; break;
    case  56:  c = 'U'; break;
    case  57:  c = 'V'; break;
    case  58:  c = 'W'; break;
    case  59:  c = 'X'; break;
    case  60:  c = 'Y'; break;
    case  61:  c = 'Z'; break;
    case  62:  c = '_'; break;  // Доп. символы 62 - 63
    case  63:  c = '~'; break;
    }

    return c;
}
char Convert(wchar_t a) {
    char b = '?';
    switch (a) {
    case  L'А':  b = 'А'; break;
    case  L'Б':  b = 'Б'; break;
    case  L'В':  b = 'В'; break;
    case  L'Г':  b = 'Г'; break;
    case  L'Д':  b = 'Д'; break;
    case  L'Е':  b = 'Е'; break;
    case  L'Ё':  b = 'Ё'; break;
    case  L'Ж':  b = 'Ж'; break;
    case  L'З':  b = 'З'; break;
    case  L'И':  b = 'И'; break;
    case  L'Й':  b = 'Й'; break;
    case  L'К':  b = 'К'; break;
    case  L'Л':  b = 'Л'; break;
    case  L'М':  b = 'М'; break;
    case  L'Н':  b = 'Н'; break;
    case  L'О':  b = 'О'; break;
    case  L'П':  b = 'П'; break;
    case  L'Р':  b = 'Р'; break;
    case  L'С':  b = 'С'; break;
    case  L'Т':  b = 'Т'; break;
    case  L'У':  b = 'У'; break;
    case  L'Ф':  b = 'Ф'; break;
    case  L'Х':  b = 'Х'; break;
    case  L'Ц':  b = 'Ц'; break;
    case  L'Ч':  b = 'Ч'; break;
    case  L'Ш':  b = 'Ш'; break;
    case  L'Щ':  b = 'Щ'; break;
    case  L'Ъ':  b = 'Ъ'; break;
    case  L'Ы':  b = 'Ы'; break;
    case  L'Ь':  b = 'Ь'; break;
    case  L'Э':  b = 'Э'; break;
    case  L'Ю':  b = 'Ю'; break;
    case  L'Я':  b = 'Я'; break;
    case  L'а':  b = 'а'; break;
    case  L'б':  b = 'б'; break;
    case  L'в':  b = 'в'; break;
    case  L'г':  b = 'г'; break;
    case  L'д':  b = 'д'; break;
    case  L'е':  b = 'е'; break;
    case  L'ё':  b = 'ё'; break;
    case  L'ж':  b = 'ж'; break;
    case  L'з':  b = 'з'; break;
    case  L'и':  b = 'и'; break;
    case  L'й':  b = 'й'; break;
    case  L'к':  b = 'к'; break;
    case  L'л':  b = 'л'; break;
    case  L'м':  b = 'м'; break;
    case  L'н':  b = 'н'; break;
    case  L'о':  b = 'о'; break;
    case  L'п':  b = 'п'; break;
    case  L'р':  b = 'р'; break;
    case  L'с':  b = 'с'; break;
    case  L'т':  b = 'т'; break;
    case  L'у':  b = 'у'; break;
    case  L'ф':  b = 'ф'; break;
    case  L'х':  b = 'х'; break;
    case  L'ц':  b = 'ц'; break;
    case  L'ч':  b = 'ч'; break;
    case  L'ш':  b = 'ш'; break;
    case  L'щ':  b = 'щ'; break;
    case  L'ъ':  b = 'ъ'; break;
    case  L'ы':  b = 'ы'; break;
    case  L'ь':  b = 'ь'; break;
    case  L'э':  b = 'э'; break;
    case  L'ю':  b = 'ю'; break;
    case  L'я':  b = 'я'; break;
        //---//
    case  L'І':  b = 'І'; break;
    case  L'і':  b = 'і'; break;
    case  L'Ї':  b = 'Ї'; break;
    case  L'ї':  b = 'ї'; break;
    case  L'Ґ':  b = 'Ґ'; break;
    case  L'ґ':  b = 'ґ'; break;
    case  L'№':  b = '№'; break;
    case  L'Є':  b = 'Є'; break;
    case  L'є':  b = 'є'; break;
    case  L'—':  b = '—'; break;
    }
    return b;
}
wchar_t Convert_2(char a) {
    wchar_t b = L'?';
    switch (a) {
    case 'А':  b = L'А'; break;
    case 'Б':  b = L'Б'; break;
    case 'В':  b = L'В'; break;
    case 'Г':  b = L'Г'; break;
    case 'Д':  b = L'Д'; break;
    case 'Е':  b = L'Е'; break;
    case 'Ё':  b = L'Ё'; break;
    case 'Ж':  b = L'Ж'; break;
    case 'З':  b = L'З'; break;
    case 'И':  b = L'И'; break;
    case 'Й':  b = L'Й'; break;
    case 'К':  b = L'К'; break;
    case 'Л':  b = L'Л'; break;
    case 'М':  b = L'М'; break;
    case 'Н':  b = L'Н'; break;
    case 'О':  b = L'О'; break;
    case 'П':  b = L'П'; break;
    case 'Р':  b = L'Р'; break;
    case 'С':  b = L'С'; break;
    case 'Т':  b = L'Т'; break;
    case 'У':  b = L'У'; break;
    case 'Ф':  b = L'Ф'; break;
    case 'Х':  b = L'Х'; break;
    case 'Ц':  b = L'Ц'; break;
    case 'Ч':  b = L'Ч'; break;
    case 'Ш':  b = L'Ш'; break;
    case 'Щ':  b = L'Щ'; break;
    case 'Ъ':  b = L'Ъ'; break;
    case 'Ы':  b = L'Ы'; break;
    case 'Ь':  b = L'Ь'; break;
    case 'Э':  b = L'Э'; break;
    case 'Ю':  b = L'Ю'; break;
    case 'Я':  b = L'Я'; break;
    case 'а':  b = L'а'; break;
    case 'б':  b = L'б'; break;
    case 'в':  b = L'в'; break;
    case 'г':  b = L'г'; break;
    case 'д':  b = L'д'; break;
    case 'е':  b = L'е'; break;
    case 'ё':  b = L'ё'; break;
    case 'ж':  b = L'ж'; break;
    case 'з':  b = L'з'; break;
    case 'и':  b = L'и'; break;
    case 'й':  b = L'й'; break;
    case 'к':  b = L'к'; break;
    case 'л':  b = L'л'; break;
    case 'м':  b = L'м'; break;
    case 'н':  b = L'н'; break;
    case 'о':  b = L'о'; break;
    case 'п':  b = L'п'; break;
    case 'р':  b = L'р'; break;
    case 'с':  b = L'с'; break;
    case 'т':  b = L'т'; break;
    case 'у':  b = L'у'; break;
    case 'ф':  b = L'ф'; break;
    case 'х':  b = L'х'; break;
    case 'ц':  b = L'ц'; break;
    case 'ч':  b = L'ч'; break;
    case 'ш':  b = L'ш'; break;
    case 'щ':  b = L'щ'; break;
    case 'ъ':  b = L'ъ'; break;
    case 'ы':  b = L'ы'; break;
    case 'ь':  b = L'ь'; break;
    case 'э':  b = L'э'; break;
    case 'ю':  b = L'ю'; break;
    case 'я':  b = L'я'; break;
        //---//
    case 'І':  b = L'І'; break;
    case 'і':  b = L'і'; break;
    case 'Ї':  b = L'Ї'; break;
    case 'ї':  b = L'ї'; break;
    case 'Ґ':  b = L'Ґ'; break;
    case 'ґ':  b = L'ґ'; break;
    case '№':  b = L'№'; break;
    case 'Є':  b = L'Є'; break;
    case 'є':  b = L'є'; break; 
    case '—':  b = L'—'; break;
    }
    return b;
}