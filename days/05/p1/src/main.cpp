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

using namespace std;


struct Point {
	Point()
	: x(0), y(0) {}

	Point(int x, int y)
	: x(x) ,y(y) {}

	bool operator<(const Point& b) const {
		return x < b.x || (x == b.x && y < b.y);
	}

	int x, y;
};

ostream& operator<<(ostream & out, const Point& p) {
	out << '(' << p.x << ", " << p.y << ')';
	return out;
}

class Line {
public:
	Line(Point a, Point b)
	: a(a), b(b) {}

	vector<Point> getPoints() const {
		vector<Point> points;
		if (a.x == b.x) {
			for (int y = min(a.y, b.y); y <= max(a.y, b.y); ++y) {
				points.push_back(Point(a.x, y));
			}
		} else {
			for (int x = min(a.x, b.x); x <= max(a.x, b.x); ++x) {
				points.push_back(Point(x, a.y));
			}
		}
		return points;
	}

public:
	Point a;
	Point b;
};

int MAX_X = 0;
int MAX_Y = 0;

int main() {

	vector<Line> lines;
	map<Point, int> m;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		auto v = util::splitString(line, ' ');
		assert(v.size() == 3);
		Point a(stoi(v[0]), stoi(v[0].substr(v[0].find(',') + 1)));
		Point b(stoi(v[2]), stoi(v[2].substr(v[2].find(',') + 1)));

		if (a.x != b.x && a.y != b.y) {
			continue;
		}

		lines.push_back(Line(a, b));

		MAX_X = max(MAX_X, max(a.x, b.x));
		MAX_Y = max(MAX_Y, max(a.y, b.y));
	}
	file.close();

	for (const auto& line : lines) {
		auto points = line.getPoints();
		for (auto p : points) {
			if (m.count(p) == 0) {
				m[p] = 0;
			}
			m[p] += 1;
		}
	}

	int cnt = 0;
	for (auto k : m) {
		if (k.second > 1) {
			++cnt;
		}
	}
	cout << cnt << endl;
	return 0;
}
