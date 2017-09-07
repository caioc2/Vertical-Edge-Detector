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
	vector< vector<Point2D> > & store;
public:

	ProcessJob(vector<wstring> & _files, vector< vector<Point2D> > & _store, string _dir, int _threshold, bool _writeImages, int _init, int _step) :
		files(_files), store(_store), dir(_dir), threshold(_threshold), writeImages(_writeImages), init(_init), step(_step) {};

	void run() {
		for (int i = init; i < files.size(); i += step) {
			int w, h, n;
			string fname(files[i].begin(), files[i].end());
			cout << "Processing " << fname << endl;

			unsigned char *data = stbi_load(fname.c_str(), &w, &h, &n, 0);
			if (data != nullptr) {

				Image img(data, w, h, n);
				store[i] = vertical_edge_detection(img, threshold);
				fname = dir + string(fname.begin(), fname.end() - 3);
				if (writeImages) {
					for (int j = 0; j < store[i].size(); ++j) img.paintYellow(store[i][j].x, store[i][j].y);
					fname += "jpg";
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

	vector< vector<Point2D> > allPoints(files.size());

	vector<thread *> jobs(threads);

	for (int i = 0; i < threads; ++i) {
		ProcessJob *myJob = new ProcessJob(files, allPoints, ".\\yellow\\", threshold, writeImages, i, threads);
		jobs[i] = new thread(&ProcessJob::run, myJob);
	}

	for (int i = 0; i < threads; ++i) {
		jobs[i]->join();
	}

	for (int i = 0; i < threads; ++i) {
		delete jobs[i];
	}

	writeTxtPoint(".\\yellow\\yellow.txt" , allPoints);
}

