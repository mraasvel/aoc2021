#include "util/util.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define FILENAME "input.txt"
// #define FILENAME "sample.txt"

using namespace std;

int main() {

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;

	long depth = 0;
	long pos = 0;
	long aim = 0;
	while (std::getline(file, line)) {
		auto v = util::splitString(line, ' ');
		if (v[0] == "forward") {
			depth += aim * std::stol(v[1]);
			pos += std::stol(v[1]);
		} else if (v[0] == "down") {
			aim += std::stol(v[1]);
		} else {
			assert(v[0] == "up");
			aim -= std::stol(v[1]);
		}
	}
	file.close();

	cout << depth << ", " << pos << endl;
	cout << depth * pos << endl;

	return 0;
}
