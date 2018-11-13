#pragma once

#define STB_IMAGE_IMPLEMENTATION    
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <Windows.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "image_process.h"
#include <string>
#include <thread>
#include "util.h"

using namespace std;

class ProcessJob {
	vector<wstring> & files;
	int threshold;
	bool writeImages;
	int init;
	int step;
	string dir;
public:

	ProcessJob(vector<wstring> & _files, string _dir, int _threshold, bool _writeImages, int _init, int _step) :
		files(_files), dir(_dir), threshold(_threshold), writeImages(_writeImages), init(_init), step(_step) {};

	void run() {
		for (int i = init; i < files.size(); i += step) {
			int w, h, n;
			string fname(files[i].begin(), files[i].end());
			cout << "Processing " << fname << endl;

			unsigned char *data = stbi_load(fname.c_str(), &w, &h, &n, 0);
			if (data != nullptr) {

				Image img(data, w, h, n);

				fname = string(fname.begin(), fname.end() - 4);
				vector<Point2D> store = vertical_edge_detection(img, threshold);
				writeTxtPoint(dir, fname, ".temp", store);
				if (writeImages) {
					for (int j = 0; j < store.size(); ++j) img.paintYellow(store[j].x, store[j].y);
					fname = dir + fname + ".jpg";
					stbi_write_jpg(fname.c_str(),  w, h, n, data, 100);
				}
			}
		}
	}
};

void processImages(vector<wstring> & files, int threshold, bool writeImages) {

	unsigned threads = std::thread::hardware_concurrency();
	threads = (threads == 0 ? 1 : threads);

    CreateDirectory(L".\\yellow", NULL);

	vector<thread *> jobs(threads);

	for (int i = 0; i < threads; ++i) {
		ProcessJob *myJob = new ProcessJob(files, ".\\yellow\\", threshold, writeImages, i, threads);
		jobs[i] = new thread(&ProcessJob::run, myJob);
	}

	for (int i = 0; i < threads; ++i) {
		jobs[i]->join();
	}

	for (int i = 0; i < threads; ++i) {
		delete jobs[i];
	}
}


void joinFiles(vector<wstring> & files, string fileDir, string filename) {
	ofstream myFile(fileDir + filename, ios::trunc|ios_base::binary);

	if (myFile.good()) {
		for (int i = 0; i < files.size(); ++i) {
			string fo = fileDir + string(files[i].begin(), files[i].end());
			ifstream toWrite(fo, std::ios_base::binary);
			if (toWrite.good()) {
				myFile << toWrite.rdbuf() << endl;
				toWrite.close();
				DeleteFileA(fo.c_str());
			}
			else {
				wcout << L"Could not open file: " << files[i] << endl;
			}
		}
		myFile.close();
	}
	else {
		cout << "Could not create file: " << filename << " and merge point files" << endl;
	}
}