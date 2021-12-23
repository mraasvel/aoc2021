#include "include.hpp"

using namespace std;

typedef long PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

typedef pair<PointValue, PointValue> Range;

struct Cube {
	Cube()
	: active(true) {}
	Cube(Range x, Range y, Range z)
	: x(x), y(y), z(z), active(true) {}
	Range x, y, z;

	void print() const {
		cout << "Cube: "
			<< x.first << ".." << x.second << ", "
			<< y.first << ".." << y.second << ", "
			<< z.first << ".." << z.second << endl;
	}

	size_t area() const {
		return (x.second - x.first + 1)
			* (y.second - y.first + 1)
			* (z.second - z.first + 1);
	}

	bool active;
};

Range parseRange(const string& line, const string& coord) {
	Range range;
	size_t pos = line.find(coord) + 2;
	size_t dot = line.find("..", pos);
	size_t end = line.find(",", dot);
	range.first = std::stol(line.substr(pos, dot - pos));
	range.second = std::stol(line.substr(dot + 2, end - dot));
	return range;
}

// it intersects if it doesn't not intersect
bool rangeIntersects(const Range& a, const Range& b) {
	return !( (b.first > a.second && b.second > a.second) || (b.first < a.first && b.second < a.first) );
}

bool cubeIntersects(const Cube& a, const Cube& b) {
	return rangeIntersects(a.x, b.x) &&
		rangeIntersects(a.y, b.y) &&
		rangeIntersects(a.z, b.z);
}

Range computeSubRange(const Range& a, const Range& b) {
	return Range(
		std::max(a.first, b.first),
		std::min(a.second, b.second)
	);
}

Cube computeSubCube(const Cube& a, const Cube& b) {
	return Cube(
		computeSubRange(a.x, b.x),
		computeSubRange(a.y, b.y),
		computeSubRange(a.z, b.z)
	);
}

std::vector<Range> splitRange(const Range& a, const Range& tosplit) {
	std::vector<Range> ranges;
	if (a.first != tosplit.first) {
		ranges.push_back(Range(a.first, tosplit.first - 1));
	}
	if (a.second != tosplit.second) {
		ranges.push_back(Range(tosplit.second + 1, a.second));
	}
	return ranges;
}

std::vector<Cube> splitCube(const Cube& a, const Cube& subcube) {
	std::vector<Cube> result;

	std::vector<Range> range_x = splitRange(a.x, subcube.x);
	for (const Range& r : range_x) {
		result.push_back(Cube(
			r, a.y, a.z
		));
	}
	std::vector<Range> range_y = splitRange(a.y, subcube.y);
	for (const Range& r : range_y) {
		result.push_back(Cube(
			subcube.x, r, a.z
		));
	}
	std::vector<Range> range_z = splitRange(a.z, subcube.z);
	for (const Range& r : range_z) {
		result.push_back(Cube(
			subcube.x, subcube.y, r
		));
	}
	return result;
}

void intersectCube(const Cube& a, std::vector<Cube>& cubes) {
	size_t i = 0;
	while (i < cubes.size()) {
		if (cubeIntersects(a, cubes[i])) {
			Cube subcube = computeSubCube(a, cubes[i]);
			std::vector<Cube> new_cubes = splitCube(cubes[i], subcube);
			cubes.erase(cubes.begin() + i);
			for (const Cube& nc : new_cubes) {
				cubes.push_back(nc);
			}
		} else {
			i++;
		}
	}
}

void intersectCubeFast(const Cube& a, std::vector<Cube>& cubes) {
	size_t i = 0;
	while (i < cubes.size()) {
		if (cubes[i].active && cubeIntersects(a, cubes[i])) {
			Cube subcube = computeSubCube(a, cubes[i]);
			std::vector<Cube> new_cubes = splitCube(cubes[i], subcube);
			cubes[i].active = false;
			for (const Cube& nc : new_cubes) {
				cubes.push_back(nc);
			}
		}
		i++;
	}
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	std::vector<Cube> cubes;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		Cube cube;
		cube.x = parseRange(line, "x=");
		cube.y = parseRange(line, "y=");
		cube.z = parseRange(line, "z=");
		intersectCubeFast(cube, cubes);
		if (line.find("on") == 0) {
			cubes.push_back(cube);
		}
	}
	file.close();
	cout << "Total cubes: " << cubes.size() << endl;
	size_t total = 0;
	for (const Cube& cube : cubes) {
		if (cube.active) {
			total += cube.area();
		}
	}
	cout << "Total: " << total << endl;
	return 0;
}
