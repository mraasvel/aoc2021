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
#include <sstream>

#define FILENAME "input.txt"
// #define FILENAME "sample.txt"

using namespace std;

#define DAYS 256

std::vector<size_t> nextCycle(const std::vector<size_t>& fish) {
	std::vector<size_t> next(10, 0);
	for (std::size_t i = 0; i < fish.size(); i++) {
		if (i == 0) {
			next[6] += fish[i];
			next[8] += fish[i];
		} else {
			next[i - 1] += fish[i];
		}
	}
	return next;
}

void printCycle(const std::vector<size_t>& fish) {
	std::cout << "Cycle: " << std::endl;
	for (size_t count : fish) {
		std::cout << count << ' ';
	}
	std::cout << std::endl;
}

void printTotalFish(const std::vector<size_t>& fish) {
	size_t total = 0;
	for (size_t count : fish) {
		total += count;
	}
	std::cout << total << endl;
}

int main(int argc, char *argv[]) {
	std::vector<size_t> fish(10, 0);
	assert(argc != 1);
	std::ifstream ifs(argv[1]);
	assert(ifs.is_open());
	while (!ifs.eof()) {
		if (!isdigit(ifs.peek())) {
			break;
		}
		int n = ifs.get() - '0';
		fish[n] += 1;
		ifs.ignore(1);
	}
	for (size_t i = 0; i < DAYS; i++) {
		fish = nextCycle(fish);
	}
	printTotalFish(fish);
	return 0;
}
