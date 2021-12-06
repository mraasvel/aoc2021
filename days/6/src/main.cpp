#include "util/util.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>

// #define FILENAME "input.txt"
#define FILENAME "sample.txt"

using namespace std;

struct Fish {

	Fish(size_t time = 8) : timer(time) {}

	// return true if new fish should be created
	bool increment() {
		if (timer == 0) {
			timer = 6;
			return true;
		} else {
			timer -= 1;
		}
		return false;
	}

	size_t timer;
};

#define DAYS 20

/*
Start: 8
	create a new fish after 9 days

Start: 3

on the 4th day (start + 1), it creates a new fish with timer 8
Then after another 7 days it creates a new fish with timer 8
2
1
0
6 + create

6
5
4
3
2
1
0
6 = create

So one fish creates (days - start - 1) / 7
Each of which create (days - start - 1) / 7 from the time they are created
For each fish: have a new start day
*/
size_t calculateTotalFish(size_t start, size_t days) {
	if (days >= start + 1) {
		size_t numfish = days - start - 1 / 7;
		size_t total = 2 + numfish;
		for (size_t i = 0; i < numfish; ++i) {
			total += calculateTotalFish()
		}
		return total;
	}

	return 1;
}

int main() {

	vector<Fish> fish;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		auto v = util::splitString(line, ',');
		for (auto f : v) {
			fish.push_back(Fish(stoull(f)));
		}
	}
	file.close();

	cout << "Calculation: " << calculateTotalFish(fish.front().timer, DAYS) << endl;

	for (size_t i = 0; i < DAYS; ++i) {
		size_t s = fish.size();
		for (size_t j = 0; j < s; ++j) {
			if (fish[j].increment()) {
				fish.push_back(Fish(8));
			}
		}
		cout << i << ": " << fish.size() << endl;
	}

	cout << fish.size() << endl;

	return 0;
}
