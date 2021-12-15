#include "include.hpp"
#include <queue>

using namespace std;

typedef util::Point<int> Point;
typedef util::HashPoint<int> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
typedef std::unordered_map<Point, size_t, HashPoint> PointHash;

vector<Point> getAdjacent(const Point& p) {
	const vector<Point> adj = {
		Point(p.x + 1, p.y),
		Point(p.x - 1, p.y),
		Point(p.x, p.y + 1),
		Point(p.x, p.y - 1)
	};

	return adj;
}

struct Data {
	Data(Point p, size_t score = std::numeric_limits<size_t>::max())
	: p(p), score(score) {}

	bool operator<(const Data& rhs) const {
		return score > rhs.score;
	}

	Point p;
	size_t score;
};

bool isValidPoint(Point p, Point end) {
	return p.x < end.x && p.y < end.y && p.x >= 0 && p.y >= 0;
}

bool isEnd(Point p, Point end) {
	return p.x == end.x - 1 && p.y == end.y - 1;
}

#define ORIG_HEIGHT 100
#define EXPANSIONS 50

size_t HEIGHT;

size_t calcIndex(Point p) {
	return p.y * HEIGHT + p.x;
}

int calcValue(int v) {
	v = v % 9;
	return v == 0 ? 9 : v;
}

void insertPositions(vector<int>& grid, vector<int>& min_scores, Point pos, int val, size_t w) {
	for (int y = 0; y < EXPANSIONS; ++y) {
		for (int x = 0; x < EXPANSIONS; ++x) {
			Point p(pos.x + x * w, pos.y + y * w);
			size_t index = calcIndex(Point(pos.x + x * w, pos.y + y * w));
			grid[index] = calcValue(val + x + y);
			min_scores[index] = std::numeric_limits<int>::max();
		}
	}
}

int dijkstra(const vector<int>& grid, vector<int>& min_scores, Point end) {
	std::priority_queue<Data> pq;
	pq.push(Data(Point(0, 0), 0));
	while (!pq.empty()) {
		auto val = pq.top();
		pq.pop();
		int score = val.score;
		if (min_scores[calcIndex(val.p)] < score) {
			continue;
		} else if (isEnd(val.p, end)) {
			return score;
		}
		auto adj = getAdjacent(val.p);
		for (const auto& a : adj) {
			if (!isValidPoint(a, end)) {
				continue;
			}
			size_t index = calcIndex(a);
			int ns = val.score + grid[index];
			if (ns < min_scores[index]) {
				pq.push(Data(a, ns));
				min_scores[index] = ns;
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	vector<int> grid;
	vector<int> min_scores;

	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	Point pos(0, 0);
	while (std::getline(file, line)) {
		if (grid.size() == 0) {
			HEIGHT = line.size() * EXPANSIONS;
			grid.resize(HEIGHT * HEIGHT);
			min_scores.resize(HEIGHT * HEIGHT);
		}
		pos.x = 0;
		for (auto c : line) {
			insertPositions(grid, min_scores, pos, c - '0', line.size());
			pos.x += 1;
		}
		pos.y += 1;
	}
	file.close();

	pos.x *= EXPANSIONS;
	pos.y *= EXPANSIONS;

	cout << pos << endl;
	grid[0] = 0;
	cout << dijkstra(grid, min_scores, pos) << endl;
	return 0;
}
