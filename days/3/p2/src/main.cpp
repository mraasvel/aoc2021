#include "util/util.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <utility>


#define FILENAME "input.txt"
// #define FILENAME "sample.txt"

using namespace std;

// FIRST = 0, SECOND = 1
vector<string> lines;
vector<string> ogr;
vector<string> co2;

void createOgr(size_t pos, char c) {
	vector<string> temp;
	if (ogr.size() == 1) {
		return;
	}
	temp.swap(ogr);
	for (auto s : temp) {
		if (s[pos] == c) {
			ogr.push_back(s);
		}
	}
}

void createCo2(size_t pos, char c) {
	vector<string> temp;
	if (co2.size() == 1) {
		return;
	}
	temp.swap(co2);
	for (auto s : temp) {
		if (s[pos] == c) {
			co2.push_back(s);
		}
	}
}

pair<int, int> countBits(vector<string> const & v, size_t pos) {
	pair<int, int> cnt;
	for (auto s : v) {
		if (s[pos] == '0') {
			cnt.first += 1;
		} else {
			cnt.second += 1;
		}
	}
	return cnt;
}

int main() {


	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}
	file.close();

	ogr = lines;
	co2 = lines;

	for (std::size_t i = 0; i < lines[0].size(); ++i) {
		pair<int, int> bitogr = countBits(ogr, i);
		pair<int, int> bitco2 = countBits(co2, i);
		createOgr(i, bitogr.first > bitogr.second ? '0' : '1');
		createCo2(i, bitco2.first > bitco2.second ? '1' : '0');
	}

	cout << ogr[0] << endl;
	cout << co2[0] << endl;

	int v1 = stoi(ogr[0], NULL, 2);
	int v2 = stoi(co2[0], NULL, 2);
	cout << v1 << ", " << v2 << endl;
	cout << v1 * v2 << endl;
	return 0;
}
