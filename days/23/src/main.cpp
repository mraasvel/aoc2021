#include "include.hpp"
#include <queue>
#include <limits>

using namespace std;

typedef long PointValue;

typedef util::Point<PointValue> Point;
typedef util::HashPoint<PointValue> HashPoint;
typedef std::unordered_set<Point, HashPoint> PointSet;
// typedef std::unordered_map<Point, VALUE, HashPoint> PointHashMap;

unordered_map<char, int> move_costs = {
	{'A', 1},
	{'B', 10},
	{'C', 100},
	{'D', 1000}
};

unordered_map<char, size_t> target_room {
	{'A', 0},
	{'B', 1},
	{'C', 2},
	{'D', 3}
};

PointSet forbidden;

struct Square {
	Square(char c = '#')
	: c(c) {}

	void set(char c) {
		this->c = c;
	}

	void clear() {
		c = '.';
	}

	char c;
};

struct Room {
	Room() {}
	Point p1, p2;

	bool isFull(const vector<vector<Square>>& grid, char exp) const {
		return grid[p1.y][p1.x].c == exp
			&& grid[p2.y][p2.x].c == exp;
	}
};

struct Amphibian {
	Amphibian(char c, Point p)
	: c(c), p(p), moved(false) {}
	char c;
	Point p;
	bool moved;
};

struct State {
	State(size_t cost = 0)
	: cost(cost) {}

	size_t cost;
	vector<vector<Square>> grid;
	Room rooms[4];
	vector<Amphibian> amphs;

	bool operator<(const State& rhs) const {
		return cost > rhs.cost;
	}

	vector<State> expand() {
		vector<State> expansions;
		for (Amphibian& a : amphs) {
			if (!shouldMove(a)) {
				continue;
			}
			if (moveIntoRoom(a, expansions)) {
				continue;
			}
			if (a.moved) {
				continue;
			}
			a.moved = true;
			get(a.p).clear();
			moveAmphibian(a, expansions);
			a.moved = false;
			get(a.p).set(a.c);
		}
		return expansions;
	}

	bool inRoom(const Amphibian& a) const {
		for (int i = 0; i < 4; i++) {
			if (a.p == rooms[i].p1 || a.p == rooms[i].p2) {
				return true;
			}
		}
		return false;
	}

	bool shouldMove(const Amphibian& a) const {
		size_t index = target_room[a.c];
		return !(rooms[index].p2 == a.p
			|| rooms[index].isFull(grid, a.c));
	}

	size_t moveDistance(Point a, Point b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	}

	bool moveIntoRoom(Amphibian& a, vector<State>& expansions) {
		size_t index = target_room[a.c];
		size_t steps = 0;
		if (canMoveIntoRoom(a, index, steps)) {
			Point target = rooms[index].p2;
			if (get(rooms[index].p2).c == a.c) {
				target = rooms[index].p1;
			}
			get(a.p).clear();
			expansions.push_back(createExpansion(a, target, cost + move_costs[a.c] * steps));
			get(a.p).set(a.c);
			return true;
		}
		return false;
	}

	bool canMoveIntoRoom(const Amphibian& a, size_t index, size_t& steps) {
		if (get(rooms[index].p1).c != '.' || get(rooms[index].p2).c != a.c) {
			return false;
		}
		Point pos(a.p);
		Point end = Point(rooms[index].p1);
		end.y -= 1;
		bool left = a.p.x > end.x;
		if (a.p.y == 2 && get(Point(a.p.x, a.p.y - 1)).c == '.') {
			++steps;
			pos.y -= 1;
		}
		while (pos != end) {
			if (left) {
				pos.x -= 1;
			} else {
				pos.x += 1;
			}
			if (get(pos).c != '.') {
				return false;
			}
			++steps;
		}
		++steps;
		if (get(rooms[index].p2).c == '.') {
			++steps;
		}
		return true;
	}

	bool shouldExplore(Point p) const {
		return get(p).c == '.';
	}

	State createExpansion(Amphibian& a, Point target, size_t move_cost) const {
		Point orig = a.p;
		a.p = target;
		State new_state(*this);
		a.p = orig;
		new_state.get(target).set(a.c);
		new_state.cost = move_cost;
		return new_state;
	}

