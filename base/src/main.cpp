#include "include.hpp"

using namespace std;

typedef util::Point<int> Point;
typedef util::HashPoint<int> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHash;

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
	}
	file.close();
	return 0;
}
