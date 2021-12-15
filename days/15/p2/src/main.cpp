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
	return p.x < end.x && p.y < end.y && p.x >= 0 && p.y >= 0;
}

bool isEnd(Point p, Point end) {
	return p.x == end.x - 1 && p.y == end.y - 1;
}

size_t dijkstraHash(const PointHash& grid, PointHash& min_scores, Point end) {
	std::priority_queue<Data> pq;
	pq.push(Data(Point(0, 0), 0));
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

#define HEIGHT 500
#define ORIG_HEIGHT 100

size_t calcIndex(Point p) {
	return p.y * HEIGHT + p.x;
}

void insertPositions(vector<int>& grid, vector<int>& min_scores, Point pos, int val, size_t w) {
	for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 5; ++x) {
			Point p(pos.x + x * w, pos.y + y * w);
			size_t index = calcIndex(Point(pos.x + x * w, pos.y + y * w));
			grid[index] = val + x + y < 10 ? val + x + y : ((val + x + y) % 10) + 1;
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

	grid.resize(500 * 500);
	min_scores.resize(500 * 500);



	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	Point pos(0, 0);
	while (std::getline(file, line)) {
		pos.x = 0;
		for (auto c : line) {
			insertPositions(grid, min_scores, pos, c - '0', line.size());
			pos.x += 1;
		}
		pos.y += 1;
	}
	file.close();

	pos.x *= 5;
	pos.y *= 5;

	cout << pos << endl;
	grid[0] = 0;
	cout << dijkstra(grid, min_scores, pos) << endl;
	return 0;
}
