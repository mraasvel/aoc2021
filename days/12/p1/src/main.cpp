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

class Cave {
public:
	Cave()
	: small(false), visited(false) {}

	bool isSmall() const {
		return small;
	}

	bool small;
	bool visited;
	vector<string> connected;
};

bool islowercase(const string& s) {
	return islower(s[0]);
}

unordered_map<string, Cave> graph;
size_t paths = 0;

void printGraph() {
	for (auto it : graph) {
		cout << it.first << ": ";
		for (auto c : it.second.connected) {
			cout << c << ", ";
		}
		cout << endl;
	}
}

bool shouldVisit(const string& c) {
	return !graph[c].small || !graph[c].visited;
}

size_t visitPaths(const std::string& key) {
	if (key == "end") {
		paths += 1;
		return 1;
	}
	size_t result = 0;
	graph[key].visited = true;
	for (auto c : graph[key].connected) {
		if (shouldVisit(c)) {
			result += visitPaths(c);
		}
	}
	graph[key].visited = false;
	return result;
}

int main() {

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		auto v = util::splitString(line, '-');
		graph[v[0]].connected.push_back(v[1]);
		graph[v[1]].connected.push_back(v[0]);
	}
	file.close();

	for (auto it : graph) {
		if (islowercase(it.first)) {
			graph[it.first].small = true;
		}
	}

	printGraph();
	string start = "start";
	cout << visitPaths(start) << endl;
	cout << paths << endl;
	return 0;
}
