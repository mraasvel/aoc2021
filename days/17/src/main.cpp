#include "include.hpp"

using namespace std;

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;


std::pair<int, int> range_x;
std::pair<int, int> range_y;
int max_y = 0;

void executeStep(Point& pos, Point& velocity) {
	pos.x += velocity.x;
	pos.y += velocity.y;
	if (velocity.x > 0) {
		velocity.x -= 1;
	}
	velocity.y -= 1;
}

bool beyondRange(Point pos) {
	return pos.x > range_x.second || pos.y < range_y.first;
}

bool inRange(Point pos) {
	return pos.x >= range_x.first && pos.x <= range_x.second
		&& pos.y >= range_y.first && pos.y <= range_y.second;
}

bool runSimulation(Point velocity) {
	Point pos(0, 0);
	int highest_y = 0;
	while (!beyondRange(pos)) {
		executeStep(pos, velocity);
		highest_y = std::max(highest_y, pos.y);
		if (inRange(pos)) {
			max_y = std::max(max_y, highest_y);
			return true;
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	file >> line >> line >> line;
	range_x = std::make_pair(
		std::stoi(line.substr(line.find('=') + 1)),
		std::stoi(line.substr(line.find("..") + 2))
	);
	file >> line;
	range_y = std::make_pair(
		std::stoi(line.substr(line.find('=') + 1)),
		std::stoi(line.substr(line.find("..") + 2))
	);
	file.close();

	int total = 0;
	for (int y = range_y.first; y < std::abs(range_y.first); ++y) {
		for (int x = 0; x <= range_x.second; ++x) {
			if (runSimulation(Point(x, y))) {
				total += 1;
			}
		}
	}
	std::cout << "P1: " << max_y << std::endl;
	std::cout << "P2: " << total << std::endl;
	return 0;
}
