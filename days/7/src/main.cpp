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

long summation(long n) {
	return n * (n + 1) / 2;
}

long computeFuel(vector<long> const & crabs, long m) {
	long total = 0;
	for (auto c : crabs) {
		total += summation(abs(m - c));
	}
	return total;
}

int main() {

	vector<long> crabs;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		auto v = util::splitString(line, ',');
		for_each(v.begin(), v.end(), [&crabs] (const string& val) -> void {
			crabs.push_back(stoull(val));
		});
	}
	file.close();

	long min = *min_element(crabs.begin(), crabs.end());
	long max = *max_element(crabs.begin(), crabs.end());

	long fuel = std::numeric_limits<long>::max();
	while (min <= max) {
		fuel = std::min(fuel, computeFuel(crabs, min));
		++min;
	}

	cout << fuel << endl;

	return 0;
}
