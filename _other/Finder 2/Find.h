#pragma once
#include <vector>
#include <string>
#include <Windows.h>

using std::string;
using std::wstring; 
using std::vector;

class Find {
	HWND text_scan;
	HWND text_time;
	HWND hwnd;

	vector<wstring> files;
	vector<string> result;
	vector<string> types;

	vector<wstring> delete_files;

	int err = 0;
	int year_1 = 0, month_1 = 0, day_1 = 0;
	int year_2 = 0, month_2 = 0, day_2 = 0;
	unsigned int N = 0;
	unsigned int t = 0;

	bool autodel, window_folder;

	int Time(wstring Name);
	bool if_type(string name);
	void find(wstring Name);
	void Save();

public:
	Find(bool b_autodel, bool b_window_folder, HWND &hwnd, wstring w_path, wstring w_date_start, wstring w_date_end, wstring w_file_type);
};