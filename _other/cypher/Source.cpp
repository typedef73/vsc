#include <iostream>
#include <string>
#include <Windows.h>
#include <atlimage.h>
#include <fstream>
#include "System.h"
using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

bool txt_or_img = 0;
HWND edit_1;
HWND edit_2;
HWND txt;
HWND txt2;
HWND button_start;
HWND reverse_button;
HWND txt_mes;
HDC bmpDC = CreateCompatibleDC(NULL);
HBITMAP bmp;
CImage img;

std::wstring data_edit_1 = L"C:";
std::wstring data_edit_2 = L"C:";

bool is_png(std::wstring s) {
	return s[s.size() - 1] == 'g' && s[s.size() - 2] == 'n' && s[s.size() - 3] == 'p' && s[s.size() - 4] == '.';
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_DESTROY)
		PostQuitMessage(0);
	if (msg == WM_COMMAND) {
		if (lParam == LPARAM(edit_1)) {
			int nc = HIWORD(wParam);
			wchar_t c[500];

			GetWindowText(HWND(lParam), c, 300);
			if (nc == EN_UPDATE) {
				data_edit_1 = c;
				if (txt_or_img) {
					if (img.Load(c) == 0) {
						RECT rct;
						GetWindowRect(hwnd, &rct);
						SetWindowPos(hwnd, HWND_TOP, rct.left, rct.top, 10, 10, NULL);
						SetWindowPos(hwnd, HWND_TOP, rct.left, rct.top, 300 + img.GetWidth() > 600 ? 400 + img.GetWidth() : 600,
							100 + img.GetHeight() > 300 ? 100 + img.GetHeight() : 300, NULL);
						bmp = img;
						SelectObject(bmpDC, bmp);
					}
				}
			}
		}

		if (lParam == LPARAM(edit_2)) {
			int nc = HIWORD(wParam);
			wchar_t c[500];

			GetWindowText(HWND(lParam), c, 300);
			if (nc == EN_UPDATE) {
				if (!txt_or_img) data_edit_2 = c;
			}
		}

		if (lParam == LPARAM(button_start)) {
			if (!txt_or_img) {
				ifstream test;
				test.open(data_edit_2);
				bool open = 0;
				if (test.is_open()) {
					test.close();
					test.open(data_edit_1);
					if (test.is_open()) {
						open = 1;
						char c;
						std::string data;
						while (test.get(c)) {
							data += c;
						}
						cypher_data(data, data_edit_2.c_str());
						SetWindowText(txt_mes, L"Файл зашифрован!");
					}
					test.close();
				}
				else test.close();
				if (!open) {
					SetWindowText(txt_mes, L"Файл не найден!");
				}
			}
			else {
				if (is_png(data_edit_1)) {
					string read_data;
					decypher_data(read_data, data_edit_1.c_str());
					ofstream file;
					file.open(data_edit_1 + L".txt");
					if (file.is_open()) {
						file << read_data;
					}
					cout << read_data << endl;
					SetWindowText(txt_mes, L"Файл дешифрован!");
				}
				else SetWindowText(txt_mes, L"PNG файл не найден!");
			}
		}

		if (lParam == LPARAM(reverse_button)) {
			if (!txt_or_img) {
				ShowWindow(edit_2, SW_HIDE);
				ShowWindow(txt2, SW_HIDE);
				SetWindowText(txt, L"Путь к PNG файлу");
				SetWindowText(txt_mes, L"\tДешифратор");
				RECT rct;
				GetWindowRect(hwnd, &rct);
				SetWindowPos(hwnd, HWND_TOP, rct.left, rct.top, 300 + img.GetWidth() > 600 ? 400 + img.GetWidth() : 600,
					100 + img.GetHeight() > 300 ? 100 + img.GetHeight() : 300, NULL);
				txt_or_img = 1;
			}
			else {
				ShowWindow(edit_2, SW_SHOW);
				ShowWindow(txt2, SW_SHOW);
				SetWindowText(txt, L"Путь к файлу с данными");
				SetWindowText(txt_mes, L"\tШифратор");
				RECT rct;
				GetWindowRect(hwnd, &rct);
				SetWindowPos(hwnd, HWND_TOP, rct.left, rct.top, 10, 10, NULL);
				SetWindowPos(hwnd, HWND_TOP, rct.left, rct.top, 600, 300, NULL);
				txt_or_img = 0;
			}
		}
	}
	else if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		if (txt_or_img) BitBlt(GetDC(hwnd), 350, 50, img.GetWidth(), img.GetHeight(), bmpDC, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	WNDCLASSW wcl;
	memset(&wcl, 0, sizeof(WNDCLASSW));
	wcl.lpszClassName = L"my Window";
	wcl.lpfnWndProc = WndProc;
	RegisterClassW(&wcl);

	HWND hwnd = CreateWindow(L"my Window", L"image to text", WS_OVERLAPPEDWINDOW, 200, 100, 600, 300, NULL, NULL, NULL, NULL);
	edit_1 = CreateWindow(L"edit", L"C:", WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL, 10, 100, 200, 60, hwnd, 0, 0, 0);
	edit_2 = CreateWindow(L"edit", L"C:", WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL, 360, 100, 200, 60, hwnd, 0, 0, 0);
	txt = CreateWindow(L"static", L"Путь к файлу с данными", WS_VISIBLE | WS_CHILD, 20, 40, 180, 20, hwnd, 0, 0, 0);
	txt2 = CreateWindow(L"static", L"Путь к картинке, в которую\nнужно зашифровать данные", WS_VISIBLE | WS_CHILD, 360, 40, 200, 40, hwnd, 0, 0, 0);
	button_start = CreateWindow(L"button", L"start", WS_VISIBLE | WS_CHILD, 230, 170, 100, 50, hwnd, 0, 0, 0);
	txt_mes = CreateWindow(L"static", L"\tШифратор", WS_VISIBLE | WS_CHILD, 190, 7, 200, 20, hwnd, 0, 0, 0);
	reverse_button = CreateWindow(L"button", L"↻", WS_VISIBLE | WS_CHILD, 255, 60, 50, 50, hwnd, 0, 0, 0);
	ShowWindow(hwnd, SW_NORMAL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_QUIT) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteObject(hwnd);
	DeleteDC(bmpDC);
	DeleteObject(bmp);
	return 0;
}