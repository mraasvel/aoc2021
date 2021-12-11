#include "util/util.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#define FILENAME "input.txt"
// #define FILENAME "sample.txt"

using namespace std;

struct Point {
	Point(int x = 0, int y = 0) : x(x), y(y) {}
	int x, y;
};

struct Octopus {
	Octopus()
	: has_flashed(false) {}

	bool has_flashed;
};

class Map {
public:
	void set(Point p, int n) {
		get(p) = n;
	}

	int& get(Point p) {
		return energy[getIndex(p)];
	}

	int get(Point p) const {
		return energy[getIndex(p)];
	}

	Octopus& getOcto(Point p) {
		return octos[getIndex(p)];
	}

	const Octopus& getOcto(Point p) const {
		return octos[getIndex(p)];
	}

	size_t getIndex(Point p) const {
		return p.y * WIDTH + p.x;
	}

	void print() {
		forEachPoint([] (Point p, int v) -> void {
			cout << v;
			if (p.x == WIDTH - 1) {
				cout << endl;
			}
		});
	}

	template <typename Functor>
	void forEach(Functor f) {
		for (int y = 0; y < HEIGHT; ++y) {
			for (int x = 0; x < WIDTH; ++x) {
				f(get(Point(x, y)));
			}
		}
	}

	template <typename Functor>
	void forEachPoint(Functor f) {
		for (int y = 0; y < HEIGHT; ++y) {
			for (int x = 0; x < WIDTH; ++x) {
				f(Point(x, y), get(Point(x, y)));
			}
		}
	}

	bool validPoint(Point p) {
		return p.x >= 0 && p.x < WIDTH && p.y >= 0 && p.y < HEIGHT;
	}

	vector<Point> getAdjacent(Point p) {
		const vector<Point> adj = {
			Point(p.x, p.y + 1),
			Point(p.x, p.y - 1),
			Point(p.x + 1, p.y),
			Point(p.x + 1, p.y + 1),
			Point(p.x + 1, p.y - 1),
			Point(p.x - 1, p.y),
			Point(p.x - 1, p.y + 1),
			Point(p.x - 1, p.y - 1),
		};

		vector<Point> result;
		for (auto a : adj) {
			if (validPoint(a)) {
				result.push_back(a);
			}
		}
		return result;
	}

	void incrementEnergy() {
		forEach([] (int& n) {
			n += 1;
		});
	}

	void flashPoint(Point p) {
		this_round += 1;
		getOcto(p).has_flashed = true;
		vector<Point> adj = getAdjacent(p);
		for (auto a : adj) {
			if (getOcto(a).has_flashed) {
				continue;
			}
			get(a) += 1;
			if (get(a) > 9) {
				flashPoint(a);
			}
		}
	}

	void doFlashes() {
		forEachPoint([this] (Point p, int n) {
			if (n > 9 && !this->getOcto(p).has_flashed) {
				this->flashPoint(p);
			}
		});
	}

	void simulateStep() {
		this_round = 0;
		incrementEnergy();
		doFlashes();

		// Set energy to 0
		forEachPoint([this] (Point p, int n) {
			if (this->getOcto(p).has_flashed) {
				this->get(p) = 0;
				this->getOcto(p).has_flashed = false;
			}
		});
		total += this_round;
	}

public:
	size_t this_round = 0;
	size_t total = 0;
private:
	static constexpr int HEIGHT = 10;
	static constexpr int WIDTH = 10;

	Octopus octos[HEIGHT * WIDTH];
	int energy[HEIGHT * WIDTH];
};


int main() {

	Map octo;
	int x, y;
	x = y= 0;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		x = 0;
		for (auto c : line) {
			octo.set(Point(x, y), c - '0');
			++x;
		}
		++y;
	}
	file.close();

	for (int i = 0; true; ++i) {
		octo.simulateStep();
		if (octo.this_round == 100) {
			cout << (i + 1) << endl;
			break;
		}
	}
	// octo.print();
	// cout << octo.total << std::endl;
	return 0;
}
