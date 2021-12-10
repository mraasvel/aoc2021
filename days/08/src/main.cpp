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

#define FILENAME "input.txt"
// #define FILENAME "sample.txt"
// #define FILENAME "sample2.txt"

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

vector<string> initAppearances() {
	vector<int> a(255);

	a['a'] = 8;
	a['f'] = 8;
	a['c'] = 7;
	a['d'] = 7;
	a['g'] = 7;
	a['b'] = 6;
	a['e'] = 4;

	vector<string> b(10);
	b[9] = "f";
	b[8] = "ac";
	b[7] = "dg";
	b[6] = "b";
	b[4] = "e";
	return b;
}

vector<string> appearances = initAppearances();

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

string intersectStrings(const string& a, const string& b) {
	string result;
	for (auto c : a) {
		if (b.find(c) != std::string::npos) {
			result.push_back(c);
		}
	}
	return result;
}

string addStrings(const string& a, const string& b) {
	string result = a;
	for (auto c : b) {
		if (a.find(c) == std::string::npos) {
			result.push_back(c);
		}
	}
	return result;
}

struct Line {
	// Save all options for each character
	typedef map<char, string> map_type;

	Line() {}

	void addOptions(char c, string possible) {
		// if (options.count(c) == 0) {
		// 	options[c] = possible;
		// 	return;
		// }

		options[c] = intersectStrings(options[c], possible);
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
			result = addStrings(signals[index], result);
		}
		return result;
	}

	int countAppearances(char c) {
		int cnt = 0;
		for (const string& input : inputs) {
			if (input.find(c) != std::string::npos) {
				++cnt;
			}
		}
		return cnt;
	}

	void setAppearances() {
		for (char c = 'a'; c <= 'g'; ++c) {
			int cnt = countAppearances(c);
			options[c] = appearances[cnt];
		}
	}

	void removeCharacter(char c, char skip) {
		for (auto it = options.begin(); it != options.end(); ++it) {
			if (it->first == skip) {
				continue;
			} else if (it->second.find(c) != std::string::npos) {
				options[it->first].erase(it->second.find(c), 1);
			}
		}
	}

	void finalize() {
		for (auto it = options.begin(); it != options.end(); ++it) {
			if (it->second.size() == 1) {
				removeCharacter(it->second[0], it->first);
			}
		}
	}

	size_t solve() {
		setAppearances();
		for (const string& input : inputs) {
			string possible = generatePossible(input.size());
			for (char c : input) {
				addOptions(c, possible);
			}
		}
		finalize();
		convertOutput();
		return stoull(number);
	}

	string createOutput(const string& output) {
		string result;
		for (char c : output) {
			result.push_back(options[c][0]);
		}
		return result;
	}

	string mapStringToNumber(const string& output) {
		string real = createOutput(output);
		size_t num = 0;
		for (num = 0; num < signals.size(); ++num) {
			if (real.size() != signals[num].size()) {
				continue;
			}

			for (size_t i = 0; i < signals[num].size(); ++i) {
				if (real.find(signals[num][i]) == std::string::npos) {
					break;
				}
				if (i == signals[num].size() - 1) {
					return to_string(num);
				}
			}

		}
		return "FAILED";
	}

	void convertOutput() {
		for (const string& output : outputs) {
			number.append(mapStringToNumber(output));
		}
	}

	void printMap() {
		for (auto it = options.begin(); it != options.end(); ++it) {
			cout << it->first << ": " << it->second << endl;
		}
	}

	map_type options;
	string number;
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
		lines.back().outputs.push_back(v[end - 3]);
		lines.back().outputs.push_back(v[end - 2]);
		lines.back().outputs.push_back(v[end - 1]);
		lines.back().outputs.push_back(v[end]);
	}
	file.close();

	size_t sum = 0;
	for (auto& line : lines) {
		sum += line.solve();
	}
	cout << sum << endl;

	return 0;
}
