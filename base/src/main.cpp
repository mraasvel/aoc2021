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

void Line(const std::string& line, size_t index) {

}

int main() {

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	size_t i = 0;
	while (std::getline(file, line)) {
		Line(line, i);
		++i;
	}
	file.close();

	return 0;
}
