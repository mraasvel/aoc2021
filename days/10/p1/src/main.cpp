#include "util/util.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <stack>

#define FILENAME "input.txt"
// #define FILENAME "sample.txt"

using namespace std;

typedef pair<char, size_t> pair_t;

vector<pair_t> createMatch() {
	vector<pair_t> m(255);

	m['('] = pair_t(')', 3);
	m['['] = pair_t(']', 57);
	m['{'] = pair_t('}', 1197);
	m['<'] = pair_t('>', 25137);
	return m;
}

unordered_map<char, size_t> createScores() {
	unordered_map<char, size_t> m;

	m[')'] = 3;
	m[']'] = 57;
	m['}'] = 1197;
	m['>'] = 25137;
	return m;
}

unordered_map<char, size_t> scores = createScores();

vector<pair_t> match = createMatch();

size_t scanLine(string line) {

	string starts = "([{<";

	stack<char> find;
	for (char token : line) {
		if (starts.find(token) != std::string::npos) {
			find.push(token);
		} else {
			if (find.empty()) {
				// cerr << "unexpected close: "
				return 0;
			} else {
				if (match[find.top()].first == token) {
					find.pop();
				} else {

					return scores[token];
				}
			}
		}
	}
	return 0;
}

int main() {

	size_t score = 0;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {

		score += scanLine(line);
	}
	file.close();

	cout << score << endl;
	return 0;
}
