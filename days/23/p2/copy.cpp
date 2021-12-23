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
	: cost(cost), prev(nullptr) {}

	size_t cost;
	size_t mincost;
	vector<vector<Square>> grid;
	Room rooms[4];
	vector<Amphibian> amphs;
	State* prev;

	bool operator==(const State& rhs) const {
		for (size_t y = 0; y < grid.size(); y++) {
			for (size_t x = 0; x < grid[y].size(); x++) {
				if (grid[y][x].c != rhs.grid[y][x].c) {
					return false;
				}
			}
		}
		return true;
	}

	bool operator<(const State& rhs) const {
		return mincost > rhs.mincost;
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
			if (!inRoom(a)) {
				continue;
			}
			a.moved = true;
			get(a.p).clear();
			moveAmphibian(a, expansions);
			a.moved = false;
			get(a.p).set(a.c);
		}
		// for (const auto& exp : expansions) {
		// 	exp.print();
		// }
		// exit(0);
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
		if (!(rooms[index].isFull(grid, '.')
			|| (get(rooms[index].p1).c == '.' && get(rooms[index].p2).c == a.c))) {
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

	void computeMinCost() {
		mincost = cost;
		for (const auto& amph: amphs) {
			Point target = rooms[target_room[amph.c]].p1;
			if (amph.p.x != target.x) {
				long dist = std::abs(amph.p.x - target.x) + amph.p.y;
				mincost += dist * move_costs[amph.c];
			}
		}
	}

	State createExpansion(Amphibian& a, Point target, size_t move_cost) {
		Point orig = a.p;
		a.p = target;
		State new_state(*this);
		a.p = orig;
		new_state.get(target).set(a.c);
		new_state.cost = move_cost;
		new_state.computeMinCost();
#ifdef HISTORY
		get(a.p).set(a.c);
		new_state.prev = new State(*this);
		get(a.p).clear();
#endif
		return new_state;
	}

	void moveAmphibian(Amphibian& a, vector<State>& expansions) {
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
		forEach([this](Point p) {
			if (isalpha(this->get(p).c)) {
				amphs.push_back(Amphibian(this->get(p).c, p));
			}
		});

		for (int i = 0; i < 4; i++) {
			forbidden.insert(Point(3 + i * 2, 1));
			rooms[i].p1 = Point(3 + i * 2, 2);
			rooms[i].p2 = Point(3 + i * 2, 3);
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
		cout << "Score: " << cost << endl;
		forEach([this](Point p) -> void {
			cout << get(p).c;
			if ((size_t)(p.x + 1) == this->grid[p.y].size()) {
				cout << endl;
			}
		});
		cout << endl;
	}
};

struct HashState {
	size_t operator()(const State& x) const {
		size_t hash = 0;
		for (const auto& amph : x.amphs) {
			hash ^= HashPoint()(amph.p);
		}
		return hash;
	}
};

unordered_map<State, size_t, HashState> found_states;

bool isGoalState(const State& state) {
	return state.isGoal();
}

void printPreviousStates(const State& state) {
	if (state.prev == nullptr) {
		state.print();
	} else {
		printPreviousStates(*state.prev);
		state.print();
	}
}

void saveState(const State& x) {
	found_states[x] = x.mincost;
}

size_t solve(const State& initial) {
	std::priority_queue<State> states;
	states.push(initial);
	size_t min_cost = std::numeric_limits<size_t>::max();
	while (!states.empty()) {
		State state = states.top();
		states.pop();
		if (state.mincost > found_states[state]) {
			continue;
		}
		if (isGoalState(state)) {
			cout << "Goal State" << endl;
			printPreviousStates(state);
			min_cost = std::min(min_cost, state.cost);
			exit(0);
			continue;
		}
		vector<State> expansions = state.expand();
		for (const State& x : expansions) {
			if (found_states.count(x) > 0) {
				if (x.mincost >= found_states[x]) {
					continue;
				}
			}
			states.push(x);
			saveState(x);
		}
		cout << state.mincost << endl;
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
	initial_state.computeMinCost();
	found_states[initial_state] = initial_state.mincost;
	size_t result = solve(initial_state);
	cout << "P1: " << result << endl;
	return 0;
}
