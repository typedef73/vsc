#include "Find.h"
#include "System.h"
#include <Windows.h>
#include <winternl.h>
#include <fstream>
using std::ofstream;
#pragma comment(lib,"ntdll.lib")

int to_int(int a) {
    int chislo = -1;
    switch (a) {
    case 48: chislo = 0; break;
    case 49: chislo = 1; break;
    case 50: chislo = 2; break;
    case 51: chislo = 3; break;
    case 52: chislo = 4; break;
    case 53: chislo = 5; break;
    case 54: chislo = 6; break;
    case 55: chislo = 7; break;
    case 56: chislo = 8; break;
    case 57: chislo = 9; break;
    case 13: chislo = -2; break;
    case 8: chislo = -3; break;
    case 59: chislo = -4; break;
    case 10: chislo = -5; break;
    case 45: chislo = -6; break;
    case 46: chislo = -7; break;
    }
    return chislo;
}

bool Find::if_type(string name) {
    if (types.size() > 0) {
        int n = 0;
        for (int i = name.size() - 1; i >= 0; i--) {
            if (name[i] == '\\') return 0;
            if (name[i] == '.') { n = i + 1; break; }
        }
        if (n == 0) return 0;
        string h = "";
        for (int i = n; i < name.size(); i++) {
            h += name[i];
        }
        for (int i = 0; i < types.size(); i++) {
            if (h == types[i]) return 1;
        }
        return 0;
    }
    else return 1;
}

typedef struct _SYSTEM_PROCESS_INFO
{
    ULONG                   NextEntryOffset;
    ULONG                   NumberOfThreads;
    LARGE_INTEGER           Reserved[3];
    LARGE_INTEGER           CreateTime;
    LARGE_INTEGER           UserTime;
    LARGE_INTEGER           KernelTime;
    UNICODE_STRING          ImageName;
    ULONG                   BasePriority;
    HANDLE                  ProcessId;
    HANDLE                  InheritedFromProcessId;
}SYSTEM_PROCESS_INFO, * PSYSTEM_PROCESS_INFO;

