#include "include.hpp"

using namespace std;

typedef long PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

PointSet cubes;
typedef pair<PointValue, PointValue> Range;

struct Cuboid {
	Range x, y, z;

	void print() const {
		cout << "Cube: "
			<< x.first << ".." << x.second << ", "
			<< y.first << ".." << y.second << ", "
			<< z.first << ".." << z.second << endl;
	}
};

Range getRange(const string& line, const string& coord) {
	Range range;
	
	size_t pos = line.find(coord) + 2;
	size_t dot = line.find("..", pos);
	size_t end = line.find(",", dot);
	range.first = std::stol(line.substr(pos, dot - pos));
	range.second = std::stol(line.substr(dot + 2, end - dot));
	return range;
}

bool inRegion(const Cuboid& cuboid) {
	return !(cuboid.x.second < -50 || cuboid.x.first > 50
	|| cuboid.y.second < -50 || cuboid.y.first > 50
	|| cuboid.z.second < -50 || cuboid.z.first > 50);
}

bool inRegion(const Point& p) {
	return p.x >= -50 && p.x <= 50 && p.y >= -50 && p.y <= 50 && p.z >= -50 && p.z <= 50;
}

void fillSet(const Cuboid& cuboid, bool on) {
	for (auto x = cuboid.x.first; x <= cuboid.x.second; x++) {
		for (auto y = cuboid.y.first; y <= cuboid.y.second; y++) {
			for (auto z = cuboid.z.first; z <= cuboid.z.second; z++) {
				if (!inRegion(Point(x, y, z))) {
					continue;
				}
				if (on)  {
					cubes.insert(Point(x, y, z));
				} else if (cubes.count(Point(x, y, z)) > 0) {
					cubes.erase(Point(x, y, z));
				}
			}
		}
	}
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		Cuboid cuboid;
		cuboid.x = getRange(line, "x=");
		cuboid.y = getRange(line, "y=");
		cuboid.z = getRange(line, "z=");
		if (!inRegion(cuboid)) {
			continue;
		}
		cuboid.print();
		if (line.find("on") == 0) {
			fillSet(cuboid, true);
		} else {
			fillSet(cuboid, false);
		}
	}
	file.close();
	cout << cubes.size() << endl;
	return 0;
}
