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

struct Board {
	typedef int value_type;

	value_type m[5][5];
	bool marked[5][5];

	bool iscomplete;

	Board()
	: iscomplete(false) {
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				marked[i][j] = false;
			}
		}
	}
	
	void markComplete() {
		iscomplete = true;
	}

	void mark(int input) {
		forEach([this, &input] (int i, int j) -> void {
			if (m[i][j] == input) {
				marked[i][j] = true;
			}
		});
	}

	int calculate() {
		int sum = 0;
		forEach([this, &sum] (int i, int j) -> void {
			if (marked[i][j] == false) {
				sum += m[i][j];
			}
		});
		return sum;
	}

	bool bingo() {
		if (iscomplete) {
			return false;
		}
		for (int i = 0; i < 5; ++i) {
			int c1 = 0;
			int c2 = 0;
			for (int j = 0; j < 5; ++j) {
				if (marked[i][j] == true) {
					++c1;
				}
				if (marked[j][i] == true) {
					++c2;
				}
			}
			if (c1 == 5 || c2 == 5) {
				return true;
			}
		}
		return false;
	}

	template <typename Functor>
	void forEach(Functor f) {
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				f(i, j);
			}
		}
	}

	void print() const {
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				cout << m[i][j] << ' ';
			}
			cout << endl;
		}
	}
};

vector<Board> boards;
vector<int> inputs;

void run() {
	Board* last_board = NULL;
	int last_input = 0;
	for (auto input : inputs) {
		for (auto& board : boards) {
			if (board.iscomplete) {
				continue;
			}
			board.mark(input);
			if (board.bingo()) {
				last_board = &board;
				last_input = input;
				board.markComplete();
				// break;
			}
		}
	}

	assert (last_board != NULL);
	cout << last_input << endl;
	cout << last_board->calculate() << endl;
	cout << last_input * last_board->calculate() << endl;

}

int main() {

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;

	getline(file, line);
	auto v = util::splitString(line, ',');
	for_each(v.begin(), v.end(), [] (string const & x) -> void {
		inputs.push_back(stoi(x));
	});
	getline(file, line);

	Board new_board;
	size_t i = 0;
	while (std::getline(file, line)) {
		if (line.size() == 0) {			
			boards.push_back(new_board);
			i = 0;
			continue;
		}

		auto l = util::splitString(line, ' ');

		size_t j = 0;
		for (auto x : l) {
			new_board.m[i][j] = stoi(x);
			++j;
		}
		++i;
	}
	boards.push_back(new_board);
	file.close();

	run();

	return 0;
}
