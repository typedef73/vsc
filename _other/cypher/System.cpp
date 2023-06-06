#include "System.h"
#include <string>
#include <atlimage.h>
#include <iostream>

int sum_string(const std::string& data) {
	int abs = 0;
	for (int i = 0; i <= data.size(); i++) { abs += (data[i] + 128); }
	return abs;
}

void cypher_data(const std::string& data, const wchar_t* path) {
	CImage img;
	img.Load(path);
	HBITMAP bmp(img);
	HDC memDC = CreateCompatibleDC(NULL);
	SelectObject(memDC, bmp);

	float x = 2, y = 0, h = img.GetHeight(), w = img.GetWidth();
	int sum = sum_string(data);
	float step = float(h * w - 2) / sum;
	unsigned char color_data = 2;

	SetPixel(memDC, 0, 0, RGB((sum & 0xff000000) >> 24, (sum & 0x00ff0000) >> 16, (sum & 0x0000ff00) >> 8));
	SetPixel(memDC, 1, 0, RGB((sum & 0x000000ff), GetGValue(GetPixel(memDC, 1, 0)), color_data));
	for (int i = 0; i <= data.size(); i++) {
		COLORREF color = 0;
		if (color_data == 0) color = RGB(data[i] + 128, GetGValue(GetPixel(memDC, x, y)), GetBValue(GetPixel(memDC, x, y)));
		if (color_data == 1) color = RGB(GetRValue(GetPixel(memDC, x, y)), data[i] + 128, GetBValue(GetPixel(memDC, x, y)));
		if (color_data == 2) color = RGB(GetRValue(GetPixel(memDC, x, y)), GetGValue(GetPixel(memDC, x, y)), data[i] + 128);

		SetPixel(memDC, x, y, color);
		x += step * (data[i] + 128);
		for (;;) { if (x > w - 1) { x -= w; y++; } else break; }
	}
 
	img.Attach(bmp);
	std::wstring save = path;
	for (int i = save.size() - 1; i >= 0; i--) if (save[i] == '.') { save.pop_back(); break; } else save.pop_back();
	save += L"_new.png";
	img.Save(save.c_str());

	img.Destroy();
	DeleteObject(bmp);
}

void decypher_data(std::string& data, const wchar_t* path) {
	data.clear();
	CImage img;
	img.Load(path);
	HBITMAP bmp(img);
	HDC memDC = CreateCompatibleDC(NULL);
	SelectObject(memDC, bmp);

	float x = 2, y = 0, h = img.GetHeight(), w = img.GetWidth();;
	int sum = 0;
	COLORREF c = GetPixel(memDC, 0, 0);
	sum = (GetRValue(c) << 24) | (GetGValue(c) << 16) | (GetBValue(c) << 8) | (GetRValue(GetPixel(memDC, 1, 0)));
	unsigned char color_data = GetBValue(GetPixel(memDC, 1, 0));
	float step = float(h * w - 2) / sum;
	for (;;) {
		unsigned char data_c = 0;
		if (color_data == 0) data_c = GetRValue(GetPixel(memDC, x, y));
		if (color_data == 1) data_c = GetGValue(GetPixel(memDC, x, y));
		if (color_data == 2) data_c = GetBValue(GetPixel(memDC, x, y));

		if (data_c == 128) break;
		data += data_c - 128;
		x += step * data_c;
		for (;;) { if (x > w - 1) { x -= w; y++; } else break; }
		if (y > h - 1) std::cout << "ERROR_READ!\n";
	}

	img.Destroy();
	DeleteObject(bmp);
}