int findprocess(wstring name) {
    NTSTATUS status;
    PVOID buffer;
    PSYSTEM_PROCESS_INFO spi;

    buffer = VirtualAlloc(NULL, 1024 * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    spi = (PSYSTEM_PROCESS_INFO)buffer;

    if (!NT_SUCCESS(status = NtQuerySystemInformation(SystemProcessInformation, spi, 1024 * 1024, NULL))) {
        VirtualFree(buffer, 0, MEM_RELEASE);
        return -1;
    }

    while (spi->NextEntryOffset) {
        //spi->ImageName.Buffer, spi->ProcessId 
        spi = (PSYSTEM_PROCESS_INFO)((LPBYTE)spi + spi->NextEntryOffset); // Calculate the address of the next entry.
        if (spi->ImageName.Buffer == name) {
            VirtualFree(buffer, 0, MEM_RELEASE);
            return 1;
        }
    }
    VirtualFree(buffer, 0, MEM_RELEASE);
    return 0;
}

bool file_or_no(std::wstring Name) {
    wstring name_help = Name;
    Name += L"\\*";
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    LPCWSTR nam = Name.c_str();
    hf = FindFirstFile(nam, &FindFileData);
    if (hf != INVALID_HANDLE_VALUE) {
        do {
            return 0;

        } while (FindNextFile(hf, &FindFileData) != 0);
        FindClose(hf);
    }
    else return -1;
}
/////////////////////////////////////////////////////////////

Find::Find(bool b_autodel, bool b_winwow_folder, HWND &hwnd, wstring w_path, wstring w_date_start, wstring w_date_end, wstring w_file_type) {
    autodel = b_autodel;
    window_folder = b_winwow_folder;
    this->hwnd = hwnd;

    text_scan = CreateWindow(L"static", L"0", WS_VISIBLE | WS_CHILD, 10, 30, 570, 150, hwnd, NULL, NULL, NULL);
    text_time = CreateWindow(L"static", L"0", WS_VISIBLE | WS_CHILD, 10, 200, 570, 150, hwnd, NULL, NULL, NULL);

    string getdata1 = wstring_to_string(w_date_start);
    string getdata2 = wstring_to_string(w_date_end);

    int j = 0;
    for (int i = 0; i < getdata1.size(); i++) {
        if (getdata1[i] == '/') { j++; continue; }
        if (j == 0) {
            day_1 = day_1 * 10 + to_int(getdata1[i]);
        }
        if (j == 1) {
            month_1 = month_1 * 10 + to_int(getdata1[i]);
        }
        if (j == 2) {
            year_1 = year_1 * 10 + to_int(getdata1[i]);
        }
    }
    j = 0;
    for (int i = 0; i < getdata2.size(); i++) {
        if (getdata2[i] == '/') { j++; continue; }
        if (j == 0) {
            day_2 = day_2 * 10 + to_int(getdata2[i]);
        }
        if (j == 1) {
            month_2 = month_2 * 10 + to_int(getdata2[i]);
        }
        if (j == 2) {
            year_2 = year_2 * 10 + to_int(getdata2[i]);
        }
    }
    
    string type = wstring_to_string(w_file_type);
    string h = "";
    for (int i = 0; i < type.size(); i++) {
        if (type[i] == ' ') continue;
        if (type[i] == ',') {
            types.push_back(h);
            h.clear();
            continue;
        }
        h += type[i];
    }
    if (h != "") types.push_back(h);
    if (w_path[w_path.size() - 1] == '\\') w_path.pop_back();

    //std::wcout << w_path << std::endl;
    //std::cout << day_1 << '/' << month_1 << '/' << year_1 << std::endl;
    //std::cout << day_2 << '/' << month_2 << '/' << year_2 << std::endl;
    //std::cout << type << " - " << types.size() << std::endl;   
    t = clock();
    find(w_path);
    Save();
}

int Find::Time(wstring Name) {
    HANDLE hFile1;
    FILETIME ftCreate;
    SYSTEMTIME stUTC, stLocal;

    hFile1 = CreateFile(Name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile1 == INVALID_HANDLE_VALUE) {
        return -1;
    }

    if (!GetFileTime(hFile1, &ftCreate, NULL, NULL)) {
        err++;
        return -2;
    }

    FileTimeToSystemTime(&ftCreate, &stUTC);
    int data_1 = year_1 * 366 + month_1 * 31 + day_1;
    int data_2 = year_2 * 366 + month_2 * 31 + day_2;   
    int data_now = stUTC.wYear * 366 + stUTC.wMonth * 31 + stUTC.wDay;

    if (data_1 <= data_now && data_2 >= data_now && if_type(wstring_to_string(Name))) {       
        delete_files.push_back(Name);

        wstring w = Name + L"\t\t" + std::to_wstring(stUTC.wDay) + L"." + std::to_wstring(stUTC.wMonth) + L"." + std::to_wstring(stUTC.wYear) + L" " +
            std::to_wstring(stUTC.wHour + 2) + L":" + std::to_wstring(stUTC.wMinute);
        result.push_back(wstring_to_string(w));

    }
    CloseHandle(hFile1);
}

void Find::find(wstring Name) {
    wstring name_help = Name;
    Name += L"\\*";
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf = FindFirstFile(Name.c_str(), &FindFileData);
    if (hf != INVALID_HANDLE_VALUE) {
        do {
            MSG msg;
            if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            if (wcscmp(FindFileData.cFileName, L".") != 0 && wcscmp(FindFileData.cFileName, L"..") != 0) {
                if (name_help == L"C:\\Windows" && window_folder == 0) { continue; }
                if (GetKeyState(VK_ESCAPE) < 0) {
                    Save();
                    exit(0);
                }
                wstring name = name_help + L"\\" + FindFileData.cFileName;
                if (!file_or_no(name)) {
                    find(name);
                }
                else {    
                    N++;               
                    Time(name);
                    if (N % 1000 == 0) {
                        wstring strtext_scan = L"Просканировано: " + std::to_wstring(N) + L".\nНайдено совпадений: " + std::to_wstring(result.size()) +
                            L"\nНе удалось просканировать " + std::to_wstring(err);
                        SetWindowText(text_scan, strtext_scan.c_str());
                        wstring strtext_time = L"Всего прошло времени: ";

                        float r = float(clock() - t) / 1000;
                        if (r > 60) {
                            int m = r / 60;
                            strtext_time += std::to_wstring(m) + L" минут " + std::to_wstring(int(r - m * 60)) + L" секунд";
                        }
                        else {
                            strtext_time += std::to_wstring(int(r)) + L" секунд" ;
                        }

                        SetWindowText(text_time, strtext_time.c_str());
                    }
                    
                }
            }
        } while (FindNextFile(hf, &FindFileData) != 0);
        FindClose(hf);
    }
}

wstring get_name(wstring Name) {
    wstring res = L"";
    int n = 0;
    for (int i = Name.size() - 1; i >= 0; i--) {
        if (Name[i] == '\\') { n = i; break; }
    }
    for (int i = n + 1; i < Name.size(); i++) {
        res += Name[i];
    }
    return res;
}

void Find::Save() {
    wstring t1 = L"\t\t\t\tСканирование завершено!\n\nВсего было просканировано: " + std::to_wstring(N) + L".\nНайдено совпадений: " + std::to_wstring(result.size()) +
        L"\nНе удалось просканировать " + std::to_wstring(err) + L" файлов.\nРезультат ";
    
    ofstream File1;
    File1.open(path(0) + "Result.txt");
    if (File1.is_open()) {
        for (int i = 0; i < result.size(); i++) {
            File1 << result[i];
            File1 << '\n';            
        }
        t1 += L"Успешно сохранен!\nПуть: " + string_to_wstring(path(0) + "Result.txt");
    }
    else t1 += L"Не удалось сохранить результат :(";
    File1.close();
    SetWindowText(text_scan, t1.c_str());

   
  
    if (autodel == 1) {
        string no_delete;
        int ok = 0;
        int if_finder = 0;
        wstring t2;
        for (int i = 0; i < delete_files.size(); i++) {     
            t2 = L"";     
            wstring file_name = get_name(delete_files[i]);           
            if (wstring_to_string(delete_files[i]) != path(1)) {                
                if (findprocess(file_name)) {
                    if (file_name != L"Finder 2") {
                        string str_name = "taskkill /f /im \"";
                        str_name += wstring_to_string(file_name);
                        str_name + '\"';
                        WinExec(str_name.c_str(), SW_HIDE);
                        Sleep(1000);

                        if (DeleteFile(delete_files[i].c_str()) == 1) {
                            ok++;
                        }
                        else {
                            no_delete += wstring_to_string(delete_files[i]);
                            no_delete += '\n';
                        }
                    }
                    else if_finder++;
                }
                else {
                    if (DeleteFile(delete_files[i].c_str()) == 1) {
                        ok++;
                    }
                    else {
                        no_delete += wstring_to_string(delete_files[i]);
                        no_delete += '\n';
                    }
                }
            }
            else if_finder++;
            t2 = L"Удалено: " + std::to_wstring(ok) + L" из " + std::to_wstring(delete_files.size() - if_finder) + L" файлов";
            SetWindowText(text_time, t2.c_str());
        }

        int N_no_delete = (delete_files.size() - if_finder) - ok;
        t2 += L"\nВсего не удалось удалить: " + std::to_wstring(N_no_delete) + L"\n";

        if (N_no_delete != 0) {
            ofstream File1;
            File1.open(path(0) + "No_delete.txt");
            if (File1.is_open()) {

                File1 << "Всего не удалось удалить: " << std::to_string(N_no_delete) << '\n';
                File1 << no_delete;

                t2 += L"\nСписок файлов которые не вышло удалить успешно сохранен!\nПуть: " + string_to_wstring(path(0) + "No_delete.txt");
            }
            else t2 += L"\nНе удалось сохранить список файлов которые не получилось удалить.";
            File1.close();
        }
        SetWindowText(text_time, t2.c_str());
    }
    else { 
        SetWindowText(text_time, L"Автоудаление файлов было отключено");
    }
}