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

	std::vector<int> v;
	while (std::getline(file, line)) {
		v.push_back(std::stoi(line));
	}
	file.close();


	int cnt = 0;
	int sum = 0;
	sum = v[0] + v[1] + v[2];
	for (size_t i = 3; i < v.size(); ++i) {

		sum -= v[i - 3];

		cout << sum + v[i] << ", " << sum + v[i - 3] << endl;
		if (sum + v[i] > sum + v[i - 3]) {
			++cnt;
		}
		sum += v[i];
	}

	cout << cnt << std::endl;
	return 0;
}
