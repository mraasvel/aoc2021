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

	m[')'] = 1;
	m[']'] = 2;
	m['}'] = 3;
	m['>'] = 4;
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
					return 0;
					return scores[token];
				}
			}
		}
	}

	size_t score = 0;
	while (!find.empty()) {
		char token = match[find.top()].first;
		score = (score * 5) + scores[token];
		find.pop();
	}
	return score;
}

int main() {

	size_t score = 0;

	vector<size_t> results;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {

		score = scanLine(line);
		if (score != 0) {
			results.push_back(score);
		}
	}
	file.close();

	cout << results.size() << endl;
	sort(results.begin(), results.end());
	cout << results[results.size() / 2] << endl;
	return 0;
}
