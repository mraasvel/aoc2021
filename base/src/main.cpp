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
	while (std::getline(file, line)) {
	}
	file.close();

	return 0;
}
