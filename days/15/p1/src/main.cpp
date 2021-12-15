#include "include.hpp"

using namespace std;

typedef util::Point<size_t> Point;
typedef util::HashPoint<int> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHash;

struct Map {

	Map() : width(0), height(0) {}

	size_t Index(Point p) const {
		return p.y * width + p.x;
	}
	
	bool isEnd(Point p) const {
		return p.x == width - 1 && p.y == height - 1;
	}

	int get(Point p) const {
		return m[Index(p)];
	}

	size_t getScore(Point p) const {
		return min_score[Index(p)];
	}

	bool isValid(Point p) const {
		return p.x < width && p.y < height;
	}

	void reconstruct() {
		
	}

	size_t width;
	size_t height;
	vector<int> m;
	vector<size_t> min_score;
};

vector<Point> getAdjacent(const Point& p) {
	const vector<Point> adj = {
		Point(p.x + 1, p.y),
		Point(p.x, p.y + 1),
	};

	return adj;
}

size_t risk = std::numeric_limits<std::size_t>::max();

void calculateRisk(const Point& pos, Map& risks, size_t current) {
	if (risks.isEnd(pos)) {
		risk = std::min(current + risks.get(pos), risk);
		return;
	} if (!risks.isValid(pos)) {
		return;
	}
	std::size_t value = risks.get(pos) + current;
	if (value >= risks.getScore(pos)) {
		return;
	}
	risks.min_score[risks.Index(pos)] = value;
	// auto v = getAdjacent(pos);
	Point p1(pos.x + 1, pos.y);
	Point p2(pos.x, pos.y + 1);
	calculateRisk(p1, risks, risks.get(pos) + current);
	calculateRisk(p2, risks, risks.get(pos) + current);
}


int main(int argc, char *argv[]) {
	Map risks;
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	while (std::getline(file, line)) {
		risks.width = line.size();
		for (auto c : line) {
			risks.m.push_back((int)(c - '0'));
			risks.min_score.push_back(std::numeric_limits<size_t>::max());
		}
		risks.height += 1;
	}
	file.close();
	calculateRisk(Point(0, 0), risks, 0);
	cout << risk - risks.m.front() << endl;

	return 0;
}
