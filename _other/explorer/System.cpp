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
        if (str[i] == 13) { s += "~2"; continue; }  // "������ �������"
        if (str[i] == 26) { s += "~3"; continue; }	// "��������"
        if (str[i] == 37) { s += "~4"; continue; }	// %
        if (str[i] == 38) { s += "~5"; continue; }	// &
        if (str[i] == 43) { s += "~6"; continue; }	// + 
        if (str[i] == 126) { s += "~7"; continue; } // ~ (���������������)
        if (str[i] == 127) { s += "~8"; continue; }	// "������������"

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
            if (h == "~2") { c = 13; }  // "������ �������"
            if (h == "~3") { c = 26; }	// "��������"
            if (h == "~4") { c = 37; }	// %
            if (h == "~5") { c = 38; }	// &
            if (h == "~6") { c = 43; }	// + 
            if (h == "~7") { c = 126; } // ~ (���������������)
            if (h == "~8") { c = 127; }	// "������������"

            s += c;
        }
    }
}

int Symbol_Ttable_2(char n) {
    int c = -1;

    switch (n) {
    case  '0':  c = 0; break;  // ����� 0 - 9
    case  '1':  c = 1; break;
    case  '2':  c = 2; break;
    case  '3':  c = 3; break;
    case  '4':  c = 4; break;
    case  '5':  c = 5; break;
    case  '6':  c = 6; break;
    case  '7':  c = 7; break;
    case  '8':  c = 8; break;
    case  '9':  c = 9; break;
    case  'a':  c = 10; break;  // ��������� ����� 10 - 35
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
    case  'A':  c = 36; break;  // ������� ����� 36 - 61 
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
    case  '_':  c = 62; break;  // ���. ������� 62 - 63
    case  '~':  c = 63; break;
    }

    return c;
}
char Symbol_Ttable(int n) {
    char c = '?';

    switch (n) {
    case  0:  c = '0'; break;  // ����� 0 - 9
    case  1:  c = '1'; break;
    case  2:  c = '2'; break;
    case  3:  c = '3'; break;
    case  4:  c = '4'; break;
    case  5:  c = '5'; break;
    case  6:  c = '6'; break;
    case  7:  c = '7'; break;
    case  8:  c = '8'; break;
    case  9:  c = '9'; break;
    case  10:  c = 'a'; break;  // ��������� ����� 10 - 35
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
    case  36:  c = 'A'; break;  // ������� ����� 36 - 61 
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
    case  62:  c = '_'; break;  // ���. ������� 62 - 63
    case  63:  c = '~'; break;
    }

    return c;
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