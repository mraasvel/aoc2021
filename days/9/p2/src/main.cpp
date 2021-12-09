#include "util/util.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <queue>
#include <map>

#define FILENAME "input.txt"
// #define FILENAME "sample.txt"

struct Point {
	Point(int x = 0, int y = 0) : x(x), y(y) {}

	int x, y;
};


using namespace std;

struct Data {
	Data(int value) : value(value), explored(false) {}

	operator int() {
		return value;
	}

	int value;
	bool explored;
};

vector<size_t> basin_sizes;

struct Map {

	bool isValidPoint(Point a) const {
		return a.x >= 0 && a.x < columns
			&& a.y >= 0 && a.y < rows;
	}

	vector<Point> getAdjacents(Point p) const {
		const vector<Point> adj = {
			Point(p.x, p.y - 1),
			Point(p.x, p.y + 1),
			Point(p.x - 1, p.y),
			Point(p.x + 1, p.y),
		};

		vector<Point> result;
		for (auto a : adj) {
			if (isValidPoint(a)) {
				result.push_back(a);
			}
		}
		return result;
	}

	bool shouldExplore(Point p) const {
		return get(p) != 9 && get(p).explored == false;
	}

	void setBasin(Point p) {
		size_t basin_size = 0;
		std::queue<Point> q;
		q.push(p);
		getRef(p).explored = true;
		while (!q.empty()) {
			Point x = q.front();
			basin_size += 1;
			q.pop();
			vector<Point> adj = getAdjacents(x);
			for (Point a : adj) {
				if (shouldExplore(a)) {
					q.push(a);
					getRef(a).explored = true;
				}
			}
		}
		basin_sizes.push_back(basin_size);
	}

	void setBasinSizes() {
		for (int y = 0; y < rows; ++y) {
			for (int x = 0; x < columns; ++x) {
				if (shouldExplore(Point(x, y))) {
					setBasin(Point(x, y));
				}
			}
		}
	}

	bool isLowPoint(Point p) const {
		vector<Point> v = getAdjacents(p);

		for (auto a : v) {
			if (get(p) >= get(a)) {
				return false;
			}
		}
		return true;
	}

	Data get(Point a) const {
		return m[a.y][a.x];
	}

	Data& getRef(Point a) {
		return m[a.y][a.x];
	}

	int columns;
	int rows;
	vector<vector<Data> > m;
};

int main() {

	Map m;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	size_t y = 0;
	while (std::getline(file, line)) {
		m.m.push_back(vector<Data>());
		m.columns = line.size();
		for (auto c : line) {
			m.m[y].push_back(Data(c - '0'));
		}
		++y;
	}
	file.close();
	m.rows = m.m.size();

	m.setBasinSizes();

	size_t result[3];
	for (int i = 0; i < 3; ++i) {
		auto it = max_element(basin_sizes.begin(), basin_sizes.end());
		result[i] = *it;
		basin_sizes.erase(it);
	}

	cout << result[0] << ", " << result[1] << ", " << result[2] << endl;
	cout << result[0] * result[1] * result[2] << endl;

	return 0;
}
