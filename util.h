/**
	This file is part of Vertical Edge Detector.

	Copyright (C) 2018 - Caio SOUZA

	Vertical Edge Detector is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Vertical Edge Detector is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

**/
#pragma once
#include <Windows.h>
#include <vector>
#include <regex>
#include <fstream>

using namespace std;

vector<wstring> getFileList(wstring cd = wstring(), wregex r = wregex(L".*\.(jpg|JPG|png|PNG|bmp|BMP)$")) {
	WIN32_FIND_DATA search_data;
	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));

	if (cd.empty()) {
		TCHAR currentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, currentDir);
		cd = wstring(currentDir);
	}
	
	HANDLE handle = FindFirstFile((cd + L"\\*").c_str(), &search_data);

	vector<wstring> fileList;


	while (handle != INVALID_HANDLE_VALUE)
	{
		if (regex_match(search_data.cFileName, r)) {
			fileList.push_back(search_data.cFileName);
		}

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}
	FindClose(handle);

	return fileList;
}



string getFormatedDate() {
	std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char buffer[200];
	ctime_s(buffer, 200, &end_time);
	return string(buffer);
}

template<class T>
void writeTxtPoint(string dir, string filename, string ext, vector<T> vec) {
	ofstream myFile(dir+filename+ext, ios::trunc);
	
	if (myFile.good()) {
		myFile << filename << endl << endl;
		myFile << "(X)\t(Y)" << endl;
		for (int i = 0; i < vec.size(); ++i) {
			myFile << vec[i].x << "\t" << vec[i].y << endl;
		}
		myFile.close();
	}
	else {
		cout << "Could not create file: " << filename << endl;
	}
}
