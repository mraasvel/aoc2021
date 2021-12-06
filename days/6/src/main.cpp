#include "util/util.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <map>

#define FILENAME "input.txt"
// #define FILENAME "sample.txt"

using namespace std;

struct Fish {

	Fish(size_t time = 8) : timer(time), num_fish(1) {}

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
	size_t num_fish;
};

#define DAYS 256

/*
1. Decremting: we want to change the number to the key [key - 1]: however if this key is already taken and not processed yet, it should be processed first before we replace the value
2. 
*/
struct Fishies {
	typedef map<int, size_t> map_type;
	// typedef unordered_map<int, size_t> map_type;
	Fishies() : total(0) {}

	void add(const Fish& fish) {
		auto it = m.find(fish.timer);
		if (it == m.end()) {
			m[fish.timer] = 1;
		} else {
			m[fish.timer] += 1;
		}
		total += 1;
	}

	void print() {
		for (auto f : m) {
			cout << f.first << " (" << f.second << ") ";
		}
		cout << "Total: " << total << endl;
	}

	void execDay() {
		size_t to_add = 0;
		for (auto& it  : m) {
			size_t num = it.second;
			if (it.first == 0) {
				to_add += num;
				addNew(6, num);
			} else {
				addNew(it.first - 1, num);
			}
		}
		addFishies(to_add);
		m.clear();
		new_map.swap(m);
	}

	void addNew(int time, size_t n) {
		auto it = new_map.find(time);
		if (it == new_map.end()) {
			new_map[time] = n;
		} else {
			new_map[time] += n;
		}
	}

	void addFishies(size_t n) {
		if (n == 0) {
			return;
		}
		addNew(8, n);
		total += n;
	}

	size_t count() const {
		// size_t total = 0;
		// for (auto& f : m) {
		// 	total += f.second;
		// }
		return total;
	}

	// map timer to number of fish;
	map_type m;
	map_type new_map;
	size_t total;
};

int main() {

	vector<Fish> fish;
	Fishies fishies;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		auto v = util::splitString(line, ',');
		for (auto val : v) {
			Fish fishy = Fish(stoull(val));
			fishies.add(fishy);
		}
	}
	file.close();

	for (size_t i = 0; i < DAYS; ++i) {
		// fishies.print();
		fishies.execDay();
		// cout << i+ 1 << ": " << fishies.count() << endl;
	}
	cout << fishies.count() << endl;
	return 0;
}
