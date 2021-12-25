#include "include.hpp"

using namespace std;

typedef int PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
typedef std::unordered_map<Point, char, HashPoint> PointHashMap;

size_t WIDTH = 0;
size_t HEIGHT = 0;

struct State {
	State()
	: steps(0) {}
	State(const vector<string>& lines)
	: steps(0) {
		for (size_t y = 0; y < lines.size(); y++) {
			for (size_t x = 0; x < lines[y].size(); x++) {
				if (lines[y][x] == '.') {
					continue;
				}
				if (lines[y][x] == '>') {
					rights.push_back(Point(x, y));
				} else if (lines[y][x] == 'v') {
					downs.push_back(Point(x, y));
				} else {
					throw std::runtime_error("Invalid character: " + lines[y].substr(x, 1));
				}
				grid[Point(x, y)] = lines[y][x];
			}
		}
	}

	void swap(State& rhs) {
		std::swap(grid, rhs.grid);
		std::swap(rights, rhs.rights);
		std::swap(downs, rhs.downs);
		std::swap(steps, rhs.steps);
	}

	State expand() const {
		State next;
		// Rights first
		for (const Point& p : rights) {
			Point target(p.x + 1, p.y);
			if (target.x == (int)WIDTH) {
				target.x = 0;
			}
			// Check if there is another cucumber in the spot it's trying to move into.
			Point index;
			if (grid.count(target) == 0) {
				index = target;
				next.steps++;
			} else {
				index = p;
			}
			next.grid[index] = '>';
			next.rights.push_back(index);

		}

		for (const Point& p : downs) {
			Point target(p.x, p.y + 1);
			if (target.y == (int)HEIGHT) {
				target.y = 0;
			}
			// Either there is a 'v' in the current grid or a '>' in the next grid
			Point index;
			if (next.grid.count(target) == 0
				&& (grid.count(target) == 0 || grid.at(target) == '>')) {
				index = target;
				next.steps++;
			} else {
				index = p;
			}
			next.grid[index] = 'v';
			next.downs.push_back(index);
		}
		return next;
	}

	void print() const {
		for (size_t y = 0; y < HEIGHT; y++) {
			for (size_t x = 0; x < WIDTH; x++) {
				if (grid.count(Point(x, y)) == 0) {
					cout << '.';
				} else {
					cout << grid.at(Point(x, y));
				}
			}
			cout << endl;
		}
	}

	size_t steps;
	PointHashMap grid;
	vector<Point> rights;
	vector<Point> downs;
};

vector<string> readLines(ifstream& file) {
	vector<string> lines;
	string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		lines.push_back(line);
	}
	return lines;
}

int main(int argc, char *argv[]) {
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	vector<string> lines = readLines(file);
	file.close();
	WIDTH = lines[0].size();
	HEIGHT = lines.size();
	cout << "Width: " << WIDTH << endl;
	cout << "Height: " << HEIGHT << endl;

	State initial(lines);
	size_t i = 0;
	while (true) {
		State next = initial.expand();
		i++;
		initial.swap(next);
		if (initial.steps == 0) {
			break;
		}
	}
	cout << i << endl;
	return 0;
}
