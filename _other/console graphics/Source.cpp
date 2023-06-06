#include <iostream>
#include <Windows.h>
#include <atlimage.h>

using std::cout;
using std::endl;

char symbols[] = {'.', ',', ':', '+', '*', '?', '%', 'S', '#', '@'};

bool GetFile(TCHAR* szFile, int length, HWND hWnd = 0);

void SetColorText(HANDLE handle, bool r, bool g, bool b) {
	SetConsoleTextAttribute(handle, (r ? FOREGROUND_RED : 0) | (g ? FOREGROUND_GREEN : 0) | (b ? FOREGROUND_BLUE : 0) | FOREGROUND_INTENSITY);
}

void SetSizeText(HANDLE handle, int x, int y) {
	CONSOLE_FONT_INFOEX fontInfo = {0};
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.dwFontSize.X = x;
	fontInfo.dwFontSize.Y = y;
	SetCurrentConsoleFontEx(handle, FALSE, &fontInfo);
}

int main() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetColorText(handle, 1, 1, 1);
	SetSizeText(handle, 2, 2);
	SetWindowPos(GetConsoleWindow(), NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN) * 1, GetSystemMetrics(SM_CYSCREEN) * 0.97, NULL);
	
	wchar_t path[500] = {};
	if (!GetFile(path, sizeof(path))) return -1;

	CImage img;
	img.Load(path);			

	float kof = 1;


	if (img.GetWidth() * 2.1 > GetSystemMetrics(SM_CXSCREEN)) {
		kof = float(img.GetWidth()) * 2.1 / (GetSystemMetrics(SM_CXSCREEN));
	}
	if (img.GetHeight() * 2.1 > GetSystemMetrics(SM_CYSCREEN) * 0.97) {
		float new_kof = float(img.GetHeight()) * 2.1 / (GetSystemMetrics(SM_CYSCREEN) * 0.97);
		if (new_kof > kof) {
			kof = new_kof;
		}
	}

	int h = img.GetHeight() / kof, w = img.GetWidth() / kof;


	for (int i = 0; i < h; i += 1) {
		for (int j = 0; j < w; j += 1) {			
			COLORREF color = img.GetPixel(j * kof, i * kof);
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			int N = ((r + g + b) / 3) * (float(sizeof(symbols) - 1) / 255);

			SetColorText(handle, r > 127 ? 1 : 0, g > 127 ? 1 : 0, b > 127 ? 1 : 0);

			cout << symbols[N];
		}
		cout << '\n';
	}

	img.Destroy();
	system("pause");
	return 0;
}

bool GetFile(TCHAR *szFile, int length, HWND hWnd) {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = length;
	ofn.lpstrFilter = TEXT("Images\0*.png;*.jpg\0Other\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return GetOpenFileName(&ofn);
}
