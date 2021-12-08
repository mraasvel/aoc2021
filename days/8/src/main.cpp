#include "util/util.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>

// #define FILENAME "input.txt"
// #define FILENAME "sample.txt"
#define FILENAME "sample2.txt"

using namespace std;

vector<int> numSegments = {
	6, 2, 5, 5, 4, 5, 6, 3, 7, 6
};

/*
1: ab -> cf
4: eafb -> bcdf
7: dab -> acf
8: acedgfb -> abcdefg

a : cf
b : cf
e : bd
f : bd
d : a
*/

vector<string> signals = {
	"abcefg",
	"cf",
	"acdeg",
	"acdfg",
	"bcdf",
	"abdfg",
	"abdefg",
	"acf",
	"abcdefg",
	"abcdfg",
};

struct Line {
	// Save all options for each character
	typedef map<char, string> map_type;

	Line() {
		for (int c = 'a'; c <= 'g'; ++c) {
			options[c] = "";
		}
	}

	void addOptions(char c, string possible) {
		if (options[c].empty()) {
			for (auto x : possible) {
				if (options[c].find(x) == std::string::npos) {
					options[c].push_back(x);
				}
			}
			return;
		}

		for (size_t i = 0; i < options[c].size(); ++i) {
			char x = options[c][i];
			size_t pos = possible.find(x);
			if (pos == std::string::npos) {
				options[c].erase(i, 1);
			}
		}
	}

	string generatePossible(int len) {
		vector<size_t> indexes;
		for (size_t i = 0; i < numSegments.size(); ++i) {
			if (len == numSegments[i]) {
				indexes.push_back(i);
			}
		}

		string result;
		for (auto index : indexes) {
			for (auto c : signals[index]) {
				if (result.find(c) == std::string::npos) {
					result.push_back(c);
				}
			}
		}
		return result;
	}

	void solve() {
		for (const auto& input : inputs) {
			string possible = generatePossible(input.size());
			for (auto c : input) {
				addOptions(c, possible);
			}
		}
		printMap();
	}

	void printMap() {
		for (auto it = options.begin(); it != options.end(); ++it) {
			cout << it->first << ": " << it->second << endl;
		}
	}

	map_type options;
	vector<string> inputs;
	vector<string> outputs;
};

int main() {
	vector<Line> lines;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(Line());
		auto v = util::splitString(line, ' ');
		size_t end = v.size() - 1;

		for (size_t i = 0; i < end - 4; ++i) {
			lines.back().inputs.push_back(v[i]);
		}
		lines.back().outputs.push_back(v[end]);
		lines.back().outputs.push_back(v[end - 1]);
		lines.back().outputs.push_back(v[end - 2]);
		lines.back().outputs.push_back(v[end - 3]);
		break;
	}
	file.close();

	lines[0].solve();

	return 0;
}
