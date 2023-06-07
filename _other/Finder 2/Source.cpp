#include <Windows.h>
#include <iostream>
#include <string>
#include "System.h"
#include "Find.h"

using namespace::std;

HWND check_win;
#define id_chek_win HMENU(1)
HWND check_del;
HWND check_autodel;
#define id_chek_autodel HMENU(2)
HWND edit_path;
HWND start_date;
HWND end_date;
HWND edit_type;
HWND start;
HDC dc;

int W = 600;
int H = 420;

bool window_folder;
bool autodel;
bool mode_find = 0;

////////////////
wstring find_path = L"C:";
wstring date_start = L"";
wstring date_end = L"";
wstring file_type = L"";
///////////////
HWND text_path;
HWND text_date_start;
HWND text_date_end;
HWND text_type;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {	
	if (msg == WM_DESTROY) PostQuitMessage(0);
	else if (msg == WM_PARENTNOTIFY) {
		HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
		SendMessage((HWND)lParam, WM_SETFONT, (WPARAM)hFont, TRUE);
	}
	else if (msg == WM_COMMAND){
		if (lParam == LPARAM(check_win)) {
			HWND hwndCheck = GetDlgItem(hwnd, int(id_chek_win));
			LRESULT res = SendMessage(hwndCheck, BM_GETCHECK, 0, 0);

			if (res == BST_CHECKED) {
				window_folder = 1;
				ShowWindow(check_autodel, SW_HIDE);
			}
			if (res == BST_UNCHECKED) {
				ShowWindow(check_autodel, SW_SHOW);
				window_folder = 0;
			}

			if (find_path == L"C:\\Windows" || find_path == L"C:\\Windows\\") {
				window_folder = 1;
			}
		}

		if (lParam == LPARAM(check_autodel)) {
			HWND hwndCheck = GetDlgItem(hwnd, int(id_chek_autodel));
			LRESULT res = SendMessage(hwndCheck, BM_GETCHECK, 0, 0);

			if (res == BST_CHECKED) { 
				autodel = 1; 
				window_folder = 0;
				ShowWindow(check_win, SW_HIDE);
			}
			if (res == BST_UNCHECKED) { 
				ShowWindow(check_win, SW_SHOW);
				/////////
				HWND hwndCheck = GetDlgItem(hwnd, int(id_chek_win));
				LRESULT res = SendMessage(hwndCheck, BM_GETCHECK, 0, 0);

				if (res == BST_CHECKED) window_folder = 1;
				if (res == BST_UNCHECKED) window_folder = 0;
				/////////

				autodel = 0;
			}
		}


		if (lParam == LPARAM(edit_path)) {
			int nc = HIWORD(wParam);
			wchar_t c[300];

			GetWindowText(HWND(lParam), c, 300);
			if (nc == EN_UPDATE) {
				find_path = c;
				if (find_path == L"C:" || find_path == L"C:\\") {
					if (!autodel) ShowWindow(check_win, SW_SHOW);
				}
				else {
					ShowWindow(check_win, SW_HIDE);
					ShowWindow(check_autodel, SW_SHOW);
				}
			}
		}
		
		if (lParam == LPARAM(start_date)) {
			int nc = HIWORD(wParam);
			wchar_t c[100];
			GetWindowText(HWND(lParam), c, 100);
			if (nc == EN_UPDATE) date_start = c;			
		}
		if (lParam == LPARAM(end_date)) {
			int nc = HIWORD(wParam);
			wchar_t c[100];
			GetWindowText(HWND(lParam), c, 100);
			if (nc == EN_UPDATE) date_end = c;			
		}
		if (lParam == LPARAM(edit_type)) {
			int nc = HIWORD(wParam);
			wchar_t c[100];
			GetWindowText(HWND(lParam), c, 100);
			if (nc == EN_UPDATE) file_type = c;			
		}


		if (lParam == LPARAM(start)) {		
			DestroyWindow(check_win);
			DestroyWindow(check_del);
			DestroyWindow(check_autodel);
			DestroyWindow(edit_path);
			DestroyWindow(start_date);
			DestroyWindow(end_date);
			DestroyWindow(edit_type);
			DestroyWindow(start);
			DestroyWindow(text_path);
			DestroyWindow(text_date_start);
			DestroyWindow(text_date_end);
			DestroyWindow(text_type);


			Find f(autodel, window_folder, hwnd, find_path, date_start, date_end, file_type);
			mode_find = 1;
		}
	}
	else if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int main(int argc, char *argv[]) {
	HWND consloe_hwnd = GetConsoleWindow();
	ShowWindow(consloe_hwnd, SW_HIDE);
	path(0, argv[0]);
	WNDCLASSW wcl;
	memset(&wcl, 0, sizeof(WNDCLASSW));
	wcl.lpszClassName = L"my Window";
	wcl.lpfnWndProc = WndProc;
	RegisterClassW(&wcl);

	HWND hwnd = CreateWindow(L"my Window", L"Finder 2.0", WS_OVERLAPPEDWINDOW, 200, 100, W, H, NULL, NULL, NULL, NULL);
	dc = GetDC(hwnd);

	edit_path = CreateWindow(L"edit", L"C:", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 40, 450, 25, hwnd, NULL, NULL, NULL);

	check_win = CreateWindow(L"button", L"Сканировать папку C:\\Windows", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BST_CHECKED, 
		10, 80, 450, 30, hwnd, id_chek_win, NULL, NULL);

	start_date = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 190, 150, 25, hwnd, NULL, NULL, NULL);
	end_date = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 300, 190, 150, 25, hwnd, NULL, NULL, NULL);

	text_path = CreateWindow(L"static", L"Папка/диск для сканириования файлов по дате: ", WS_VISIBLE | WS_CHILD, 10, 10, 450, 25, hwnd, NULL, NULL, NULL);
	text_date_start = CreateWindow(L"static", L"Пример: 3/1/2023", WS_VISIBLE | WS_CHILD, 170, 185, 90, 40, hwnd, NULL, NULL, NULL);
	text_date_end = CreateWindow(L"static", L"Пример: 5/1/2023", WS_VISIBLE | WS_CHILD, 460, 185, 90, 40, hwnd, NULL, NULL, NULL);

	check_autodel = CreateWindow(L"button", L"Авто-удаление найденых файлов (не совестимо со сканированием C:\\Windows)", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BST_CHECKED,
		10, 120, 550, 30, hwnd, id_chek_autodel, NULL, NULL);

	text_type = CreateWindow(L"static", L"Типы искомых файлов через запятую без пробелов. Пример: exe,com,bat  (путстое поле - все файлы)",
		WS_VISIBLE | WS_CHILD, 10, 230, 400, 40, hwnd, NULL, NULL, NULL);
	edit_type = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 280, 500, 25, hwnd, NULL, NULL, NULL);

	start = CreateWindow(L"button", L"Начать сканирование", WS_VISIBLE | WS_CHILD, 200, 330, 170, 40, hwnd, NULL, NULL, NULL);

	ShowWindow(hwnd, SW_NORMAL);

	MSG msg;

	while (1) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
			
		Sleep(5);
	}	
	DestroyWindow(hwnd);
	return 0;
}