#include "include.hpp"
#include <queue>

using namespace std;

typedef util::Point<size_t> Point;
typedef util::HashPoint<size_t> HashPoint;
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

size_t calculateNewScore(int val) {
	return val < 10 ? val : val % 10 + 1;
}

vector<pair<Point, size_t>> newPoints(Point p, size_t w, size_t h, int val) {
	vector<pair<Point, size_t>> result;

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (i == 0 && j == 0) {
				continue;
			}
			Point new_pos(p.x + j * w, p.y + i * h);
			result.push_back(pair<Point, size_t>(new_pos, calculateNewScore(val + i + j)));
		}
	}

	return result;
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
	return p.x < end.x && p.y < end.y;
}

bool isEnd(Point p, Point end) {
	return p.x == end.x - 1 && p.y == end.y - 1;
}

size_t dijkstra(const PointHash& grid, PointHash& min_scores, Point end) {
	std::priority_queue<Data> pq;
	pq.push(Data(Point(0, 0), 0));
	int i = 0;
	while (!pq.empty()) {
		auto val = pq.top();
		pq.pop();
		size_t score = val.score;
		if (min_scores[val.p] < score) {
			continue;
		} else if (isEnd(val.p, end)) {
			cout << score << endl;
			return score;
		}
		auto adj = getAdjacent(val.p);
		for (const auto& a : adj) {
			if (!isValidPoint(a, end)) {
				continue;
			}
			size_t ns = val.score + grid.at(a);
			if (ns < min_scores[a]) {
				pq.push(Data(a, ns));
				min_scores[a] = ns;
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	PointHash grid;
	PointHash min_scores;


	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	Point pos(0, 0);
	while (std::getline(file, line)) {
		pos.x = 0;
		for (auto c : line) {
			grid[pos] = (c - '0');
			pos.x += 1;
		}
		pos.y += 1;
	}
	file.close();

	PointHash new_grid(grid);
	for (auto it : grid) {
		auto v = newPoints(it.first, pos.x, pos.y, it.second);
		min_scores[it.first] = std::numeric_limits<std::size_t>::max();
		for (auto p : v) {
			new_grid[p.first] = p.second;
			min_scores[p.first] = std::numeric_limits<std::size_t>::max();
		}
	}

	pos.x *= 5;
	pos.y *= 5;

	grid[Point(0, 0)] = 0;
	dijkstra(new_grid, min_scores, pos);
	return 0;
}
