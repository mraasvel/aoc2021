#include "include.hpp"

using namespace std;

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

struct Probe {
	Probe(Point v)
	: pos(0, 0), velocity(v), MAX_Y(0) {}

	void decr(int& val) {
		if (val < 0) {
			val += 1;
		} else if (val > 0) {
			val -= 1;
		}
	}

	void step() {
		pos = pos + velocity;
		if (pos.y > MAX_Y) {
			MAX_Y = pos.y;
		}
		decr(velocity.x);
		velocity.y -= 1;
		// cout << "step: " << pos << ": " << velocity << endl;
	}

	bool inTarget(Point targetx, Point targety) {
		return inTargetX(targetx) && inTargetY(targety);
		return pos.x >= targetx.x && pos.x <= targetx.y
			&& pos.y >= targety.x && pos.y <= targety.y;
	}

	bool inTargetX(Point targetx) {
		return pos.x >= targetx.x && pos.x <= targetx.y;
	}

	bool inTargetY(Point targety) {
		return pos.y >= targety.x && pos.y <= targety.y;
	}

	bool beyondTarget(Point targetx, Point targety) {
		if (inTarget(targetx, targety)) {
			return false;
		}
		if (velocity.x == 0 && !inTargetX(targetx)) {
			// cout << "X is zero and not in T" << endl;
			return true;
		} else if (velocity.x > 0 && pos.x > targetx.y) {
			// cout << "X > 0 and beyond MAX_X" << endl;
			return true;
		} else if (velocity.x < 0 && pos.x < targetx.x) {
			// cout << "X < 0 and beyond MIN_X" << endl;
			return true;
		} else if (velocity.y < 0 && pos.y < targety.x) {
			// cout << "Y is < 0 and beyond MIN_Y" << endl;
			return true;
		}
		return false;
	}

	Point pos;
	Point velocity;
	PointValue MAX_Y;
};

struct Target {
	Point targetx;
	Point targety;
};

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	file >> line >> line >> line;
	Target target;
	target.targetx = Point(
		std::stoi(line.substr(line.find('=') + 1)),
		std::stoi(line.substr(line.find("..") + 2))
	);
	file >> line;
	target.targety = Point(
		std::stoi(line.substr(line.find('=') + 1)),
		std::stoi(line.substr(line.find("..") + 2))
	);
	file.close();

	Point max_height(0, std::numeric_limits<PointValue>::min());
	int MAX_Y = 0;
	int TOTAL = 0;

	for (int y = -1000; y < 1000; ++y) {
		for (int x = -1000; x < 1000; ++x) {
			Probe probe(Point(x, y));
			bool TOTAL_SET = false;
			while (!probe.beyondTarget(target.targetx, target.targety)) {
				probe.step();
				if (probe.inTarget(target.targetx, target.targety)) {
					// cout << Point(x, y) << probe.pos << endl;
					if (MAX_Y < probe.MAX_Y) {
						MAX_Y = probe.MAX_Y;
						max_height = probe.pos;
						cout << Point(x, y) << ": " << MAX_Y << endl;
					}
					if (!TOTAL_SET) {
						++TOTAL;
						TOTAL_SET = true;
					}
						break;
				}
			}
		}
	}
	cout << TOTAL << endl;
	return 0;
}
