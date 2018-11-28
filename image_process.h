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

	You should have received a copy of the GNU General Public License
	along with Vertical Edge Detector.  If not, see <https://www.gnu.org/licenses/>.
**/

#pragma once
#include "stb_image.h"
#include <stdlib.h>
#include <assert.h>
#include <vector>

using namespace std;


struct Point2D {
	unsigned __int16 x;
	unsigned __int16 y;
};

const unsigned char yellow[] = { 0xFF, 0xFF, 0x0, 0xFF };

class Image {
public:
	unsigned char * _data;
	const unsigned int _width;
	const unsigned int _height;
	const unsigned int _n;


	Image(unsigned char * data, unsigned int width, unsigned int height, unsigned int n) : _data(data), _width(width), _height(height), _n(n) {};

	unsigned char * operator()(int i, int j) {
		return &_data[_n * (_width * i + j)];
	}

	void paintYellow(int i, int j) {
		for (int k = 0; k < _n; ++k) (*this)(i, j)[k] = yellow[k];
	}

	void paintBlack(int i, int j) {
		for (int k = 0; k < _n; ++k) (*this)(i, j)[k] = 0x0;
	}

	~Image() {
		stbi_image_free(_data);
	}
};

int rgb_2_gray(unsigned char * pix) {
	return (13926 * (int)pix[0] + 46884 * (int)pix[1] + 4726 * (int)pix[2]) >> 16;
}

vector<Point2D> vertical_edge_detection_gray(Image & im, int threshold) {
	vector<Point2D> ret;
	ret.reserve(im._height * im._width / 50);
	for (unsigned __int16 j = 0; j < im._width; ++j) {
		bool push = false;
		Point2D p;
		int min = 1000000;
		for (unsigned __int16 i = 4; i < im._height - 5; ++i) {
			int left = (im(i - 4, j)[0] + im(i - 3, j)[0] + im(i - 2, j)[0]) / 3;
			int center = (im(i - 1, j)[0] + im(i, j)[0] + im(i + 1, j)[0]) / 3;
			int right = (im(i + 4, j)[0] + im(i + 3, j)[0] + im(i + 2, j)[0]) / 3;

			int val = right + left - 2 * center;
			if (val < threshold) {
				if (min > val) {
					min = val;
					push = true;
					p = { j, i };
				}
			}
			else {
				if (push) {
					push = false;
					min = 1000000;
					ret.push_back(p);
				}
			}
		}
	}
	return ret;
}

vector<Point2D> vertical_edge_detection_rgb(Image & im, int threshold) {
	vector<Point2D> ret;
	ret.reserve(im._height * im._width / 50);
	for (unsigned __int16 j = 0; j < im._width; ++j) {
		bool push = false;
		Point2D p;
		int min = 1000000;
	    for (unsigned __int16 i = 4; i < im._height-5; ++i) {
			int left = (rgb_2_gray(im(i - 4, j)) + rgb_2_gray(im(i - 3, j)) + rgb_2_gray(im(i - 2, j))) / 3;
			int center = (rgb_2_gray(im(i - 1, j)) + rgb_2_gray(im(i, j)) + rgb_2_gray(im(i + 1, j))) / 3;
			int right = (rgb_2_gray(im(i + 4, j)) + rgb_2_gray(im(i + 3, j)) + rgb_2_gray(im(i + 2, j))) / 3;

			int val = right + left - 2 * center;
			if (val < threshold) {
				if (min > val) {
					min = val;
					push = true;
					p = { j, i};
				}
			}
			else {
				if (push) {
					push = false;
					min = 1000000;
					ret.push_back(p);
				}
			}
		}
	}
	return ret;
}

vector<Point2D> vertical_edge_detection(Image & im, int threshold) {

	if (im._n == 1) {
		return vertical_edge_detection_gray(im, threshold);
	}
	else if(im._n == 3) {
		return vertical_edge_detection_rgb(im, threshold);
	}
	else {
		cout << "Alpha channel image not supported. " << endl;
		return vector<Point2D>();
	}
}