	void moveAmphibian(Amphibian& a, vector<State>& expansions) const {
		auto adj = util::getEdgeAdjacentPoints(a.p);
		PointSet explored;
		std::queue<pair<Point, size_t>> to_explore;
		size_t move_cost = move_costs[a.c];
		explored.insert(a.p);
		for (auto ad : adj) {
			if (shouldExplore(ad)) {
				to_explore.push(make_pair(ad, cost + move_cost));
				explored.insert(ad);
			}
		}
		while (!to_explore.empty()) {
			auto exp = to_explore.front();
			to_explore.pop();
			if (forbidden.count(exp.first) == 0) {
				// Create a new state
				expansions.push_back(createExpansion(a, exp.first, exp.second));
			}
			auto adj = util::getEdgeAdjacentPoints(exp.first);
			for (const auto& ad : adj) {
				if (!shouldExplore(ad) || explored.count(ad) > 0
					|| (forbidden.count(exp.first) > 0 && ad.y == exp.first.y - 1)) {
					continue;
				}
				to_explore.push(make_pair(ad, exp.second + move_cost));
				explored.insert(ad);
			}
		}
	}

	bool isGoal() const {
		return rooms[0].isFull(grid, 'A')
			&& rooms[1].isFull(grid, 'B')
			&& rooms[2].isFull(grid, 'C')
			&& rooms[3].isFull(grid, 'D');
	}

	void initRooms() {
		size_t index = 0;
		for (size_t i = 0; i < grid[2].size(); i++) {
			if (isalpha(grid[2][i].c)) {
				forbidden.insert(Point(i, 1));
				rooms[index].p1 = Point(i, 2);
				rooms[index].p2 = Point(i, 3);
				amphs.push_back(Amphibian(grid[2][i].c, Point(i, 2)));
				amphs.push_back(Amphibian(grid[3][i].c, Point(i, 3)));
				index++;
			}
		}
	}

	template <typename Function>
	void forEach(Function f) {
		for (size_t y = 0; y < grid.size(); y++) {
			for (size_t x = 0; x < grid[y].size(); x++) {
				f(Point(x, y));
			}
		}
	}

	template <typename Function>
	void forEach(Function f) const {
		for (size_t y = 0; y < grid.size(); y++) {
			for (size_t x = 0; x < grid[y].size(); x++) {
				f(Point(x, y));
			}
		}
	}

	Square& get(Point p) {
		return grid[p.y][p.x];
	}

	const Square& get(Point p) const {
		return grid[p.y][p.x];
	}

	void print() const {
		forEach([this](Point p) -> void {
			cout << get(p).c;
			if ((size_t)(p.x + 1) == this->grid[p.y].size()) {
				cout << endl;
			}
		});
	}
};

bool isGoalState(const State& state) {
	return state.isGoal();
}

size_t solve(const State& initial) {
	std::priority_queue<State> states;
	states.push(initial);
	size_t min_cost = std::numeric_limits<size_t>::max();
		static int i = 0;
	while (!states.empty()) {
		State state = states.top();
		states.pop();
		if (isGoalState(state)) {
			cout << "Goal State" << ": " << state.cost << endl;
			state.print();
			min_cost = std::min(min_cost, state.cost);
			exit(0);
			continue;
		}
		vector<State> expansions = state.expand();
		for (const State& x : expansions) {
			states.push(x);
		}
		cout << "States: " << states.size() << endl;
	}
	return min_cost;
}

vector<Square> parseLine(const string& line, size_t x) {
	vector<Square> squares;
	for (size_t i = 0; i < line.size(); i++) {
		squares.push_back(Square(line[i]));
	}
	while (x-- > 0) {
		squares.push_back(Square(' '));
	}
	return squares;
}

int main(int argc, char *argv[]) {
	State initial_state;
	assert(argc != 1);
	std::ifstream file(argv[1]);
	assert(file.is_open());
	std::string line;
	size_t x = 0;
	while (getline(file, line)) {
		if (x == 0) {
			x = line.size();
		}
		initial_state.grid.push_back(parseLine(line, x - line.size()));
	}
	initial_state.print();
	initial_state.initRooms();
	size_t result = solve(initial_state);
	cout << "P1: " << result << endl;
	return 0;
}
