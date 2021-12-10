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
vector<std::pair<long, long> > v;

long calcGamma() {
	long n = 0;
	long i = 0;
	for (auto it = v.rbegin(); it != v.rend(); ++it) {
		if (it->first > it->second) {
			// 1 is least common
		} else {
			n += (1 << i);
			// n = (n * 2) + 1;
		}
		++i;
	}
	return n;
}

long calcEpsilon() {
	long n = 0;
	long i = 0;
	for (auto it = v.rbegin(); it != v.rend(); ++it) {
		if (it->first < it->second) {
			// 0 is least common
		} else {
			n += (1 << i);
		}
		++i;
	}
	return n;
}

int main() {


	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		if (v.size() == 0) {
			v.resize(line.size(), make_pair(0, 0));
		}
		for (size_t i = 0; i < line.size(); ++i) {
			if (line[i] == '0') {
				v[i].first += 1;
			} else {
				v[i].second += 1;
			}
		}
	}
	file.close();

	for (auto x : v) {
		cout << (x.first > x.second ? '0' : '1');
	}
	cout << endl;

	cout << "Gamma: " << calcGamma() << endl;
	cout << "Epsilon: " << calcEpsilon() << endl;
	cout << calcGamma() * calcEpsilon() << endl;

	return 0;
}
