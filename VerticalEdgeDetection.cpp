// VerticalEdgeDetection.cpp : Defines the entry point for the console application.
//
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"

#include "stdafx.h"
#include <iostream>
#include "Timer.h"
#include "image_process.h"
#include "process_files.h"
#include "util.h"



int main(int argc, char* argv[])
{
	

	if (argc < 3) {
		cout << "Usage: VectorialEdgeDetection.exe <options>" << endl;
		cout << "Process all  bmp, png and jpg files in the current folder" << endl;
		cout << "output \".\\yellow\\\<imageName\>.txt\"" << endl;
		cout << "<options> :" << endl;
		cout << "-t \"#n\"\t: threshold, an integer (-255, 255)" << endl;
		cout << "-w\t: write image with detected points in yellow to \".\\yellow\" folder" << endl;

		system("PAUSE");
		return -1;
	}

	int threshold = 0;
	bool write = false;
	
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
			i++;
			try {
				threshold = stoi(argv[i]);
			}
			catch (exception e) {
				cout << "Expected a number after -t";
				system("PAUSE");
				return -1;
			}
		}
		else if (strcmp(argv[i], "-w") == 0) {
			write = true;
		}
		else {
			cout << "Unknow option: " << argv[i] << endl;
			system("PAUSE");
			return -1;
		}
	}

	
	cout << "Threshold = " << threshold << ", writeImages = " << (write ? "true" : "false") << endl << endl;

	cout << "Reading files in the directory" << endl;

	Timer t;
	t.tick();

	vector<wstring> files = getCurrentDirImagesFileList();
	cout << "Found " << files.size() << " images, in " << t.tock() << endl;

	cout << "Processing images..." << endl << endl;


	t.tick();
	processImages(files, threshold, write);
	cout << "Done, total time: " << t.tock() << endl << endl;

	system("PAUSE");
    return 0;
}

