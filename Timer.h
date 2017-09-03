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
 
