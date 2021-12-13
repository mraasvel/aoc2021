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

#define FILENAME "input.txt"
// #define FILENAME "sample.txt"

using namespace std;

struct Point {
	Point(int x = 0, int y = 0)
	: x(x), y(y) {}

	bool operator<(const Point& rhs) const {
		return x < rhs.x || (x == rhs.x && y < rhs.y);
	}

	int x, y;
};

ostream& operator<<(ostream& out, Point p) {
	out << "(" << p.x << ", " << p.y << ")";
	return out;
}

int calculateNewCoord(int center, int pos) {
	return center + (center - pos);
}

set<Point> fold(Point p, const set<Point>& points) {
	set<Point> result;

	for (const auto& point: points) {
		if (p.x != 0) {
			// fold along vertical line
			if (p.x < point.x) {
				Point new_point(point);
				new_point.x = calculateNewCoord(p.x, point.x);
				if (new_point.x >= 0) {
					result.insert(new_point);
				}
			} else {
				result.insert(point);
			}
		} else if (p.y != 0) {
			// fold along horizontal line
			if (p.y < point.y) {
				Point new_point(point);
				new_point.y = calculateNewCoord(p.y, point.y);
				if (new_point.y >= 0) {
					result.insert(new_point);
				}
			} else {
				result.insert(point);
			}
		} else {
			throw std::runtime_error("invalid fold");
		}
	}
	return result;
}

int MAX_X = 0;
int MAX_Y = 0;

int main() {

	std::set<Point> points;
	std::vector<Point> folds;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		if (line.front() == 'f') {
			Point p;
			size_t index = line.find('=');
			if (line[index - 1] == 'x') {
				p.x = std::stoi(line.substr(index + 1));
			} else if (line[index - 1] == 'y') {
				p.y = std::stoi(line.substr(index + 1));
			} else {
				throw std::runtime_error("no char found: " + line);
			}
			folds.push_back(p);
		} else {
			Point p;
			p.x = std::stoi(line);
			p.y = std::stoi(line.substr(line.find(',') + 1));
			points.insert(p);
		}
	}
	file.close();


	for (auto f : folds) {
		points = fold(f, points);
		cout << points.size() << endl;
	}
	cout << points.size() << endl;


	for (auto point : points) {
		MAX_X = std::max(point.x, MAX_X);
		MAX_Y = std::max(point.y, MAX_Y);
	}

	for (int y = 0; y <= MAX_Y; ++y) {
		for (int x = 0; x <= MAX_X; ++x) {
			if (points.count(Point(x, y)) > 0) {
				cout << "#";
			} else {
				cout << '.';
			}
		}
		cout << endl;
	}
	return 0;
}
