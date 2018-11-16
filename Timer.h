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
#include <Windows.h>
#include <string>
#pragma once

std::string pad2(std::string & in) {
	if (in.size() < 2) {
		in = "0" + in;
	}
	return in;
}

class Timer {
	long total;

public:
	Timer() {};

	void tick() {
		total = GetTickCount();
	}

	std::string tock() {
		total = GetTickCount() - total;
		int mil = total % 1000;
		total = total / 1000;
		int s = total % 60;
		total = total / 60;
		int min = total % 60;
		total = total / 60;
		
		std::string ret = pad2(std::to_string(total)) + ":" + pad2(std::to_string(min)) + ":" + pad2(std::to_string(s)) + " .:. " + pad2(std::to_string(mil));

		return ret;
	}
};
 
