#include "include.hpp"
#include "Scanner.hpp"
#include <sstream>
#include <queue>

using namespace std;

typedef util::HashPoint<int> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;

static void pointRotations(const Point& p, vector<Point>& v, int index) {
// Rotations around the x-axis
	v[index] = Point(p.x, p.y, p.z);
	v[index + 1] = Point(p.x, p.z, -p.y);
	v[index + 2] = Point(p.x, -p.y, -p.z);
	v[index + 3] = Point(p.x, -p.z, p.y);

// Other direction
	v[index + 4] = Point(-p.x, -p.y, p.z);
	v[index + 5] = Point(-p.x, -p.z, -p.y);
	v[index + 6] = Point(-p.x, p.y, -p.z);
	v[index + 7] = Point(-p.x, p.z, p.y);
}

static vector<Point> pointPermutations(const Point& p) {
	vector<Point> v;
	v.resize(24);
	pointRotations(p, v, 0);
	pointRotations(Point(p.y, -p.x, p.z), v, 8);
	pointRotations(Point(p.z, p.y, -p.x), v, 16);
	return v;
}

void Scanner::generateOrientations() {
	assert(orientations.size() == 0);
	orientations.resize(24);
	for (const Point& p : beacons) {
		vector<Point> v = pointPermutations(p);
		for (int i = 0; i < 24; ++i) {
			orientations[i].push_back(v[i]);
		}
	}
}

std::vector<std::vector<Point>> Scanner::getOrientations() const {
	return orientations;
}

Point translatePoint(const Point& p, size_t orientation) {
	return pointPermutations(p)[orientation];
}

vector<Point> translatePoints(const vector<Point>& points, size_t orientation, Point delta) {
	vector<Point> translated;
	for (const Point& p : points) {
		translated.push_back(translatePoint(p, orientation) + delta);
	}
	return translated;
}

/*
First is the initial translation
Then we go to the next scanner
*/
std::vector<Point> Scanner::getTranslations(const vector<Scanner>& scanners) const {	
	vector<Point> translated;
	for (const Point& p : orientations[orientation]) {
		translated.push_back(p + delta);
	}
	size_t index = next;
	while (index != 0) {
		const Scanner& scanner = scanners[index];
		translated = translatePoints(translated, scanner.orientation, scanner.delta);
		index = scanner.next;
	}
	return translated;
}


bool intersectPoints(const vector<Point>& a, const vector<Point>& b, Point& delta) {
	PointSet s;
	for (const Point& p : a) {
		s.insert(p);
	}

	for (const Point& ap : a) {
		for (const Point& bp : b) {
			int cnt = 0;
			delta = ap - bp;
			for (const Point& bp2 : b) {
				if (s.count(bp2 + delta) > 0) {
					cnt++;
				}
			}
			if (cnt == 12) {
				return true;
			}
		}
	}
	return false;
}

/*
1. Get all orientations of b
2. Compare each orientation with a's base orientation
3. Return true if 12 match
*/
// Sets b's position relative to scanner[0]
bool intersectScanners(const Scanner& a, Scanner& b) {
	vector<vector<Point>> orientations = b.getOrientations();
	for (size_t i = 0; i < orientations.size(); i++) {
		Point delta;
		if (intersectPoints(a.beacons, orientations[i], delta)) {
			b.delta = delta;
			b.orientation = i;
			return true;
		}
	}
	return false;
}

// return all indices that intersected with index
vector<size_t> checkIntersections(size_t index, vector<Scanner>& scanners) {
	vector<size_t> result;

	for (size_t i = 0; i < scanners.size(); i++) {
		if (scanners[i].explored) {
			continue;
		}
		if (intersectScanners(scanners[index], scanners[i])) {
			result.push_back(i);
		}
	}
	return result;
}

void exploreIntersections(vector<Scanner>& scanners) {
	queue<size_t> q;
	q.push(0);
	scanners[0].explored = true;
	while (!q.empty()) {
		size_t index = q.front();
		q.pop();
		vector<size_t> intersections = checkIntersections(index, scanners);
		for (size_t i : intersections) {
			q.push(i);
			scanners[i].explored = true;
			scanners[i].next = index;
			printf("Scanners[%lu] intersected with Scanners[%lu]\n", index, i);
		}
	}
}

Point translateDelta(const Scanner& scanner, const vector<Scanner>& scanners) {
	Point delta = scanner.delta;
	size_t index = scanner.next;
	while (index != 0) {
		delta = scanners[index].delta + translatePoint(delta, scanners[index].orientation);
		index = scanners[index].next;
	}
	return delta;
}

vector<Point> calculateDeltas(vector<Scanner>& scanners) {
	vector<Point> deltas;
	for (Scanner& scanner : scanners) {
		deltas.push_back(translateDelta(scanner, scanners));
	}
	return deltas;
}
size_t manhattanDistance(const Point& a, const Point& b) {
	return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

int main(int argc, char *argv[]) {
	vector<Scanner> scanners;
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		if (line.find("scanner") != string::npos) {
			scanners.resize(scanners.size() + 1);
		} else {
			stringstream ss(line);
			Point p;
			char c;
			ss >> p.x >> c >> p.y >> c >> p.z;
			scanners.back().beacons.push_back(p);
		}
	}
	file.close();

	for (auto& scanner : scanners) {
		scanner.generateOrientations();
	}
	exploreIntersections(scanners);
	PointSet beacons;
	int i = 0;
	for (auto& scanner : scanners) {
		const vector<Point> translated = scanner.getTranslations(scanners);
		for (const Point& p : translated) {
			beacons.insert(p);
		}
	}
	cout << beacons.size() << endl;
	auto deltas = calculateDeltas(scanners);
	size_t distance = 0;
	for (const Point& a : deltas) {
		for (const Point& b : deltas) {
			distance = max(distance, manhattanDistance(a, b));
		}
	}
	cout << distance << endl;
	return 0;
}
