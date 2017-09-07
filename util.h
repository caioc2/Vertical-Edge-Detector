#pragma once
#include <Windows.h>
#include <vector>
#include <regex>
#include <fstream>

using namespace std;

vector<wstring> getCurrentDirImagesFileList() {
	WIN32_FIND_DATA search_data;
	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));

	TCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);
	wstring cd(currentDir);
	HANDLE handle = FindFirstFile((cd + L"\\*").c_str(), &search_data);

	vector<wstring> fileList;

	wregex r(L".*\.(jpg|JPG|png|PNG|bmp|BMP)$");

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
void writeTxtPoint(string filename, vector< vector<T> > vec) {
	ofstream myFile(filename, ios::trunc);

	if (myFile.good()) {
		myFile << getFormatedDate() << endl << endl;
		myFile << "X\tY\tT" << endl;
		for (int i = 0; i < vec.size(); ++i) {
			for (int j = 0; j < vec[i].size(); ++j) {
				myFile << vec[i][j].x << "\t" << vec[i][j].y << "\t" << i << endl;
			}
		}
		myFile.close();
	}
	else {
		cout << "Could not create file: " << filename << endl;
	}
}
