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
#include <set>

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

	bool operator==(const Point& b) const {
		return x == b.x && y == b.y;
	}

	bool operator!=(const Point& b) const {
		return !(*this == b);
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
		vector<Point> v;

		if (a.x == b.x) {
			// horizontal
			for (int y = min(a.y, b.y); y <= max(a.y, b.y); ++y) {
				v.push_back(Point(a.x, y));
			}
		} else if (a.y == b.y) {
			// vertical
			for (int x = min(a.x, b.x); x <= max(a.x, b.x); ++x) {
				v.push_back(Point(x, a.y));
			}
		} else {
			// diagonal lines
			Point p = a;
			while (p != b) {
				int x = a.x < b.x ? 1 : -1;
				int y = a.y < b.y ? 1 : -1;
				v.push_back(p);
				p.x += x;
				p.y += y;
			}
			v.push_back(b);
		}
		return v;
	}
public:
	Point a;
	Point b;
};

int MAX_X = 0;
int MAX_Y = 0;

int main() {

	vector<Line> lines;
	map<Point, int> counts;
	set<Point> counted;

	std::ifstream file(FILENAME);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		auto v = util::splitString(line, ' ');
		assert(v.size() == 3);
		Point a(stoi(v[0]), stoi(v[0].substr(v[0].find(',') + 1)));
		Point b(stoi(v[2]), stoi(v[2].substr(v[2].find(',') + 1)));
		lines.push_back(Line(a, b));
	}
	file.close();

	int total = 0;
	for (const Line& line: lines) {
		// just generate each point on every linen and put it into a map of points to counts
		auto v = line.getPoints();
		for (auto point : v) {
			if (counted.count(point) > 0) {
				continue;
			}

			if (counts.count(point) == 0) {
				counts[point] = 0;
			}

			counts[point] += 1;
			if (counts[point] > 1) {
				++total;
				counted.insert(point);
			}
		}
	}
	cout << total << endl;
	return 0;
}
